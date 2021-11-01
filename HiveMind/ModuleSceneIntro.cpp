#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	bigDaddy = new GameObject("Scene", nullptr,NULL,NULL);
	
	//CreateObjectInScene("BakerHouse", bigDaddy, "Assets/Models/BakerHouse.fbx", NULL);
	CreateObjectInScene("BakerHouse", bigDaddy, "Assets/Models/BakerHouse.fbx", "Assets/Textures/bakeHouse.png");

	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}

GameObject* ModuleSceneIntro::CreateObjectInScene(const char* name, GameObject* parent, const char* filePath, const char* materialPath)
{
	GameObject* newGameObject = new GameObject(name, parent, filePath, materialPath);
	return newGameObject;

}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	Primitive::Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	/*Primitive::Cube s(1, 10, 20);
	s.Render();*/

	//MeshC fish("Assets/Models/BakerHouse.fbx");
	//fish.Render();

	//LALA
	for (int i = 0; i < bigDaddy->childs.size(); i++)
	{
		bigDaddy->childs.at(i)->Update();

	}


	return UPDATE_CONTINUE;
}

