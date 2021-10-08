#pragma once

#include "Module.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"


class LoadModel : public Module
{
public:
	LoadModel(Application* app, bool start_enabled = true);
	~LoadModel();

	bool Start() 
	{
		// Stream log messages to Debug window
		struct aiLogStream stream;
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
		aiAttachLogStream(&stream);
	}
	
	bool CleanUp()
	{
		// detach log stream
		aiDetachAllLogStreams();
	}


private:

	
};