#pragma once

class Light
{
public:
	enum class Type
	{
		Directional,
		Point
	};

public:

    u32 type;
    vec3 color;
    vec3 direction;
    vec3 position;

    Light(Light::Type type, vec3 color, vec3 direction, vec3 position);
    ~Light();

    void DrawInspector();
    void Draw();
};