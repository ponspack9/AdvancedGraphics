//
// platform.cpp : This file contains the 'main' function. Program execution begins and ends there.
// The platform layer is in charge to create the environment necessary so the engine disposes of what
// it needs in order to create the application (e.g. window, graphics context, I/O, allocators, etc).
//


#include "pch.h"
#include "engine.h"



#define WINDOW_TITLE  "Oscar Pons Gallart - AGP"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define GLOBAL_FRAME_ARENA_SIZE MB(16)
u8* GlobalFrameArenaMemory = NULL;
u32 GlobalFrameArenaHead = 0;

void OnGlfwError(int errorCode, const char *errorMessage)
{
	fprintf(stderr, "glfw failed with error %d: %s\n", errorCode, errorMessage);
}

void OnGlfwMouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
    App* app = (App*)glfwGetWindowUserPointer(window);
    app->input.mouseDelta.x = xpos - app->input.mousePos.x;
    app->input.mouseDelta.y = ypos - app->input.mousePos.y;
    app->input.mousePos.x = xpos;
    app->input.mousePos.y = ypos;
}

void OnGlfwMouseEvent(GLFWwindow* window, int button, int event, int modifiers)
{
    App* app = (App*)glfwGetWindowUserPointer(window);

    switch (event) {
        case GLFW_PRESS:
            switch (button) {
                case GLFW_MOUSE_BUTTON_RIGHT: app->input.mouseButtons[RIGHT] = BUTTON_PRESS; break;
                case GLFW_MOUSE_BUTTON_LEFT:  app->input.mouseButtons[LEFT]  = BUTTON_PRESS; break;
            } break;
        case GLFW_RELEASE:
            switch (button) {
                case GLFW_MOUSE_BUTTON_RIGHT: app->input.mouseButtons[RIGHT] = BUTTON_RELEASE; break;
                case GLFW_MOUSE_BUTTON_LEFT:  app->input.mouseButtons[LEFT]  = BUTTON_RELEASE; break;
            } break;
    }
}

void OnGlfwScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
    // Nothing do yet... maybe zoom in/out in the future?
}

void OnGlfwKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Remap key to our enum values
    switch (key) {
        case GLFW_KEY_SPACE:  key = K_SPACE; break;
        case GLFW_KEY_0: key = K_0; break; case GLFW_KEY_1: key = K_1; break; case GLFW_KEY_2: key = K_2; break;
        case GLFW_KEY_3: key = K_3; break; case GLFW_KEY_4: key = K_4; break; case GLFW_KEY_5: key = K_5; break;
        case GLFW_KEY_6: key = K_6; break; case GLFW_KEY_7: key = K_7; break; case GLFW_KEY_8: key = K_8; break;
        case GLFW_KEY_9: key = K_9; break;
        case GLFW_KEY_A: key = K_A; break; case GLFW_KEY_B: key = K_B; break; case GLFW_KEY_C: key = K_C; break;
        case GLFW_KEY_D: key = K_D; break; case GLFW_KEY_E: key = K_E; break; case GLFW_KEY_F: key = K_F; break;
        case GLFW_KEY_G: key = K_G; break; case GLFW_KEY_H: key = K_H; break; case GLFW_KEY_I: key = K_I; break;
        case GLFW_KEY_J: key = K_J; break; case GLFW_KEY_K: key = K_K; break; case GLFW_KEY_L: key = K_L; break;
        case GLFW_KEY_M: key = K_M; break; case GLFW_KEY_N: key = K_N; break; case GLFW_KEY_O: key = K_O; break;
        case GLFW_KEY_P: key = K_P; break; case GLFW_KEY_Q: key = K_Q; break; case GLFW_KEY_R: key = K_R; break;
        case GLFW_KEY_S: key = K_S; break; case GLFW_KEY_T: key = K_T; break; case GLFW_KEY_U: key = K_U; break;
        case GLFW_KEY_V: key = K_V; break; case GLFW_KEY_W: key = K_W; break; case GLFW_KEY_X: key = K_X; break;
        case GLFW_KEY_Y: key = K_Y; break; case GLFW_KEY_Z: key = K_Z; break;
        case GLFW_KEY_ESCAPE: key = K_ESCAPE; break;
        case GLFW_KEY_ENTER:  key = K_ENTER; break;
    }

    App* app = (App*)glfwGetWindowUserPointer(window);
    switch (action) {
        case GLFW_PRESS:   app->input.keys[key] = BUTTON_PRESS; break;
        case GLFW_RELEASE: app->input.keys[key] = BUTTON_RELEASE; break;
    }
}

void OnGlfwCharEvent(GLFWwindow* window, unsigned int character)
{
    // Nothing to do yet
}

void OnGlfwResizeFramebuffer(GLFWwindow* window, int width, int height)
{
    App* app = (App*)glfwGetWindowUserPointer(window);
    app->displaySize = vec2(width, height);
}

void OnGlfwCloseWindow(GLFWwindow* window)
{
    App* app = (App*)glfwGetWindowUserPointer(window);
    app->isRunning = false;
}

int main()
{
    Log::Init();
    LOG_TRACE("Entering application");

    App app         = {};
    app.deltaTime   = 1.0f/60.0f;
    app.displaySize = ivec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    app.isRunning   = true;

		glfwSetErrorCallback(OnGlfwError);

    if (!glfwInit())
    {
        LOG_ERROR("glfwInit() failed\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window)
    {
        LOG_ERROR("glfwCreateWindow() failed\n");
        return -1;
    }

    glfwSetWindowUserPointer(window, &app);

    glfwSetMouseButtonCallback(window, OnGlfwMouseEvent);
    glfwSetCursorPosCallback(window, OnGlfwMouseMoveEvent);
    glfwSetScrollCallback(window, OnGlfwScrollEvent);
    glfwSetKeyCallback(window, OnGlfwKeyboardEvent);
    glfwSetCharCallback(window, OnGlfwCharEvent);
    glfwSetFramebufferSizeCallback(window, OnGlfwResizeFramebuffer);
    glfwSetWindowCloseCallback(window, OnGlfwCloseWindow);

    glfwMakeContextCurrent(window);

    // Load all OpenGL functions using the glfw loader function
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize OpenGL context\n");
        return -1;
    }

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

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
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
    Init(&app);

    LOG_TRACE("Update loop");

    while (app.isRunning)
    {
        // Tell GLFW to call platform callbacks
        glfwPollEvents();

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Gui(&app);
        ImGui::Render();

        // Clear input state if required by ImGui
        if (ImGui::GetIO().WantCaptureKeyboard)
            for (u32 i = 0; i < KEY_COUNT; ++i)
                app.input.keys[i] = BUTTON_IDLE;

        if (ImGui::GetIO().WantCaptureMouse)
            for (u32 i = 0; i < MOUSE_BUTTON_COUNT; ++i)
                app.input.mouseButtons[i] = BUTTON_IDLE;

        // Update
        Update(&app);

        // Transition input key/button states
        if (!ImGui::GetIO().WantCaptureKeyboard)
            for (u32 i = 0; i < KEY_COUNT; ++i)
                if      (app.input.keys[i] == BUTTON_PRESS)   app.input.keys[i] = BUTTON_PRESSED;
                else if (app.input.keys[i] == BUTTON_RELEASE) app.input.keys[i] = BUTTON_IDLE;

        if (!ImGui::GetIO().WantCaptureMouse)
            for (u32 i = 0; i < MOUSE_BUTTON_COUNT; ++i)
                if      (app.input.mouseButtons[i] == BUTTON_PRESS)   app.input.mouseButtons[i] = BUTTON_PRESSED;
                else if (app.input.mouseButtons[i] == BUTTON_RELEASE) app.input.mouseButtons[i] = BUTTON_IDLE;

        app.input.mouseDelta = glm::vec2(0.0f, 0.0f);

        // Render
        Render(&app);

        // ImGui Render
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // Present image on screen
        glfwSwapBuffers(window);

        // Frame time
        f64 currentFrameTime = glfwGetTime();
        app.deltaTime = (f32)(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

        // Reset frame allocator
        GlobalFrameArenaHead = 0;
    }

    free(GlobalFrameArenaMemory);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}






std::string MakePath(std::string dir, std::string filename)
{

    return dir + "/" + filename;
}

std::string GetDirectoryPart(std::string path)
{
    size_t pos = path.rfind('/');
    if (pos == std::string::npos)
        pos = path.rfind('\\');

    if (pos == std::string::npos)
        return path; // not found

    return path.substr(0, path.length() - pos);
}


std::string ReadTextFile(const char* filepath)
{    
    std::ifstream ifs (filepath);

    if (ifs)
    {
        std::string fileText ((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return  fileText;
    }

    LOG_ERROR("fopen() failed reading file %s", filepath);
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
    if(GetFileAttributesExA(filepath, GetFileExInfoStandard, &Data)) {
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
