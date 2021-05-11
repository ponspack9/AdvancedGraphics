#include "pch.h"

Camera::Camera() : Component(Component::Type::Camera, nullptr)
{

}

Camera::~Camera()
{
}

void Camera::DrawInspector()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("x##camera_pos", &position.x);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("y##camera_pos", &position.y);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("z##camera_pos", &position.z);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("x##camera_rot", &rotation.x);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("y##camera_rot", &rotation.y);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("z##camera_rot", &rotation.z);
	}
}

void Camera::Draw()
{

}

void Camera::Move()
{

}