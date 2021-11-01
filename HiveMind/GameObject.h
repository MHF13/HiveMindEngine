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

	virtual bool  GetEnable() { return active; }
	virtual void  SetEnable(bool state) { active = state; }
	virtual void Destroy() {  }


	GameObject* GetOwner() { return owner; }
	
	bool active = false;
	bool updateTransform = false;
	ComponentType type = ComponentType::NONE;

};

class TransformC : public Component {
public:
	TransformC(){}
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
	MeshC(GameObject* _owner, const char* fileName) : Component(_owner, ComponentType::MESH)
	{
		vertexB = 0;
		indexB = 0;
		numIndx = 0;

		LoadMesh(fileName);
	}
	
	~MeshC()
	{
		if (vertexB != 0)
		{
			glDeleteBuffers(1, &vertexB);
		}

		if (indexB != 0)
		{
			glDeleteBuffers(1, &indexB);
		}
		//Clear();
	}
	void Update() override;

	bool LoadMesh(const char* fileName);

private:
	//void InitFromScene(unsigned int index, const aiMesh* aiMesh);
	void Init(unsigned int index, const aiMesh* aiMesh);
	void Buffers(const std::vector<vec3>& vertex, const std::vector<vec2>& textureC, const std::vector<vec3>& normals, const std::vector<unsigned int>& index);

private:
	uint materialIndx = 0;
	std::vector<MeshC> meshEnt;
	std::vector<const aiMesh*> meshes;

public:

	GLuint textureId = NULL;
	GLuint vertexB = NULL;
	GLuint textureB = NULL;
	GLuint indexB = NULL;
	GLuint normalB = NULL;
	uint numIndx = 0;

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

	void LoadTexture(const char* texturePath);

	GLuint texture = NULL;
	GLuint widthTex;
	GLuint heightTex;
private:
};

class GameObject
{
public:

	GameObject(const char* name, GameObject* _parent, const char* filePath, const char* materialPath);
	virtual ~GameObject();

	void Update();

	virtual bool  GetEnable() { return active; }
	virtual void  SetEnable(bool state) { active = state; }

	void AddComponent(ComponentType type, const char* fileName = "");
	void RemoveComponent(ComponentType type);

	Component* GetAllComponents(ComponentType type);
	Component* GetComponent(ComponentType _type);

public:
	bool active = false;

	std::vector<Component*> components;
	TransformC* transform; 
	MeshC* mesh;
	TextureC* texture;

	GameObject* parent; 
	std::vector<GameObject*> childs; 
	const char* name;

};