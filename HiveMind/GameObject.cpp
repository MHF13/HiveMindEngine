#include "Globals.h"
#include "Application.h"
#include "GameObject.h"


#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

GameObject::GameObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
GameObject::~GameObject()
{}


// Called before render is available
bool GameObject::Init()
{
	bool ret = true;

	
	

	return ret;
}

// PreUpdate: clear buffer
update_status GameObject::PreUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status GameObject::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool GameObject::CleanUp()
{

	return true;
}


