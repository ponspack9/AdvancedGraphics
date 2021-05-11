#pragma once
#include <Components/Component.h>

class Camera : public Component
{
public:
    vec3 position;
    vec3 rotation;

    Camera();
    ~Camera();

    void DrawInspector() override;
    void Draw();

    void Move();
};