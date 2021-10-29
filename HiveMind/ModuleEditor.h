#pragma once
#include "Module.h"
#include "Globals.h"
#include <gl/GL.h>
#include "GameObject.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void UpdateInspector(GameObject* go);


private:

	void RecolVector(std::vector<float>* vec, int size, float dt);
	void RecolVector(std::vector<float>* vec, int size, float* push, int toMultiply);

	void HierarchyList(GameObject* list);

	bool showGuiDemo = false;
	bool showConfig = false;
	bool showAboutWindow = false;
	bool showSmallExample = true;
	bool closeWindow = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::vector<float> fps_log;
	std::vector<float> Milliseconds_log;
	int columns;

	///////////bools checkbox
	bool windowActive = true;
	bool hardwareActive = true;


	/////WINDOWS

	bool hierarchy = true;
	GameObject* selectedH;

	bool inspector = true;

	//Console
	bool console = true;

	
};