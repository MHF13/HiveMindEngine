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

    //Exit 
    ImGui::Begin("Exit");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("Press to close");               // Display some text (you can use a format strings too)
    if (ImGui::Button("Close"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        return update_status::UPDATE_STOP;
    ImGui::End();
    
    //Config
    ImGui::Begin("Configuration");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("Options");   
    char title[25];
    
	// log FPS
	if (fps_log.size()<=32){
		fps_log.push_back(*App->GetLastFrameRate());
	}
	else
	{
		//RecolVector(&fps_log);
		fps_log.erase(fps_log.begin());
		fps_log.push_back(*App->GetLastFrameRate());
	}

    sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size()-1]);
    ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(),0,title, 0.0f,100.0f, ImVec2(310,100));
    ImGui::End();

    //Demo
    ImGui::Begin("Demo Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Click to open the demostration window");
    ImGui::Checkbox("Demo Window", &showGuiDemo);

    if (showGuiDemo) ImGui::ShowDemoWindow(&showGuiDemo);

    ImGui::End();

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("GUI Demo"))
        {
            //showcase = !showcase;
        }
        if (ImGui::MenuItem("Documentation"))
        {

        }
        if (ImGui::MenuItem("GUI Demo"))
        {

        }
        if (ImGui::MenuItem("GUI Demo"))
        {

        }

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    //ImGuiIO& io = ImGui::GetIO(); (void)io;
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

void ModuleEditor::RecolVector(std::vector<float> *vec)
{
	vec->erase(vec->begin());

}
