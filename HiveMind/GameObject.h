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
////////
class GameObject;

enum class ComponentType
{
	NONE,
	TRANSFORM,
	MESH,
	MATERIAL,
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
		transform = IdentityMatrix;
		position = vec3(0, 0, 0);
		scale = vec3(1, 1, 1);
	}
	~TransformC() {}


	void Update() override {
		if (active)
		{
			if (updateTransform) {
				transform.translate(position.x, position.y, position.z);
				if(scale.x != 0 && scale.y != 0 && scale.z != 0)	transform.scale(scale.x, scale.y, scale.z);
				updateTransform = false;

			}
			if (rotation.x) {
				SetRotation(rotation.x, vec3(1, 0, 0));
				rotationX = false;
			}
			if (rotation.y){
				SetRotation(rotation.y, vec3(0, 1, 0));
				rotationY = false;
			}
			if (rotation.z){
				SetRotation(rotation.z, vec3(0, 0, 1));
				rotationZ = false;
			}

		}

	}

	void SetPos(float x, float y, float z)
	{
		transform.translate(x, y, z);
	}
	void SetRotation(float angle, const vec3& u)
	{
		transform.rotate(angle, u);
	}
	
		
	

	vec3 GetPos() {
		return transform.translation(); 
	}

public:

	mat4x4 transform;
	vec3 position;
	vec3 rotation;
	bool rotationX;
	bool rotationY;
	bool rotationZ;
	vec3 scale;
	bool axis = false;
	bool wire = false;
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

	bool InitTexture(const aiScene* pScene, const char* Filename);

private:
	void InitFromScene(const aiScene* pScene, const char* fileName);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void Clear();
	void Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord,
		const std::vector<unsigned int>& Indices);
private:
	const char* filePath;

	GLuint meshTextureID;
	GLuint texture;
	GLuint textureID;
	uint CHECKERS_HEIGHT = 64;
	uint CHECKERS_WIDTH = 64;
	GLubyte checkerImage[64][64][4];

	GLuint VB;
	GLuint TB;
	GLuint IB;
	unsigned int numIndices;
	unsigned int materialIndex;

	std::vector<MeshC> m_Entries;
	std::vector<const aiMesh*> activeMeshes;

};

class GameObject
{
public:
	GameObject(const char* name, GameObject* _parent, const char* filePath, int _id = -1);
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
	int	 id = 0;
	//LALA : Vector de compontes 
	//o un componente de cada tipo
	
	std::vector<Component*> components;
	TransformC* transform; 
	MeshC* mesh;

	GameObject* parent; 
	std::vector<GameObject*> childs; 
	const char* name;

};