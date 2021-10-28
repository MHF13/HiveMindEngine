#include "GameObject.h"

GameObject::GameObject(const char* _name, GameObject* _parent, int _id)
{
	name = _name;
	parent = _parent;
	enabled = true;
	id = _id;
	if (parent != nullptr)
	{
		parent->childs.push_back(this);
	}
}

GameObject::~GameObject()
{
}
void GameObject::Update()
{
}
void GameObject::Enable()
{
	enabled = true;
}

void GameObject::Disable()
{
	enabled = false;
}

void GameObject::CleanUp()
{
}

void GameObject::AddComponent(Component::ComponentType type)
{
	Component* ret = nullptr;
	if (type == Component::ComponentType::TRANSFORM)
	{
		//ret = new ComponentT
	}
}

void GameObject::RemoveComponent(Component::ComponentType type)
{

}
Component* GameObject::GetAllComponents(Component::ComponentType type)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components.at(i)->componentType == type)
		{
			return components.at(i);
		}
	}

	return nullptr;

}

