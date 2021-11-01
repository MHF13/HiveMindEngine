#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ManagerGameObject.h"
#include "ModuleEditor.h"
#include <list>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* sceneIntro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ManagerGameObject* moduleGameObject;

private:


	Timer frameTime;

	float dt = 0.0f;
	float framerate;
	float cappedMs = -1;

	float fPS = 0;

	std::list<Module*> listModules;

public:
	int framerBlock;

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	void SaveConfigu();
	void LoadConfigu();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();



};