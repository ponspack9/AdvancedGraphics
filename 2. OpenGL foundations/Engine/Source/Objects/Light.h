#pragma once

class Light
{
public:
	enum class Type
	{
		Directional,
		Point
	};

    u32 type;
    vec3 color;
    vec3 direction;
    vec3 position;

public:
    Light(Light::Type type, vec3 color, vec3 direction, vec3 position);
    ~Light();

    void DrawInspector();
};