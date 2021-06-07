#pragma once

class DirectionalLight
{
public:
    vec3 color;
    vec3 direction;

public:
    DirectionalLight(vec3 color, vec3 dir) : direction(dir), color(color) {};
    ~DirectionalLight() {};

    void DrawInspector();
};

class PointLight
{
public:
    vec3 color;
    vec3 position;
    float radius;

    bool active;

public:
    PointLight(vec3 color, vec3 position, float radius) : position(position), color(color), radius(radius), active(false) {};
    ~PointLight() {};

    void DrawInspector(int count);
};