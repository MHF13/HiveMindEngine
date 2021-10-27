#pragma once
#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include "Globals.h"



class Component {
public:

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
	void Destroy() { delete(this); }

	enum class ComponentType
	{
		NONE,
		TRANSFORM,
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

private:
	bool enabled = false;
	int	 id = 0;
	std::vector<Component*> components;
	GameObject* daddy; //parent
	std::vector<GameObject*> kodomus; //childs
	const char* name;
};

#endif