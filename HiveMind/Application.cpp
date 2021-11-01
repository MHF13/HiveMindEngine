#include "Application.h"

#include "parson.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	moduleGameObject = new ManagerGameObject(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(moduleGameObject);

	// Scenes
	AddModule(scene_intro);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator i = listModules.rbegin(); i != listModules.rend(); ++i)
	{
		delete (*i);
	}
}

bool Application::Init()
{
	bool ret = true;


	// Call Init() in all modules
	for (Module*& i : listModules)
	{
		i->Init();
	}


	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (Module*& i : listModules)
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

	for (Module*& i : listModules)
	{
		if (ret == UPDATE_CONTINUE)
		{
		ret = i->PreUpdate(dt);
		}

	}

	for (Module*& i : listModules)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = i->Update(dt);
		}

	}

	for (Module*& i : listModules)
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
	for (std::list<Module*>::reverse_iterator i = listModules.rbegin(); i != listModules.rend(); ++i)
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
	listModules.push_back(mod);
}
void Application::SaveConfigu()
{
	JSON_Value* file = json_parse_file("config.json");
	file = json_value_init_object();

	
	json_object_set_number(json_object(file), "maxfps", fPS);
	//json_object_set_number(json_object(file), "frameBlock", framerBlock);
	json_object_dotset_number(json_object(file), "brightness", window->brightness);
	json_object_dotset_number(json_object(file), "width", window->width);
	json_object_dotset_number(json_object(file), "height", window->height);
	json_object_dotset_boolean(json_object(file), "fullScreen", window->fullScreen);
	json_object_dotset_boolean(json_object(file), "resizable", window->resizable);
	json_object_dotset_boolean(json_object(file), "borderless", window->borderless);
	json_object_dotset_boolean(json_object(file), "fullDesktop", window->fullDesktop);

	
	json_serialize_to_file(file, "config.json");
	json_value_free(file);
}
void Application::LoadConfigu()
{
	
	JSON_Value* root = json_parse_file("config.json");

	if (root == nullptr)
	{
		LOG("Config flie not loaded\n");
		fPS = 60;

		window->width = 1080;
		window->height = 720;
		window->brightness = 1;
		window->SetWindowModification();

	}
	else
	{
		JSON_Object* object = json_value_get_object(root);
		
		fPS = (float)json_object_get_number(object, "maxfps");
		//framerBlock = (int)json_object_get_number(object, "framerBlock");

		window->brightness = (float)json_object_dotget_number(object, "brightness");
		window->width = (float)json_object_dotget_number(json_object(root), "width");
		window->height = (float)json_object_dotget_number(object, "height");
		window->fullScreen = (bool)json_object_dotget_boolean(object, "fullScreen");
		window->resizable = (bool)json_object_dotget_boolean(object, "resizable");
		window->borderless = (bool)json_object_dotget_boolean(object, "borderless");
		window->fullDesktop = (bool)json_object_dotget_boolean(object, "fullDesktop");

		window->SetWindowModification();

		char* serialized_string = json_serialize_to_string_pretty(root);
		json_free_serialized_string(serialized_string);

	}

	json_value_free(root);
}