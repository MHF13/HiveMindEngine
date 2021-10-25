#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include <vector>



class ManagerGameObject : public Module
{
public:
	ManagerGameObject(Application* app, bool start_enabled = true);
	~ManagerGameObject();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* Create();
};