#pragma once
#define WORLD_UP glm::vec3(0.0f, 1.0f, 0.0f)

class Camera
{
public:
    // Orientation
    vec3 pos;
    vec3 center;
    //vec3 up;

    // Perspective
    float fov;			//view angle in degrees (1/zoom)
    float aspect;		//aspect ratio (width/height)
    float near_plane;	//near plane
    float far_plane;	//far plane

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    float radius;
    float speed;

    float yaw; //horizontal
    float pitch; //vertical

public:
    Camera(vec3 pos, vec3 center, float radius, float fov, float aspect, float near_plane, float far_plane);
    ~Camera();

    void DrawInspector();
    void Move();

    void UpdateViewMatrix();
    void UpdateProjMatrix();
    glm::mat4 GetViewProjectionMatrix();
};