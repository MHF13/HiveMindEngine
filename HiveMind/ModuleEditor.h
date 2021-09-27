#pragma once
#include "Module.h"
#include <vector>
#include <chrono>

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();



private:
	bool showGuiDemo = false;
	bool showSmallExample = true;
	bool closeWindow = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::vector<float> fps_log;

	std::chrono::steady_clock::time_point end;
	std::chrono::steady_clock::time_point begin;
};