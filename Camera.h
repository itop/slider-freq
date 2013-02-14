#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

class Camera
{
public:
    Camera();
    ~Camera();

    void SetPosition(glm::vec3 pos);
    void SetPosition(float x, float y, float z);

    void SetTarget(glm::vec3 target);
    void SetTarget(float x, float y, float z);

    void SetUp(glm::vec3 up);
    void SetUp(float x, float y, float z);

    void SetFOV(float fov);
    void SetDepthRange(float nearPlane, float farPlane);
    void SetAspectRatio(float aspect);

    const float* GetViewMatrix();
    const float* GetProjectionMatrix();

    void Update();

private:
    float m_fov;
    float m_nearPlane;
    float m_farPlane;
    float m_aspect;

    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::vec3 m_up;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};

#endif