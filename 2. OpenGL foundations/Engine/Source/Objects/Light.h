#pragma once

class DirectionalLight
{
public:
    vec3 color;
    vec3 direction;
    float intensity;

public:
    DirectionalLight(vec3 _color = glm::vec3(1.0f), vec3 _direction = glm::vec3(1.0f), float _intensity = 1.0f);
    ~DirectionalLight();

    void DrawInspector(int i);
};

class PointLight
{
public:
    vec3 color;
    vec3 direction;
    vec3 position;

public:
    PointLight(vec3 color, vec3 direction, vec3 position);
    ~PointLight();

    void DrawInspector(int i);
};
