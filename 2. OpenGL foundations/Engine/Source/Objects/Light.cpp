#include "pch.h"

void Light::DrawInspector(LightType type, int count)
{
	std::string name = "Light [" + std::to_string(count) + std::string("]");
	switch (type)
	{
	case LightType::DIRECTIONAL: name = "Directional " + name;
	case LightType::POINT:		 name = "Point " + name;
	}

	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##color", glm::value_ptr(color));

		if (type == DIRECTIONAL)
		{
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
		}
		else if (type == POINT)
		{
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

			ImGui::Text("Range");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60.0f);
			ImGui::DragFloat(std::string("##range" + name).c_str(), &range);
		}

		ImGui::TreePop();
	}
	ImGui::Separator();
}