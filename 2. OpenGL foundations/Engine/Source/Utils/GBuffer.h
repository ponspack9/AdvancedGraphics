#pragma once

class GBuffer
{
public:
    GBuffer();
    ~GBuffer();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();
    void BindForReading();

private:

    GLuint FBO;

    GLuint albedo;
    GLuint normals;
    GLuint position;
    GLuint depth;
};
