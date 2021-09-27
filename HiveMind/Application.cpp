#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);

	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(editor);
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	frameTime.Start();

	//---
	cappedMs = 1000 / 60;
	framerBlock = 60;
	//---

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	//dt = (float)frameTime.Read() / 1000.0f;
	dt = frameTime.ReadSec();
	frameTime.Start();
	FPS = SDL_GetTicks();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	float tmpFPS = SDL_GetTicks() - FPS;

	float secondsSinceStartup = startupTime.ReadSec();
	framerate = frameCount / secondsSinceStartup;
	timeFramesSecond += dt;
	if (timeFramesSecond > 1.0f)
	{
		framesOnLastSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		timeFramesSecond = 0;
	}

	oldLastFrame = lastFrameMs;
	lastFrameMs = lastSecFrameTime.Read();
	lastSecFrameTime.Start();

	if (cappedMs > tmpFPS)
	{
		// Use SDL_Delay to make sure you get your capped framerate
		SDL_Delay(cappedMs - tmpFPS);
	}
	//LOG("frame Rate: %f", framesOnLastSecond);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}