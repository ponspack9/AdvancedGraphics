#pragma once
#define WORLD_UP glm::vec3(0.0f, 1.0f, 0.0f)
#define FOV 60
#define NEAR_PLANE 0.1
#define FAR_PLANE 10000

class Camera
{
public:
    glm::vec3 pos; // Camera position in 3D
    glm::vec3 center; // Point that the camera is looking at
    glm::vec3 up; // Orientation of the camera

    // Perspective
    float fov;			//view angle in degrees (1/zoom)
    float aspect;		//aspect ratio (width/height)
    float near_plane;	//near plane
    float far_plane;	//far plane

    glm::mat4x4 viewMatrix;
    glm::mat4 projMatrix;

    float radius;
    float speed;
    float yaw; //horizontal
    float pitch; //vertical

public:
    Camera(vec3 _pos, vec3 _center, float _radius, float _fov, float _aspect, float _near_plane, float _far_plane);
    ~Camera();

    void DrawInspector();
    void Move();

    void UpdateViewMatrix();
    void UpdateProjMatrix();
    glm::mat4 GetViewProjectionMatrix();

    // Camera forward is -z
    glm::vec3 GetViewDir() const { return -glm::transpose(viewMatrix)[2]; }
    glm::vec3 GetRightVector() const { return glm::transpose(viewMatrix)[0]; }
};
