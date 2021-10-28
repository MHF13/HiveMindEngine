#pragma once

#include <vector>
#include "Globals.h"

//Transform
#include "glmath.h"
#include "MathGeoLib.h"

class GameObject;
class MeshC;

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

class GameObject
{
public:
	GameObject(const char* name, GameObject* _parent, int _id = -1);
	virtual ~GameObject();

	void Update();

	void Enable();
	void Disable();
	void CleanUp();

	void AddComponent(ComponentType type);
	void RemoveComponent(ComponentType type);

	Component* GetAllComponents(ComponentType type);

public:
	bool enabled = false;
	int	 id = 0;
	//LALA : Vector de compontes 
	//o un componente de cada tipo
	std::vector<Component*> components;
	TransformC* transform;

	GameObject* parent; 
	std::vector<GameObject*> childs; 
	const char* name;

	MeshC* mesh;
};