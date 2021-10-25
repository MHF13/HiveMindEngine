#pragma once
#ifndef __GameObject_H__
#define __GameObject_H__

#include <list>

enum ComponentType
{
	ENABLED,
};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void Update();

	void Enable();
	void Disable();
	void CleanUp();

	void AddComponent(ComponentType type);
	void RemoveComponent(ComponentType type);

	struct Component {

		Component(ComponentType _type, GameObject* _owner)
		{
			type = _type;
			owner = _owner;
		}
		virtual ~Component()
		{
			Destroy();
		}

		virtual void Update() {};

		GameObject* GetOwner() { return owner; }
		ComponentType GetType() { return type; }

		void Enable() {active = true;}
		void Disable() {active = false;}
		bool GetEnabled(){return active;}
		void Destroy(){delete(this);}

	private:
		bool active = false;
		ComponentType type;
		std::list<Component*> components;
		GameObject* owner;
	};

	std::list<Component*> GetComponents();

	Component* FindComponentByType(ComponentType type);


private:

private:
	bool enabled = false;
	int	 id = 0;
	std::list<Component*> components;
};

#endif