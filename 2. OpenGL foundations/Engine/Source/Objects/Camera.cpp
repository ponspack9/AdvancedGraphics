#include "pch.h"

Camera::Camera(vec3 _pos, vec3 _center, float _fov, float _aspect, float _near_plane, float _far_plane)
{
	pos = _pos;
	center = _center;
	up = WORLD_UP;

	fov = _fov;
	aspect = _aspect;
	near_plane = _near_plane;
	far_plane = _far_plane;

	speed = 1.0f;
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

		ImGui::Text("Speed");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.0f);
		ImGui::DragFloat("##speed", &speed);

		ImGui::Separator();
	}
}

void Camera::Move(float dt)
{
	float prev_yaw = yaw;
	float prev_pitch = pitch;

	if (GetKeyState('A') & 0x8000) // Left
	{
		yaw -= speed * dt;
		if (yaw < 0.0f)
			yaw += TAU;
	}
	else if (GetKeyState('D') & 0x8000) // Right
	{
		yaw += speed * dt;
		if (yaw > TAU)
			yaw -= TAU;
	}

	if (GetKeyState('W') & 0x8000) // Up
	{
		pitch += speed * dt;
		if (pitch > 75.0f * PI / 180)
			pitch = 75.0f * PI / 180;
	}
	else if (GetKeyState('S') & 0x8000) // Down
	{
		pitch -= speed * dt;
		if (pitch < 0.0f)
			pitch = 0.0f;
	}

	if (GetKeyState('Q') & 0x8000) // Zoom In
	{
		fov -= ZOOM_SPEED * dt;
		if (fov < 1.0f)
			fov = 1.0f;
		UpdateProjMatrix();
	}
	else if (GetKeyState('E') & 0x8000) // Zoom Out
	{
		fov += ZOOM_SPEED * dt;
		if (fov > FOV)
			fov = FOV;
		UpdateProjMatrix();
	}

	float angleX = yaw - prev_yaw;
	float angleY = prev_pitch - pitch;

	// Get the homogenous position of the camera and pivot point
	glm::vec4 position(pos.x, pos.y, pos.z, 1);
	glm::vec4 pivot(center.x, center.y, center.z, 1);

	// step 2: Rotate the camera around the pivot point on the first axis.
	glm::mat4x4 rotationMatrixX(1.0f);
	rotationMatrixX = glm::rotate(rotationMatrixX, angleX, up);
	position = (rotationMatrixX * (position - pivot)) + pivot;

	// step 3: Rotate the camera around the pivot point on the second axis.
	glm::mat4x4 rotationMatrixY(1.0f);
	rotationMatrixY = glm::rotate(rotationMatrixY, angleY, GetRightVector());
	glm::vec3 final_pos = (rotationMatrixY * (position - pivot)) + pivot;

	// Update the camera view (we keep the same center and the same up vector)
	pos = final_pos;
	UpdateViewMatrix();
}

//-------------------------------------------------------------------
void Camera::UpdateViewMatrix()
{
	viewMatrix = glm::lookAt(pos, center, up);
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