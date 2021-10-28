#pragma once

#include <vector>
#include "Globals.h"

//Transform
#include "glmath.h"
#include "MathGeoLib.h"

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

	virtual void Update() {
		
	};

	GameObject* GetOwner() { return owner; }

	void Enable() { active = true; }
	void Disable() { active = false; }
	void Destroy() {  }


	
	bool active = false;
	ComponentType type = ComponentType::NONE;

};

class TransformC : public Component {
public:
	TransformC(GameObject* _owner) : Component(_owner, ComponentType::TRANSFORM) {
		transform = IdentityMatrix;
	}
	~TransformC() {}

	void SetPos(float x, float y, float z)
	{
		transform.translate(x, y, z);
	}
	void SetRotation(float angle, const vec3& u)
	{
		transform.rotate(angle, u);
	}
	void Scale(float x, float y, float z)
	{
		transform.scale(x, y, z);
	}

	vec3 GetPos() {
		return transform.translation(); 
	}

public:

	mat4x4 transform;
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

	bool LoadMesh(const char* fileName);
	void Render();



private:
	bool InitFromScene(const aiScene* pScene, const char* fileName);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void Clear();
	void Init(const std::vector<float3>& Vertices, const std::vector<unsigned int>& Indices);

private:
	const char* filePath;

	GLuint textureID;
	uint CHECKERS_HEIGHT = 64;
	uint CHECKERS_WIDTH = 64;
	GLubyte checkerImage[64][64][4];

	GLuint VB;
	GLuint IB;
	unsigned int numIndices;
	unsigned int materialIndex;




	std::vector<MeshC> m_Entries;

};

class GameObject
{
public:
	GameObject(const char* name, GameObject* _parent, const char* filePath, int _id = -1);
	virtual ~GameObject();

	void Update();

	void Enable();
	void Disable();
	void CleanUp();

	void AddComponent(ComponentType type, const char* fileName = "");
	void RemoveComponent(ComponentType type);

	Component* GetAllComponents(ComponentType type);

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