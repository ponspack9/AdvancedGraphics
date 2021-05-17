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
	std::string name = "Directional Light [" + std::to_string(i) + std::string("]");
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##color", glm::value_ptr(color));

		ImGui::Text("Direction");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("x##direction" + name).c_str(), &direction.x);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("y##direction" + name).c_str(), &direction.y);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("z##direction" + name).c_str(), &direction.z);

		ImGui::Text("Intensity");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("##intensity" + name).c_str(), &intensity);

		ImGui::TreePop();
	}
	ImGui::Separator();
}

//----------------------------------------------------
PointLight::PointLight(vec3 _color, vec3 _position, float _intensity, float _att_const, float _att_linear, float _att_quad)
{
	color = _color;
	position = _position;
	intensity = _intensity;
	att_const = _att_const;
	att_linear = _att_linear;
	att_quad = _att_quad;
}

PointLight::~PointLight()
{
}

void PointLight::DrawInspector(int i)
{
	std::string name = "Point Light [" + std::to_string(i) + std::string("]");
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##color", glm::value_ptr(color));

		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("x##position" + name).c_str(), &position.x);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("y##position" + name).c_str(), &position.y);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("z##position" + name).c_str(), &position.z);

		ImGui::Text("Intensity");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("##intensity" + name).c_str(), &intensity);

		ImGui::Text("Attenuation");
		ImGui::SetNextItemWidth(55.0f);
		ImGui::DragFloat(std::string("const##att" + name).c_str(), &att_const);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(55.0f);
		ImGui::DragFloat(std::string("linear##att" + name).c_str(), &att_linear);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(55.0f);
		ImGui::DragFloat(std::string("quad##att" + name).c_str(), &att_quad);

		ImGui::TreePop();
	}
	ImGui::Separator();
}
