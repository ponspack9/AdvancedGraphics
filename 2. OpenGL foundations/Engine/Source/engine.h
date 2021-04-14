//
// engine.h: This file contains the types and functions relative to the engine.
//

#pragma once

#include "platform.h"



typedef glm::vec2  vec2;
typedef glm::vec3  vec3;
typedef glm::vec4  vec4;
typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;

#define VBO GL_ARRAY_BUFFER 
#define EBO GL_ELEMENT_ARRAY_BUFFER

struct Image
{
    void* pixels;
    ivec2 size;
    i32   nchannels;
    i32   stride;
};

struct Texture
{
    GLuint      handle;
    std::string filepath;
};

struct Program
{
    GLuint             handle;
    std::string        filepath;
    std::string        programName;
    u64                lastWriteTimestamp;
};

enum Mode
{
    Mode_TexturedQuad,
    Mode_Count
};

struct App
{
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

struct VertexV3V2
{
    vec3 pos;
    vec2 uv;
};

const VertexV3V2 vertices[] =
{
    { vec3(-0.5, -0.5, 0.0), vec2(0.0,0.0)},
    { vec3( 0.5, -0.5, 0.0), vec2(1.0,0.0)},
    { vec3( 0.5,  0.5, 0.0), vec2(1.0,1.0)},
    { vec3(-0.5,  0.5, 0.0), vec2(0.0,1.0)}
};

const u16 indices[] =
{
    0, 1, 2,
    0, 2, 3
};


void Init(App* app);

void Gui(App* app);

void Update(App* app);

void Render(App* app);


