#include "pch.h"

Camera::Camera(vec3 _pos, vec3 _center, float _fov, float _aspect, float _near_plane, float _far_plane)
{
	pos = _pos;
	rot = glm::vec3(0.0f);
	center = _center;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = WORLD_UP;

	fov = _fov;
	aspect = _aspect;
	near_plane = _near_plane;
	far_plane = _far_plane;

	orbit_speed = 1.0f;
	move_speed = 5.0f;
	yaw = 0.0f;
	pitch = 0.0f;

	isOrbital = true;
	isFree = false;

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
		if (ImGui::Checkbox("Orbital Camera", &isOrbital)) isFree = !isOrbital;
		ImGui::SameLine();
		if(ImGui::Checkbox("Free Move Camera", &isFree)) isOrbital = !isFree;

		if (isOrbital)
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
			ImGui::DragFloat("##orbit_speed", &orbit_speed);
		}
		else
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

			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60.0f);
			ImGui::DragFloat("x##camera_rot", &rot.x);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60.0f);
			ImGui::DragFloat("y##camera_rot", &rot.y);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60.0f);
			ImGui::DragFloat("z##camera_rotr", &rot.z);

			ImGui::Text("Speed");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60.0f);
			ImGui::DragFloat("##move_speed", &move_speed);
		}
		ImGui::Separator();
	}
}

void Camera::Move(float dt)
{
	if (isOrbital)
		Orbit(dt);
	else
		WASD_Move(dt);
}

void Camera::Orbit(float dt)
{
	float prev_yaw = yaw;
	float prev_pitch = pitch;

	if (GetKeyState('A') & 0x8000) // Left
	{
		yaw -= orbit_speed * dt;
		if (yaw < 0.0f)
			yaw += TAU;
	}
	else if (GetKeyState('D') & 0x8000) // Right
	{
		yaw += orbit_speed * dt;
		if (yaw > TAU)
			yaw -= TAU;
	}

	if (GetKeyState('W') & 0x8000) // Up
	{
		pitch += orbit_speed * dt;
		if (pitch > 75.0f * PI / 180)
			pitch = 75.0f * PI / 180;
	}
	else if (GetKeyState('S') & 0x8000) // Down
	{
		pitch -= orbit_speed * dt;
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

void Camera::WASD_Move(float dt)
{
	if (GetKeyState('A') & 0x8000) // Left
		pos -= right * move_speed * dt;
	else if (GetKeyState('D') & 0x8000) // Right
		pos += right * move_speed * dt;

	if (GetKeyState('W') & 0x8000) // Forward
		pos += front * move_speed * dt;
	else if (GetKeyState('S') & 0x8000) // Backward
		pos -= front * move_speed * dt;

	if (GetKeyState('Q') & 0x8000) // Up
		pos -= up * move_speed * dt;
	else if (GetKeyState('E') & 0x8000) // Down
		pos += up * move_speed * dt;

	UpdateVectors();
	UpdateViewMatrix();
}

//-------------------------------------------------------------------
void Camera::UpdateViewMatrix()
{
	if (isOrbital)
		viewMatrix = glm::lookAt(pos, center, up);
	else
		viewMatrix = glm::lookAt(pos, pos + front, up);
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

void Camera::UpdateVectors()
{
	glm::vec3 tmp_front;
	tmp_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	tmp_front.y = sin(glm::radians(pitch));
	tmp_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}