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
	AddComponent(ComponentType::TRANSFORM);
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
	//Component* ret = nullptr;
	if (type == ComponentType::TRANSFORM)
	{
		//LALA nuevo metodo para dar un componente transform, esto lo haremos cuando se cree un gameObject en el start
		// Si modificamos los componentes como el transform y los inicializamod con un valor por defeto para luego 
		// ponerle cosas puede funcionar

		//Component* newC = new TransformC(this);
		//components.push_back(newC);

		// o guardar la componente en una variable del tipo de la propia componente?
		// me he dado cuenta qqeu para acceder es como mas facil? no se
		transform = new TransformC(this);
		
	}
}

void GameObject::RemoveComponent(ComponentType type)
{

}
Component* GameObject::GetAllComponents(ComponentType type)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components.at(i)->type == type)
		{
			return components.at(i);
		}
	}

	return nullptr;

}
