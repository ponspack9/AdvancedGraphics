#pragma once

class GBuffer
{
public:
    enum Textures {
        ALBEDO,
        NORMALS,
        POSITION,
        NUM_TEXTURES
    };

    GBuffer();
    ~GBuffer();

    bool Init();

    void BindForWriting();
    void BindForReading();
    void SetReadBuffer(Textures TextureType);

public:
    GLuint FBO;
    GLuint textures[NUM_TEXTURES];
    GLuint depth_texture;
};
