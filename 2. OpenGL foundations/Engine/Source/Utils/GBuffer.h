#pragma once

class GBuffer
{
public:
    enum Textures {
        ALBEDO,
        NORMALS,
        POSITION,
        DEPTH,
        NUM_TEXTURES
    };

    GBuffer();
    ~GBuffer();

    bool Init();

public:
    GLuint FBO;
    GLuint textures[NUM_TEXTURES];
    GLuint depth_texture;
};