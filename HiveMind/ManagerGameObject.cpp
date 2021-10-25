#include "Globals.h"
#include "Application.h"
#include "ManagerGameObject.h"


#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ManagerGameObject::ManagerGameObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ManagerGameObject::~ManagerGameObject()
{}


// Called before render is available
bool ManagerGameObject::Init()
{
	bool ret = true;

	
	

	return ret;
}

// PreUpdate: clear buffer


// PostUpdate present buffer to screen
update_status ManagerGameObject::Update(float dt)
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ManagerGameObject::CleanUp()
{

	return true;
}


