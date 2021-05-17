#include "pch.h"

DirectionalLight::DirectionalLight(vec3 _color, vec3 _direction, float _intensity)
{
	color = _color;
	direction = _direction;
	intensity = _intensity;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::DrawInspector(int i)
{
}

//----------------------------------------------------
PointLight::PointLight(vec3 _color, vec3 _direction, vec3 _position)
{
	color = _color;
	direction = _direction;
	position = _position;
}

PointLight::~PointLight()
{
}

void PointLight::DrawInspector(int i)
{
}
