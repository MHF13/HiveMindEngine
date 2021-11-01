#pragma once
#include "Module.h"
#include "Globals.h"
#include "GUI/imgui.h"
#include "GUI/backends/imgui_impl_sdl.h"
#include "GUI/backends/imgui_impl_opengl3.h"
#include "Glew.h"
#include "Primitive.h"
#include "GameObject.h"

#include <vector>

class GameObject;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool done = false;
	std::vector<float> vertices;
	GameObject* CreateObjectInScene(const char* name, GameObject* parent, const char* filePath,const char* materialPath);


public:

	GameObject* bigDaddy;

};
