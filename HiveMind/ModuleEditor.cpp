#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleSceneIntro.h"

#include "GUI/imgui.h"
#include "GUI/backends/imgui_impl_sdl.h"
#include "GUI/backends/imgui_impl_opengl3.h"
#include "SDL\include\SDL_opengl.h"

class MeshC;

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
    Milliseconds_log.push_back(0.f);
	columns = 32;
	return true;
}

update_status ModuleEditor::Update(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //Config
    
    if (showGuiDemo) ImGui::ShowDemoWindow(&showGuiDemo);
    if (showConfig)
    {
		
        ImGui::Begin("Configuration", &showConfig);

        ImGui::Text("Options");
        char title[50];

        // log FPS
        if (ImGui::CollapsingHeader("Application"))
        {
            static char str1[128] = "";
            ImGui::InputTextWithHint("App Name", "name", str1, IM_ARRAYSIZE(str1));

            static char str2[128] = "";
            ImGui::InputTextWithHint("Organization", "organization's name", str1, IM_ARRAYSIZE(str1));


            ImGui::SliderInt("Max FPS", &App->framerBlock, 1, 144);

            ImGui::Text("Limit Framerate %d", App->framerBlock);

            //ImGui::Text("Limit Framerate: %d", FPS);
			RecolVector(&fps_log, columns, dt);

            sprintf_s(title, columns, "Framerate %.1f", fps_log[fps_log.size() - 1]);
            ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

            RecolVector(&Milliseconds_log, 32, &dt, 1000);
            sprintf_s(title, 25, "Milliseconds %.1f", Milliseconds_log[Milliseconds_log.size() - 1]);
            ImGui::PlotHistogram("##framerate", &Milliseconds_log[0], Milliseconds_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));


        }
       // ImGui::End();
		
        if (ImGui::CollapsingHeader("Window"))
        {
            if(ImGui::Checkbox("Active", &windowActive))
            {
                App->window->width = 1080;
                App->window->height = 720;
                App->window->brightness = 1.0f;
                App->window->SetWindowModification();
            }
            if (windowActive)
            {
                ImGui::Text("Icon: *default*");

                if(ImGui::SliderFloat("Brightness", &App->window->brightness, 0.230f, 1.0f))
                    App->window->SetWindowModification();
                if(ImGui::SliderInt("Width", &App->window->width, 400, 1920))
                    App->window->SetWindowModification();
                if(ImGui::SliderInt("Height", &App->window->height, 300, 1080))
                    App->window->SetWindowModification();

				//
				if (ImGui::Checkbox("Fullscreen", &App->window->fullScreen))
					App->window->resizable = App->window->borderless = App->window->fullDesktop = false , App->window->SetWindowModification();
                ImGui::SameLine();
                if(ImGui::Checkbox("Resizable", &App->window->resizable))
					App->window->fullScreen = App->window->borderless = App->window->fullDesktop = false , App->window->SetWindowModification();
                if(ImGui::Checkbox("Borderless", &App->window->borderless))
					App->window->fullScreen = App->window->resizable = App->window->fullDesktop = false, App->window->SetWindowModification();
                ImGui::SameLine();
                if(ImGui::Checkbox("FullDesktop", &App->window->fullDesktop))
					App->window->fullScreen = App->window->resizable = App->window->borderless = false , App->window->SetWindowModification();

            }
          
        }
        if (ImGui::CollapsingHeader("Hardware"))
        {
            if (ImGui::Checkbox("Active", &hardwareActive))
            {

            }
            if (hardwareActive)
            {
                SDL_version version;
                SDL_GetVersion(&version);
                ImGui::Text("SDL Version:");
                ImGui::SameLine();
                ImGui::TextColored({ 255,255,0,1 }, "%d.%d.%d", version.major, version.minor, version.patch);

                ImGui::Separator();

                ImGui::Text("CPUs: ");
                ImGui::SameLine();
                ImGui::TextColored({ 255,255,0,1 }, "%d (Cache: %d)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

                ImGui::Text("System RAM:");
                ImGui::SameLine();
                ImGui::TextColored({ 255,255,0,1 }, "%d", SDL_GetSystemRAM() / 1024);

                ImGui::Text("Caps: ");
                ImGui::SameLine();
                ImGui::TextColored({ 255,255,0,1 }, "%d", SDL_GetSystemRAM() / 1024);

                ImGui::Separator();
                ImGui::Text("GPU :");
                ImGui::SameLine();
                ImGui::TextColored({ 255,255,0,1 }, "%s", glGetString(GL_RENDERER));

                ImGui::Text("Brand :");
                ImGui::SameLine();
                ImGui::TextColored({ 255,255,0,1 }, "%s", glGetString(GL_RENDERER));
                ImGui::Text("VRAM Budget :");
                ImGui::SameLine();
                // ImGui::TextColored({ 255,255,0,1 }, "%s", glGetString());

            }




        }
        ImGui::End();
		
    }

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("(demo menu)", NULL, false, false);
        if (ImGui::MenuItem("New"))
        {

        }
        if (ImGui::MenuItem("Open", "Ctrl+O"))
        {

        }
        if (ImGui::BeginMenu("Open Recent"))
        {
            ImGui::MenuItem("fish_hat.c");
            ImGui::MenuItem("fish_hat.inl");
            ImGui::MenuItem("fish_hat.h");
            if (ImGui::BeginMenu("More.."))
            {
                ImGui::MenuItem("Hello");
                ImGui::MenuItem("Sailor");
                if (ImGui::BeginMenu("Recurse.."))
                {
                    // ShowExampleMenuFile();
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {

        }
        if (ImGui::MenuItem("Save As.."))
        {

        }

        ImGui::Separator();
        if (ImGui::BeginMenu("Options"))
        {
            static bool enabled = true;
            ImGui::MenuItem("Enabled", "", &enabled);
            ImGui::BeginChild("child", ImVec2(0, 60), true);
            for (int i = 0; i < 10; i++)
                ImGui::Text("Scrolling Text %d", i);
            ImGui::EndChild();
            static float f = 0.5f;
            static int n = 0;
            ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
            ImGui::InputFloat("Input", &f, 0.1f);
            ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Colors"))
        {
            float sz = ImGui::GetTextLineHeight();
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
                ImVec2 p = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
                ImGui::Dummy(ImVec2(sz, sz));
                ImGui::SameLine();
                ImGui::MenuItem(name);
            }
            ImGui::EndMenu();
        }

        // Here we demonstrate appending again to the "Options" menu (which we already created above)
        // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
        // In a real code-base using it would make senses to use this feature from very different code locations.
        if (ImGui::BeginMenu("Options")) // <-- Append!
        {
            static bool b = true;
            ImGui::Checkbox("SomeOption", &b);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Disabled", false)) // Disabled
        {
            IM_ASSERT(0);
        }
        if (ImGui::MenuItem("Checked", NULL, true)) {}
        if (ImGui::MenuItem("Quit", "Alt+F4")) { return update_status::UPDATE_STOP; }


        ImGui::EndMenu();



    }
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Create"))
    {
        if (ImGui::MenuItem("Cube"))
        {
            cube = new GameObject("Cube", App->scene_intro->bigDaddy);
            App->scene_intro->bigDaddy->kodomus.push_back(cube);
            cube->AddComponent(Component::ComponentType::MESH);
            //App->scene_intro->bigDaddy->kodomus.back()->components.push_back(new MeshC("Assets/Models/fish.fbx"));
            cube->AddComponent(Component::ComponentType::TRANSFORM);
        }
        
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Options"))
    {
        if (ImGui::MenuItem("GUI Demo"))
        {
            showGuiDemo = !showGuiDemo;
        }
        if (ImGui::MenuItem("FPS and Millyseconds"))
        {
            showConfig = !showConfig;
        }
        if (ImGui::MenuItem("GUI Demo"))
        {

        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	return true;
}
//for FPS
void ModuleEditor::RecolVector(std::vector<float>* vec, int size, float dt)
{
	if (vec->size() <= size)
		vec->push_back(1/dt);
	else {
		vec->erase(vec->begin());
		vec->push_back(1/dt);
	}
}
//for Miliseconds
void ModuleEditor::RecolVector(std::vector<float>* vec, int size, float* push, int toMultiply)
{
	if (vec->size() <= size)
		vec->push_back((*push) * toMultiply);
	else {
		vec->erase(vec->begin());
		vec->push_back((*push) * toMultiply);
	}
}
