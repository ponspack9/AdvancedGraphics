#include "pch.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include <engine.h>
#include <Application.h>
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
    // Tell GLFW to call platform callbacks
    glfwPollEvents();

    // ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //Gui();
    

	return true;
}

bool ModuleImGui::Update(float dt)
{
    for (Panel* panel : panels)
    {
        panel->Draw();
    }
	return true;
}

bool ModuleImGui::PostUpdate(float dt)
{
    ImGui::Render();

    // Clear input state if required by ImGui
    if (ImGui::GetIO().WantCaptureKeyboard)
        for (u32 i = 0; i < KEY_COUNT; ++i)
            App->input.keys[i] = BUTTON_IDLE;

    if (ImGui::GetIO().WantCaptureMouse)
        for (u32 i = 0; i < MOUSE_BUTTON_COUNT; ++i)
            App->input.mouseButtons[i] = BUTTON_IDLE;

    // Transition input key/button states
    if (!ImGui::GetIO().WantCaptureKeyboard)
        for (u32 i = 0; i < KEY_COUNT; ++i)
            if (App->input.keys[i] == BUTTON_PRESS)   App->input.keys[i] = BUTTON_PRESSED;
            else if (App->input.keys[i] == BUTTON_RELEASE) App->input.keys[i] = BUTTON_IDLE;

    if (!ImGui::GetIO().WantCaptureMouse)
        for (u32 i = 0; i < MOUSE_BUTTON_COUNT; ++i)
            if (App->input.mouseButtons[i] == BUTTON_PRESS)   App->input.mouseButtons[i] = BUTTON_PRESSED;
            else if (App->input.mouseButtons[i] == BUTTON_RELEASE) App->input.mouseButtons[i] = BUTTON_IDLE;

    App->input.mouseDelta = glm::vec2(0.0f, 0.0f);
    // ImGui Render
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    // Present image on screen
    glfwSwapBuffers(M_Window->window);

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


