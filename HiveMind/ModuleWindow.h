#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void SetWindowModification();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int width = 1080;
	int height = 720;
	
	float brightness = 1;

	bool fullScreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullDesktop = false;

	
};

#endif // __ModuleWindow_H__