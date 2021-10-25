#include "GameObject.h"

GameObject::GameObject()
{
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

void GameObject::AddComponent(ComponentType type)
{
}

void GameObject::RemoveComponent(ComponentType type)
{
}

std::list<GameObject::Component*> GameObject::GetComponents()
{
	return std::list<Component*>();
}

