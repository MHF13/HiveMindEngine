#pragma once

#include <vector>
#include "Globals.h"

class GameObject;
class MeshC;

class Component {
public:
	Component(){}
	Component(GameObject* _owner)
	{
		active = true;
		owner = _owner;
		type = ComponentType::NONE;
	}
	virtual ~Component()
	{
		Destroy();
	}

	virtual void Update() {};

	GameObject* GetOwner() { return owner; }

	void Enable() { active = true; }
	void Disable() { active = false; }
	void Destroy() {  }

	enum class ComponentType
	{
		NONE,
		TRANSFORM,
		MESH,
	};

	ComponentType componentType;

private:
	bool active = false;
	ComponentType type;
	GameObject* owner;
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

	void AddComponent(Component::ComponentType type);
	void RemoveComponent(Component::ComponentType type);

	Component* GetAllComponents(Component::ComponentType type);

public:
	bool enabled = false;
	int	 id = 0;
	std::vector<Component*> components;
	GameObject* daddy; //parent
	std::vector<GameObject*> kodomus; //childs
	const char* name;

	MeshC* mesh;
};