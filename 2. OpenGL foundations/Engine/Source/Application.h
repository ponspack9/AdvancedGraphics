#pragma once

#include "platform.h"
#include "engine.h"
#include "Module.h"

class Application
{
public:
    Application();
    virtual ~Application();

    bool Init();
    bool Start();
    bool MainUpdate();
    bool CleanUp();
private:

    bool PreUpdate();
    bool Update();
    bool PostUpdate();

public:
    std::vector<Module*> modules;

public:
    // Loop - Maintained by platform
    f32  deltaTime;
    bool isRunning;

    // Input
    Input input;

    // Graphics
    char gpuName[64];
    char openGlVersion[64];

    ivec2 displaySize; // Maintained by platform

    // Engine stuff
    std::vector<Texture>  textures;
    std::vector<Program>  programs;

    // program indices
    u32 texturedGeometryProgramIdx;

    // texture indices
    u32 diceTexIdx;
    u32 whiteTexIdx;
    u32 blackTexIdx;
    u32 normalTexIdx;
    u32 magentaTexIdx;

    // Mode
    Mode mode;

    // Embedded geometry (in-editor simple meshes such as
    // a screen filling quad, a cube, a sphere...)
    GLuint embeddedVertices;
    GLuint embeddedElements;

    // Location of the texture uniform in the textured quad shader
    GLuint programUniformTexture;

    // VAO object to link our screen filling quad with our textured quad shader
    GLuint vao;
};

extern Application* App;