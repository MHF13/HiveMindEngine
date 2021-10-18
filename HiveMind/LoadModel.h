#pragma once

#include "Module.h"

struct model
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};

class LoadModel : public Module
{
public:
	LoadModel(Application* app, bool start_enabled = true);
	~LoadModel();

	bool Start();
	
	bool CleanUp();
	



private:


	std::vector<model> models;


};


// 