#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "GUI/imgui.h"
#include "GUI/backends/imgui_impl_sdl.h"
#include "GUI/backends/imgui_impl_opengl3.h"
#include "SDL\include\SDL_opengl.h"



ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

    fps_log.push_back(0.f);
	return true;
}

update_status ModuleEditor::Update(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //Config
    

    if (showGuiDemo) ImGui::ShowDemoWindow(&showGuiDemo);
    if (showFPS)
    {
        ImGui::Begin("Configuration", &showFPS);                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Options");
        char title[25];

        // log FPS
		RecolVector(&fps_log, 32, App->GetLastFrameRate());
        sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
        ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		RecolVector(&Milliseconds_log, 32, &dt, 1000);
        sprintf_s(title, 25, "Milliseconds %.1f", Milliseconds_log[Milliseconds_log.size() - 1]);
        ImGui::PlotHistogram("##framerate", &Milliseconds_log[0], Milliseconds_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

        ImGui::End();
    }
 
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("GUI Demo"))
        {
            showGuiDemo = !showGuiDemo;
        }
        if (ImGui::MenuItem("FPS and Millyseconds"))
        {
            showFPS = !showFPS;
        }
        if (ImGui::MenuItem("GUI Demo"))
        {
            
        }
        if (ImGui::MenuItem("Exit"))
        {
            return update_status::UPDATE_STOP;
        }

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	return true;
}

void ModuleEditor::RecolVector(std::vector<float>* vec, int size, float* push)
{
	if (vec->size() <= size)
		vec->push_back(*push);
	else {
		vec->erase(vec->begin());
		vec->push_back(*push);
	}
}
void ModuleEditor::RecolVector(std::vector<float>* vec, int size, float* push, int toMultiply)
{
	if (vec->size() <= size)
		vec->push_back((*push) * toMultiply);
	else {
		vec->erase(vec->begin());
		vec->push_back((*push) * toMultiply);
	}
}
