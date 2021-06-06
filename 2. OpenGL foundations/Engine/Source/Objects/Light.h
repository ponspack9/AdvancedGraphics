#pragma once

class DirectionalLight
{
public:
    vec3 color;
    vec3 direction;

    u32 localParams_offset;
    u32 localParams_size;

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

    u32 localParams_offset;
    u32 localParams_size;

public:
    PointLight(vec3 color, vec3 position, float radius) : position(position), color(color), radius(radius) {};
    ~PointLight() {};

    void DrawInspector(int count);
};