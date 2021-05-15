#include "pch.h"

Camera::Camera(vec3 _pos, vec3 _center, float _radius, float _fov, float _aspect, float _near_plane, float _far_plane)
{
	pos = _pos;
	center = _center;
	radius = _radius;
	//up = 0;

	fov = _fov;
	aspect = _aspect;
	near_plane = _near_plane;
	far_plane = _far_plane;

	speed = 0.1f;
	yaw = 0.0f;
	pitch = 0.0f;

	UpdateViewMatrix();
	UpdateProjMatrix();
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
		ImGui::DragFloat("x##camera_pos", &pos.x);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("y##camera_pos", &pos.y);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("z##camera_pos", &pos.z);

		ImGui::Text("Center");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("x##camera_center", &center.x);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("y##camera_center", &center.y);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("z##camera_center", &center.z);

		ImGui::Text("Radius");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("##radius", &radius);

		ImGui::Text("Speed");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("##speed", &speed);

		//ImGui::Text("Horizontal: %f", yaw);
		//ImGui::Text("Vertical: %f", pitch);
	}
}

void Camera::Move()
{
	if (GetKeyState('A') & 0x8000) // Left
	{
		yaw -= speed;
		if (yaw < 0.0f)
			yaw += 360.0f;

		float radians = (yaw * PI) / 180.0f;
		pos.x = sin(radians) * radius;
		pos.z = cos(radians) * radius;
		UpdateViewMatrix();
	}
	else if (GetKeyState('D') & 0x8000) // Right
	{
		yaw += speed;
		if (yaw > 360.0f)
			yaw -= 360.0f;

		float radians = (yaw * PI) / 180.0f;
		pos.x = sin(radians) * radius;
		pos.z = cos(radians) * radius;
		UpdateViewMatrix();
	}

	if (GetKeyState('W') & 0x8000) // Up
	{
		pitch -= speed;
		if (pitch < 0.0f)
			pitch = 0.0f;

		float radians = (pitch * PI) / 180.0f;
		pos.z = sin(radians) * radius;
		pos.y = cos(radians) * radius;
		UpdateViewMatrix();
	}
	else if (GetKeyState('S') & 0x8000) // Down
	{
		pitch += speed;
		if (pitch > 75.0f)
			pitch = 75.0f;

		float radians = (pitch * PI) / 180.0f;
		pos.z = sin(radians) * radius;
		pos.y = cos(radians) * radius;
		UpdateViewMatrix();
	}
}

//-------------------------------------------------------------------
void Camera::UpdateViewMatrix()
{
	viewMatrix = glm::lookAt(pos, center, WORLD_UP);
}

void Camera::UpdateProjMatrix()
{
	projMatrix = glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
	UpdateViewMatrix();
	UpdateProjMatrix();
	return projMatrix * viewMatrix;
}