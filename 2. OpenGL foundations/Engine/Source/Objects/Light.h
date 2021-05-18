#pragma once

class Light
{
public:
    enum LightType
    {
        DIRECTIONAL,
        POINT
    };

    vec3 position;
    vec3 direction;
    LightType type;
    vec3 color;
    float range;

public:
    Light(LightType type, vec3 color, vec3 dir, vec3 pos = glm::vec3(0.0f), float range = 0.0f) : type(type), position(pos), direction(dir), color(color), range(range) {};
    ~Light() {};

    void DrawInspector(LightType type, int count);
};