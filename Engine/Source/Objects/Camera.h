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

    // Vectors
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 right;

    // Perspective
    float fov;			//view angle in degrees (1/zoom)
    float aspect;		//aspect ratio (width/height)
    float near_plane;	//near plane
    float far_plane;	//far plane

    // Matrices
    glm::mat4x4 viewMatrix;
    glm::mat4 projMatrix;

    // Angles
    float yaw; //horizontal
    float pitch; //vertical

    float orbit_speed;
    float move_speed;
    float sensitivity;
    
    bool isOrbital;
    bool isFree;

public:
    Camera(vec3 _pos, vec3 _center, float _fov, float _aspect, float _near_plane, float _far_plane);
    ~Camera();

    void DrawInspector();
    void Move(float dt);

    void Orbit(float dt);
    void WASD_Move(float dt);

    void MouseMovement(float xoffset, float yoffset);
    void MouseScroll(float yoffset);

    void UpdateVectors(); // Free move camera
    void UpdateViewMatrix();
    void UpdateProjMatrix();
    glm::mat4 GetViewProjectionMatrix();

    // Camera forward is -z
    glm::vec3 GetViewDir() const { return -glm::transpose(viewMatrix)[2]; }
    glm::vec3 GetRightVector() const { return glm::transpose(viewMatrix)[0]; }
};
