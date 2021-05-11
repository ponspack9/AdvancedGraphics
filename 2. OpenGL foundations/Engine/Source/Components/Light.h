#pragma once
#include <Components/Component.h>

class Light : public Component
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

    void DrawInspector() override;
    void Draw();
};