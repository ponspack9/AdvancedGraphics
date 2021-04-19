//
// platform.cpp : This file contains the 'main' function. Program execution begins and ends there.
// The platform layer is in charge to create the environment necessary so the engine disposes of what
// it needs in order to create the application (e.g. window, graphics context, I/O, allocators, etc).
//


#include "pch.h"
#include "engine.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"


#define WINDOW_TITLE  "Oscar Pons Gallart - AGP"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define GLOBAL_FRAME_ARENA_SIZE MB(16)
u8* GlobalFrameArenaMemory = NULL;
u32 GlobalFrameArenaHead = 0;

enum main_states {
    MAIN_CREATION,
    MAIN_INIT,
    MAIN_START,
    MAIN_UPDATE,
    MAIN_FINISH,
    MAIN_EXIT
};

int main()
{
    Log::Init();
    LOG_TRACE("Entering application");

    //App app = {};
    App = new Application();
    App->deltaTime = 1.0f / 60.0f;
    App->displaySize = ivec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    App->isRunning = true;

    App->Init();


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

    f64 lastFrameTime = glfwGetTime();

    GlobalFrameArenaMemory = (u8*)malloc(GLOBAL_FRAME_ARENA_SIZE);

    LOG_TRACE("Initializing application");
    Init();

    LOG_TRACE("Update loop");

    while (App->isRunning)
    {
        // Tell GLFW to call platform callbacks
        glfwPollEvents();

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Gui();
        ImGui::Render();

        // Clear input state if required by ImGui
        if (ImGui::GetIO().WantCaptureKeyboard)
            for (u32 i = 0; i < KEY_COUNT; ++i)
                App->input.keys[i] = BUTTON_IDLE;

        if (ImGui::GetIO().WantCaptureMouse)
            for (u32 i = 0; i < MOUSE_BUTTON_COUNT; ++i)
                App->input.mouseButtons[i] = BUTTON_IDLE;

        // Update
        Update();

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

        // Render
        Render();

        // ImGui Render
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // Present image on screen
        glfwSwapBuffers(M_Window->window);

        // Frame time
        f64 currentFrameTime = glfwGetTime();
        App->deltaTime = (f32)(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

        // Reset frame allocator
        GlobalFrameArenaHead = 0;
    }

    free(GlobalFrameArenaMemory);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(M_Window->window);

    glfwTerminate();

    return 0;
}




std::string GetDirectoryPart(std::string path)
{
    size_t pos = path.rfind('/');
    if (pos == std::string::npos)
        pos = path.rfind('\\');

    if (pos != std::string::npos)
        return path.substr(0, path.length() - pos);

    LOG_ERROR("Could not get directory part from {0}", path);
    return path; // not found
}


std::string ReadTextFile(const char* filepath)
{
    std::ifstream ifs(filepath);

    if (ifs)
    {
        std::string fileText((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return  fileText;
    }

    LOG_ERROR("fopen() failed reading file {0}", filepath);
    return "ERROR";
}

u64 GetFileLastWriteTimestamp(const char* filepath)
{
#ifdef _WIN32
    union Filetime2u64 {
        FILETIME filetime;
        u64      u64time;
    } conversor;

    WIN32_FILE_ATTRIBUTE_DATA Data;
    if (GetFileAttributesExA(filepath, GetFileExInfoStandard, &Data)) {
        conversor.filetime = Data.ftLastWriteTime;
        return(conversor.u64time);
    }
#else
    // NOTE: This has not been tested in unix-like systems
    struct stat attrib;
    if (stat(filepath, &attrib) == 0) {
        return attrib.st_mtime;
    }
#endif

    return 0;
}

void LogString(const char* str)
{
#ifdef _WIN32
    OutputDebugStringA(str);
    OutputDebugStringA("\n");
#else
    fprintf(stderr, "%s\n", str);
#endif
}
