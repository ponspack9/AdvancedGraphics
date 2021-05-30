#pragma once

class Module;

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

    // program indices
    u32 texturedGeometryProgramIdx;
    u32 dirLightProgramIdx;
    u32 pointLightProgramIdx;

    // texture indices
    u32 diceTexIdx;
    u32 whiteTexIdx;
    u32 blackTexIdx;
    u32 normalTexIdx;
    u32 magentaTexIdx;

    // Mode
    Mode mode;

    // Location of the texture uniform in the textured quad shader
    GLuint programUniformTexture;

};

extern Application* App;