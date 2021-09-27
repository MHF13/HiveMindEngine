#pragma once
#include "Module.h"
#include <vector>


class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();



private:

	void RecolVector(std::vector<float>* vec, int size, float* push);
	void RecolVector(std::vector<float>* vec, int size, float* push, int toMultiply);

	bool showGuiDemo = false;
	bool showFPS = false;
	bool showSmallExample = true;
	bool closeWindow = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::vector<float> fps_log;
	std::vector<float> Milliseconds_log;
	float* deltaTime;

};