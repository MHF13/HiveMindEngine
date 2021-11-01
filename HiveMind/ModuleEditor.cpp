#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleSceneIntro.h"


#include "GUI/imgui.h"
#include "GUI/backends/imgui_impl_sdl.h"
#include "GUI/backends/imgui_impl_opengl3.h"
#include "SDL\include\SDL_opengl.h"

#include <list> 
#include <fstream>
#include <string>

#include <shellapi.h>

extern std::list<std::string> logs;

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
    
    //if (showGuiDemo) ImGui::ShowDemoWindow(&showGuiDemo);
    if (showAboutWindow)
    {
        ImGui::Begin("About", &showAboutWindow);
        SDL_version version;
        SDL_GetVersion(&version);
        ImGui::Text("HiveMindEngine v0.2");
        ImGui::Text("Give your games the best possible face lift!");
        ImGui::Text("By Mario Hernandez & Oriol Valverde");
        ImGui::Text("\n3rd Party Libraries used");
        ImGui::BulletText("SDL");
        ImGui::SameLine();
        ImGui::Text("%d.%d.%d", version.major, version.minor, version.patch);
        ImGui::BulletText("ImGui 1.85");
        ImGui::BulletText("MathGeoLib 1.5");
        ImGui::BulletText("OpenGL");
        ImGui::SameLine();
        ImGui::Text("%s", glGetString(GL_VERSION));
        ImGui::Text("\nLicense:");
        ImGui::Text("MIT License");
        ImGui::Text("\nCopyright (c) 2021 Mario Hernandez & Oriol Valverde");
        ImGui::Text("\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files (the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions:");
        ImGui::Text("\nThe above copyright noticeand this permission notice shall be included in all\ncopies or substantial portions of the Software.");
        ImGui::Text("\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");

        ImGui::End();
    }
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
                ImGui::TextColored({ 255,255,0,1 }, "%s", glGetString(GL_VENDOR));

                ImGui::Text("Brand :");
                ImGui::SameLine();
                ImGui::TextColored({ 255,255,0,1 }, "%s", glGetString(GL_RENDERER));

                
            }




        }
        if (ImGui::Button("Save"))    App->SaveConfigu();

        if (ImGui::Button("Load"))    App->LoadConfigu();
        ImGui::End();

		
    }



    /*if (ImGui::BeginMenu("File"))
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
    */
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::BeginMenu("Quit", "Alt+F4")) {
            return update_status::UPDATE_STOP;
            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Create Empty"))
        {
            App->sceneIntro->CreateObjectInScene("Empty object", App->sceneIntro->bigDaddy, NULL, NULL);
        }
        //
        if (ImGui::BeginMenu("Primitives"))
        {
            if (ImGui::MenuItem("Cube"))
            {
                App->sceneIntro->CreateObjectInScene("Cube", App->sceneIntro->bigDaddy, "Assets/Models/cube.fbx", NULL);
            }
            if (ImGui::MenuItem("Sphere"))
            {
                App->sceneIntro->CreateObjectInScene("Cube", App->sceneIntro->bigDaddy, "Assets/Models/sphere.fbx", NULL);
            }
            if (ImGui::MenuItem("Cylinder"))
            {
                App->sceneIntro->CreateObjectInScene("Cube", App->sceneIntro->bigDaddy, "Assets/Models/cylinder.fbx", NULL);
            }
            if (ImGui::MenuItem("Pyramid"))
            {
                App->sceneIntro->CreateObjectInScene("Cube", App->sceneIntro->bigDaddy, "Assets/Models/pyramid.fbx", NULL);
            }
            ImGui::EndMenu();
        }
        //

        ImGui::EndMenu();
    }
	/// ----- Console -----
	if (ImGui::BeginMenu("Windows"))
	{
		if (ImGui::MenuItem("console"))
		{
			console = !console;
		}
		if (ImGui::MenuItem("hierarchy"))
		{
			hierarchy = !hierarchy;
		}
		if (ImGui::MenuItem("inspector"))
		{
			inspector = !inspector;
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Options"))
    {
        /*if (ImGui::MenuItem("GUI Demo"))
        {
            showGuiDemo = !showGuiDemo;
        }*/
        if (ImGui::MenuItem("Configuration"))
        {
            showConfig = !showConfig;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("Wiki"))
        {
            ShellExecuteA(NULL, "open", "https://github.com/MHF13/HiveMindEngine", NULL, NULL, SW_SHOWNORMAL);
        }
        if (ImGui::MenuItem("Latest Version"))
        {
            ShellExecuteA(NULL, "open", "https://github.com/MHF13/HiveMindEngine/releases", NULL, NULL, SW_SHOWNORMAL);
        }
        if (ImGui::MenuItem("About Us"))
        {
            showAboutWindow = !showAboutWindow;
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
	
	//LALA
	if (console){
		ImGui::Begin("Console", &console);
		for (auto& a : logs)
		{
			ImGui::TextWrapped(a.c_str());
		}
		ImGui::End();
	}
	if (hierarchy)
	{
		ImGui::Begin("Herarchy", &hierarchy);
		if (App->sceneIntro->bigDaddy != nullptr)
		{
			HierarchyList(App->sceneIntro->bigDaddy);
		}
		ImGui::End();
	}
	if (inspector)
	{
		ImGui::Begin("Inspector", &inspector);
		if (App->sceneIntro->bigDaddy != nullptr)
		{
			for (int i = 0; i < App->sceneIntro->bigDaddy->childs.size(); ++i)
			{
				if (App->sceneIntro->bigDaddy->childs.at(i) == selectedH)
				{
                    if (ImGui::CollapsingHeader("Transform"))
                    {
                        TransformC* trans = App->sceneIntro->bigDaddy->childs.at(i)->transform;
                        if (ImGui::DragFloat3("Position", &trans->position[0], 0.1f))trans->updateTransform = true;
                        if (ImGui::DragFloat3("Rotation", &trans->rotEuler[0], 0.1f))trans->updateTransform = true;
                        if (ImGui::DragFloat3("Scale", &trans->scale[0], 0.1f))trans->updateTransform = true;
                        if (ImGui::Button("Reset Transform"))
                            trans->ResetTransform();
                    }
                    if (ImGui::CollapsingHeader("Mesh Component"))
                    {
                        MeshC* mesh = App->sceneIntro->bigDaddy->childs.at(i)->mesh;
                        if (ImGui::Checkbox("Active", &mesh->active)) {}
                    }
                    if (ImGui::CollapsingHeader("Texture Component"))
                    {
                        TextureC* texture = App->sceneIntro->bigDaddy->childs.at(i)->texture;
                        if (ImGui::Checkbox("Active", &texture->active)) {}

                    }
					
				}
			}
		}
		ImGui::End();
	}
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//-----------------------------
    if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
        if (selectedH != nullptr && selectedH != App->sceneIntro->bigDaddy)
            App->camera->CenterToObject(selectedH);
    }

    if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LCTRL)) {
        LOG("Save state");
    }

    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	return true;
}

void ModuleEditor::UpdateInspector(GameObject* go)
{
	if (go->components.size() > 0)
	{
		for (size_t i = 0; i < go->components.size(); i++)
		{
			if (go->components.at(i)->active)
			{
				go->components.at(i)->Draw();
			}
		}
	}
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

void ModuleEditor::HierarchyList(GameObject* list)
{
	ImGuiTreeNodeFlags parent = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | (list->childs.empty() ? ImGuiTreeNodeFlags_Leaf : 0);
	if (list == selectedH)
	{
		parent |= ImGuiTreeNodeFlags_Selected;
	}
	bool openTreenNode = ImGui::TreeNodeEx(list->name, parent);
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
	{
		selectedH = list;
	}
	if (openTreenNode) {
		for (size_t i = 0; i < list->childs.size(); i++)
		{
			HierarchyList(list->childs.at(i));
		};
		ImGui::TreePop();
	}
}
