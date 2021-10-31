#pragma once

#include <vector>
#include "Globals.h"

//Inspector
#include "GUI/imgui.h"
#include "GUI/backends/imgui_impl_sdl.h"
#include "GUI/backends/imgui_impl_opengl3.h"

//Transform
#include "glmath.h"
#include "MathGeoLib.h"
#include "glmath.h"

//Mesh
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#pragma comment (lib, "ExternalLib/Assimp/libx86/assimp-vc142-mt.lib")

#include "glew.h"

#include <Importer.hpp>      // C++ importer interface  // Output data structure
#include <gl/GL.h>
#include <gl/GLU.h>

#include "il.h"
#include "ilut.h"

#pragma comment (lib, "devIL.lib")
#pragma comment (lib, "ILU.lib") 
#pragma comment (lib, "ILUT.lib")

////////
class GameObject;

enum class ComponentType
{
	NONE,
	TRANSFORM,
	MESH,
	TEXTURE,
};

class Component {
protected:
	GameObject* owner = nullptr;

public:
	Component(){}
	Component(GameObject* _owner,ComponentType _type) {
		owner = _owner;
		type = _type;
		active = true;
	}
	virtual ~Component()
	{
		Destroy();
	}

	virtual void Update() {};

	virtual void Draw() {};

	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	virtual void Destroy() {  }


	GameObject* GetOwner() { return owner; }
	
	bool active = false;
	bool updateTransform = false;
	ComponentType type = ComponentType::NONE;

};

class TransformC : public Component {
public:
	TransformC(GameObject* _owner) : Component(_owner, ComponentType::TRANSFORM) {

		transform.SetIdentity();
		transform.Decompose(position, rotQuaternion, scale);
		rotQuaternion.Normalize();
		rotEuler = rotQuaternion.ToEulerXYZ();

		global = transform.Transposed();
	}
	~TransformC() {}


	void Update() override {
		if (active)
		{
			if (updateTransform) {

				rotQuaternion = Quat::FromEulerXYZ(rotEuler.x * DEGTORAD, rotEuler.y * DEGTORAD, rotEuler.z * DEGTORAD);
				rotQuaternion.Normalize();

				transform = float4x4::FromTRS(position, rotQuaternion, scale);

				global = transform.Transposed();

				updateTransform = false;

			}
		}
	}
	
	const float* GetGlobalTransform() {
		return global.ptr();
	}
	
	void ResetTransform()
	{
		position = float3::zero;
		rotEuler = float3::zero;
		scale = float3::one;

		updateTransform = true;
	}

	float3 GetPos() {
		return position;
	}

public:

	float4x4 global;
	float4x4 transform;
	float3 position;
	float3 scale;
	float3 rotEuler;
	Quat rotQuaternion;

};

class MeshC : public Component
{
public:
	MeshC(){}
	MeshC(GameObject* _owner, const char* fileName);
	

	~MeshC()
	{
		if (VB != 0)
		{
			glDeleteBuffers(1, &VB);
		}

		if (IB != 0)
		{
			glDeleteBuffers(1, &IB);
		}
		//Clear();
	}
	void Update() override {
		if (active)
			Render();

	};

	void Draw() override {};
	bool LoadMesh(const char* fileName);
	void Render();


private:
	void InitFromScene(const aiScene* pScene, const char* fileName);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void Clear();
	void Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord,
		const std::vector<unsigned int>& Indices);

	const char* filePath;

	GLuint meshTextureId;
	GLuint texture;
	GLuint textureId;
	uint CHECKERS_HEIGHT = 64;
	uint CHECKERS_WIDTH = 64;
	GLubyte checkerImage[64][64][4];

	GLuint VB;
	GLuint TB;
	GLuint IB;
	unsigned int numIndices;
	unsigned int materialIndex;
public:
	uint numVertex = 0;
	std::vector<float3> vertices;

	std::vector<MeshC> mEntries;
	std::vector<const aiMesh*> activeMeshes;

};

class TextureC : public Component
{
public:
	TextureC() {}
	TextureC(GameObject* owner, const char* texturePath) : Component(owner, ComponentType::TEXTURE) {

		ilInit();
		iluInit();
		ilutInit();

		LoadTexture(texturePath);
	}

	~TextureC() {}
	//void Update() override {};

	void LoadTexture(const char* texturePath)
	{
		/*for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
				checkerImage[i][j][0] = (GLubyte)c;
				checkerImage[i][j][1] = (GLubyte)c;
				checkerImage[i][j][2] = (GLubyte)c;
				checkerImage[i][j][3] = (GLubyte)255;
			}
		}*/
	
		ILuint imageId;
		bool load = ilLoadImage(texturePath);
		ilGenImages(1, &imageId);
		ilBindImage(imageId);
		if (load)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
				0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
			glGenerateMipmap(GL_TEXTURE_2D);
			LOG("Texture Loaded");
		}

	}

private:
	GLuint textureId;
	GLubyte checkerImage[64][64][4];

	//std::vector<TextureC> textures;

};

class GameObject
{
public:

	GameObject(const char* name, GameObject* _parent, const char* filePath, const char* materialPath);
	virtual ~GameObject();

	void Update();

	bool GetEnable() { return enabled; };

	void Enable();
	void Disable();
	void CleanUp();

	void AddComponent(ComponentType type, const char* fileName = "");
	void RemoveComponent(ComponentType type);

	Component* GetAllComponents(ComponentType type);
	Component* GetComponent(ComponentType _type);

public:
	bool enabled = false;

	std::vector<Component*> components;
	TransformC* transform; 
	MeshC* mesh;
	TextureC* texture;

	GameObject* parent; 
	std::vector<GameObject*> childs; 
	const char* name;

};