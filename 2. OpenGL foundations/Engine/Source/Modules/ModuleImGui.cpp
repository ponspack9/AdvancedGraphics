#include "pch.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include <PanelInfo.h>
#include <PanelInspector.h>

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
    /*if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 0.2f;
    }*/

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


    // Panels
    panels.push_back(new PanelInfo());
    panels.push_back(new PanelInspector());
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

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Begin dock space
    /*if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        DockSpace();*/
    

	return true;
}

bool ModuleImGui::Update(float dt)
{
    for (Panel* panel : panels)
    {
        if (panel->IsEnabled())
            panel->Draw();
    }
    // You can handle App->input keyboard/mouse here
    if (App->input.keys[K_ESCAPE] == BUTTON_PRESS)
    {
        return false;
    }
	return true;
}

bool ModuleImGui::PostUpdate(float dt)
{
    /*if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
        ImGui::End();*/

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

	return true;
}


void ModuleImGui::DockSpace() const
{
    // --- Adapt to Window changes like resizing ---
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.7f);

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
    //ImGui::PushStyleVar(ImGuiStyleVar_Alpha,255.0f);

    static bool p_open = true;
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
}