#include "Globals.h"
#include "Application.h"
#include "LoadModel.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"

LoadModel::LoadModel(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

LoadModel::~LoadModel()
{}

// -----------------------------------------------------------------
bool LoadModel::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

// -----------------------------------------------------------------
bool LoadModel::CleanUp()
{
	LOG("Cleaning camera");
	// detach log stream
	aiDetachAllLogStreams();
	
	return true;
}

