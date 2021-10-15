#pragma once

#include "Module.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")


class LoadModel : public Module
{
public:
	LoadModel(Application* app, bool start_enabled = true);
	~LoadModel();

	bool Start();
	
	bool CleanUp();
	


private:

	
};