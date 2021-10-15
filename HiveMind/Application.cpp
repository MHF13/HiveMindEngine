#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	loadModel = new LoadModel(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(loadModel);

	// Scenes
	AddModule(scene_intro);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator i = list_modules.rbegin(); i != list_modules.rend(); ++i)
	{
		delete (*i);
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (Module*& i : list_modules)
	{
		i->Init();
	}


	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (Module*& i : list_modules)
	{
		i->Start();
	}

	
	frameTime.Start();

	//---
	framerBlock = 60;
	//---

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{

	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	cappedMs = (1000 / framerBlock)-dt;
	if (cappedMs > 0)
	{
		SDL_Delay(cappedMs);
	}

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (Module*& i : list_modules)
	{
		if (ret == UPDATE_CONTINUE)
		{
		ret = i->PreUpdate(dt);
		}

	}

	for (Module*& i : list_modules)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = i->Update(dt);
		}

	}

	for (Module*& i : list_modules)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = i->PostUpdate(dt);
		}

	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::list<Module*>::reverse_iterator i = list_modules.rbegin(); i != list_modules.rend(); ++i)
	{
		if (ret)
		{
			ret = (*i)->CleanUp();
		}
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}