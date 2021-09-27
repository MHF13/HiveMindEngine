#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;

private:

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint lastSecFrameCount = 0;
	uint prevLastSecFrameCount = 0;
	float framesOnLastSecond = 0;
	uint lastFrameMs = 0;

	float dt = 0.0f;
	float framerate;
	float framerBlock;
	float perfTime;
	float oldLastFrame = 0.0f;
	float timeFramesSecond = 0.0f;
	float cappedMs = -1;

	float FPS = 0;

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	bool GetDebugMode() { return debug; };
	void SetDebugMode() { debug = !debug; };

	float* GetLastFrameRate() { return &framesOnLastSecond; };
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();


	uint frames = 0;
	int frameCount = 0;

	bool debug = false;
};