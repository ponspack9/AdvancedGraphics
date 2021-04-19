#include "pch.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include <PanelInfo.h>

ModuleImGui* M_Gui = nullptr;

ModuleImGui::ModuleImGui()
{
	this->name = "ModuleImGui";



	LOG_DEBUG("Created module [{0}]", name);
}
ModuleImGui::~ModuleImGui()
{
	LOG_DEBUG("Deleted module [{0}]", name);
}
bool ModuleImGui::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    ImGui::StyleColorsDarcula();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    if (!ImGui_ImplGlfw_InitForOpenGL(M_Window->window, true))
    {
        LOG_ERROR("ImGui_ImplGlfw_InitForOpenGL() failed\n");
        return -1;
    }

    if (!ImGui_ImplOpenGL3_Init())
    {
        LOG_ERROR("Failed to initialize ImGui OpenGL wrapper\n");
        return -1;
    }


    panels.push_back(new PanelInfo());

	return true;
}

bool ModuleImGui::Start()
{
	return true;
}

bool ModuleImGui::PreUpdate(float dt)
{
	return true;
}

bool ModuleImGui::Update(float dt)
{
	return true;
}

bool ModuleImGui::PostUpdate(float dt)
{
	return true;
}


void ModuleImGui::Draw()
{
	for (Panel* panel : panels)
	{
		panel->Draw();
	}
}

bool ModuleImGui::CleanUp()
{
	return true;
}


