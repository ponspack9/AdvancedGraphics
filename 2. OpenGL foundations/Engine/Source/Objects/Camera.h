#pragma once
#define WORLD_UP (vec3)(0.0f, 1.0f, 0.0f)

class Camera
{
public:
    vec3 pos;
    vec3 center;
    float radius;

    float speed;

    vec3 up;
    vec3 forward;
    vec3 right;

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    float yaw; //horizontal
    float pitch; //vertical

public:
    Camera(vec3 pos, vec3 center, float radius);
    ~Camera();

    void DrawInspector();

    void Draw();
    void Move();
    void UpdateVectors();

    glm::mat4 GetViewMatrix() { return glm::lookAt(pos, pos+ forward, up); }

private:
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();
};