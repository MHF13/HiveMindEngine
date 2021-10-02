#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "ExternalLib/MathGeoLib/include/Geometry/Sphere.h"




#include "GUI/imgui.h"
#include "GUI/backends/imgui_impl_sdl.h"
#include "GUI/backends/imgui_impl_opengl3.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>



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

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	Primitive::Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	return UPDATE_CONTINUE;
}

