#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include <vector>

#define MAX_LIGHTS 8

class GameObject : public Module
{
public:
	GameObject(Application* app, bool start_enabled = true);
	~GameObject();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:

	
};