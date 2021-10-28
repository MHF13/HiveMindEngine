#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "MeshC.h"


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

	bigDaddy = new GameObject("root", nullptr,NULL);
	//LALA
	bigDaddy->childs.push_back(CreateObjectInScene("BakerHouse", bigDaddy,"Assets/Models/cube.fbx"));
	LOG("%f.0  %f.0  %f.0", bigDaddy->childs.at(0)->transform->GetPos().x, bigDaddy->childs.at(0)->transform->GetPos().y, bigDaddy->childs.at(0)->transform->GetPos().z);
	
	
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

GameObject* ModuleSceneIntro::CreateObjectInScene(const char* name, GameObject* parent, const char* filePath, int id)
{
	GameObject* newGameObject = new GameObject(name, parent, filePath,id);
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
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN){

		bigDaddy->childs.at(0)->transform->SetPos(3,9,2);
		LOG("%s", bigDaddy->childs.at(0)->name);
		LOG("%f.0  %f.0  %f.0", bigDaddy->childs.at(0)->transform->GetPos().x, bigDaddy->childs.at(0)->transform->GetPos().y, bigDaddy->childs.at(0)->transform->GetPos().z);

	}
	bigDaddy->childs.at(0)->Update();


	return UPDATE_CONTINUE;
}

