#include "pch.h"

void DirectionalLight::DrawInspector()
{
	if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##color", glm::value_ptr(color));

		ImGui::Text("Direction");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("x##direction", &direction.x);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("y##direction", &direction.y);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("z##direction", &direction.z);
	
		ImGui::Separator();
	}
	ImGui::Separator();
}

void PointLight::DrawInspector(int count)
{
	std::string name = "Point Light [" + std::to_string(count) + std::string("]");

	if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		ImGui::Checkbox(std::string("Active##" + name).c_str() , &active);

		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3(std::string("##color" + name).c_str(), glm::value_ptr(color));

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

		ImGui::Text("Radius");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat(std::string("##radius" + name).c_str(), &radius);

		ImGui::Separator();
	}
}