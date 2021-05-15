#include "pch.h"

Light::Light(Light::Type _type, vec3 _color, vec3 _direction, vec3 _position)
{
	switch (_type)
	{
	case Light::Type::Directional:	type = 0;
	case Light::Type::Point:		type = 1;
	}

	color = _color;
	direction = _direction;
	position = _position;
}

Light::~Light()
{
}

void Light::DrawInspector()
{
}

void Light::Draw()
{

}