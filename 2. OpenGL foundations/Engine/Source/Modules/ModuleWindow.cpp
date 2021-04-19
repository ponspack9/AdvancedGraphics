#include "pch.h"
#include "ModuleWindow.h"
#include <Application.h>
#include <PanelInfo.h>



void OnGlfwError(int errorCode, const char* errorMessage)
{
    fprintf(stderr, "glfw failed with error %d: %s\n", errorCode, errorMessage);
}

void OnGlfwMouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
    App->input.mouseDelta.x = xpos - App->input.mousePos.x;
    App->input.mouseDelta.y = ypos - App->input.mousePos.y;
    App->input.mousePos.x = xpos;
    App->input.mousePos.y = ypos;
}

void OnGlfwMouseEvent(GLFWwindow* window, int button, int event, int modifiers)
{
    switch (event) {
    case GLFW_PRESS:
        switch (button) {
        case GLFW_MOUSE_BUTTON_RIGHT: App->input.mouseButtons[RIGHT] = BUTTON_PRESS; break;
        case GLFW_MOUSE_BUTTON_LEFT:  App->input.mouseButtons[LEFT] = BUTTON_PRESS; break;
        } break;
    case GLFW_RELEASE:
        switch (button) {
        case GLFW_MOUSE_BUTTON_RIGHT: App->input.mouseButtons[RIGHT] = BUTTON_RELEASE; break;
        case GLFW_MOUSE_BUTTON_LEFT:  App->input.mouseButtons[LEFT] = BUTTON_RELEASE; break;
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

    switch (action) {
    case GLFW_PRESS:   App->input.keys[key] = BUTTON_PRESS; break;
    case GLFW_RELEASE: App->input.keys[key] = BUTTON_RELEASE; break;
    }
}

void OnGlfwCharEvent(GLFWwindow* window, unsigned int character)
{
    // Nothing to do yet
}

void OnGlfwResizeFramebuffer(GLFWwindow* window, int width, int height)
{
    App->displaySize = vec2(width, height);
}

void OnGlfwCloseWindow(GLFWwindow* window)
{
    App->isRunning = false;
}

ModuleWindow* M_Window = nullptr;

ModuleWindow::ModuleWindow()
{
	this->name = "ModuleWindow";

	LOG_DEBUG("Created module [{0}]", name);
}
ModuleWindow::~ModuleWindow()
{
	LOG_DEBUG("Deleted module [{0}]", name);
}
bool ModuleWindow::Init()
{

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

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window)
    {
        LOG_ERROR("glfwCreateWindow() failed\n");
        return -1;
    }

    glfwSetWindowUserPointer(window, &App);

    glfwSetMouseButtonCallback(window, OnGlfwMouseEvent);
    glfwSetCursorPosCallback(window, OnGlfwMouseMoveEvent);
    glfwSetScrollCallback(window, OnGlfwScrollEvent);
    glfwSetKeyCallback(window, OnGlfwKeyboardEvent);
    glfwSetCharCallback(window, OnGlfwCharEvent);
    glfwSetFramebufferSizeCallback(window, OnGlfwResizeFramebuffer);
    glfwSetWindowCloseCallback(window, OnGlfwCloseWindow);

    glfwMakeContextCurrent(window);

    // Load all OpenGL functions using the glfw loader function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize OpenGL context\n");
        return -1;
    }

    lastFrameTime = glfwGetTime();

    App->deltaTime = 1.0f / 60.0f;
    App->displaySize = ivec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    App->isRunning = true;
	return true;
}

bool ModuleWindow::Start()
{
	return true;
}

bool ModuleWindow::PreUpdate(float dt)
{
	return App->isRunning;
}

bool ModuleWindow::Update(float dt)
{

    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Shaded model");
    switch (App->mode)
    {
    case Mode_TexturedQuad:
    {
        // TODO: Draw your textured quad here!
        // - clear the framebuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // - set the viewport
        glViewport(0, 0, App->displaySize.x, App->displaySize.y);
        // - bind the program 
        Program& programTexturedGeometry = App->programs[App->texturedGeometryProgramIdx];
        glUseProgram(programTexturedGeometry.handle);
        glBindVertexArray(App->vao);

        // - set the blending state
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // - bind the texture into unit 0
        glUniform1i(App->programUniformTexture, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, App->textures[App->diceTexIdx].handle);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);
        glUseProgram(0);
        //   (...and make its texture sample from unit 0)
        // - bind the vao
        // - glDrawElements() !!!
    }
    break;

    default:;
    }

    glPopDebugGroup();
	return true;
}

bool ModuleWindow::PostUpdate(float dt)
{

    // Frame time
    f64 currentFrameTime = glfwGetTime();
    App->deltaTime = (f32)(currentFrameTime - lastFrameTime);
    lastFrameTime = currentFrameTime;

	return true;
}



bool ModuleWindow::CleanUp()
{
    glfwDestroyWindow(M_Window->window);

    glfwTerminate();
	return true;
}


