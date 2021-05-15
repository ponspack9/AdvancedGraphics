#include "pch.h"

Camera::Camera(vec3 _pos, vec3 _center, float _radius)
{
	pos = _pos;
	center = _center;
	radius = _radius;

	speed = 0.1f;
	
	forward = normalize(pos - center);
	right = normalize(cross(WORLD_UP, forward));
	up = normalize(cross(forward, right));

	yaw = 0.0f;
	pitch = 0.0f;
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

void Camera::Draw()
{
}

void Camera::Move()
{
	if (GetKeyState('A') & 0x8000)		MoveLeft();
	else if (GetKeyState('D') & 0x8000) MoveRight();

	if (GetKeyState('W') & 0x8000)		MoveUp();
	else if (GetKeyState('S') & 0x8000) MoveDown();
}

void Camera::MoveLeft()
{
	yaw -= speed;
	if (yaw < 0.0f) 
		yaw += 360.0f;

	float radians = (yaw * PI) / 180.0f;
	pos.x = sin(radians) * radius;
	pos.z = cos(radians) * radius;
}

void Camera::MoveRight()
{
	yaw += speed;
	if (yaw > 360.0f)
		yaw -= 360.0f;

	float radians = (yaw * PI) / 180.0f;
	pos.x = sin(radians) * radius;
	pos.z = cos(radians) * radius;
}

void Camera::MoveUp() 
{
	pitch -= speed;
	if (pitch < 0.0f)
		pitch = 0.0f;

	float radians = (pitch * PI) / 180.0f;
	pos.z = sin(radians) * radius;
	pos.y = cos(radians) * radius;
}

void Camera::MoveDown() 
{
	pitch += speed;
	if (pitch > 75.0f)
		pitch = 75.0f;

	float radians = (pitch * PI) / 180.0f;
	pos.z = sin(radians) * radius;
	pos.y = cos(radians) * radius;
}

void Camera::UpdateVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(forward, WORLD_UP));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, forward));
}