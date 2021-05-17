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
    vec3 position;
    float intensity;

    float att_const;
    float att_linear;
    float att_quad;

public:
    PointLight(vec3 color, vec3 position, float intensity = 1.0f, float att_const = 1.0f, float att_linear = 0.09f, float att_quad = 0.03f);
    ~PointLight();

    void DrawInspector(int i);
};
