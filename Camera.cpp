#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

Camera::Camera()
{
    m_pos = glm::vec3(0.0,0.0,0.0);
    m_target = glm::vec3(0.0,0.0,0.0);
    m_up = glm::vec3(0.0,0.0,0.0);
    m_viewMatrix = glm::mat4(1.0, 0.0, 0.0, 0.0,
                             0.0, 1.0, 0.0, 0.0,
                             0.0, 0.0, 1.0, 0.0,
                             0.0, 0.0, 0.0, 1.0);
    m_projectionMatrix = m_viewMatrix;
}

Camera::~Camera()
{}

void Camera::SetPosition(glm::vec3 pos)
{
    m_pos = pos;
}

void Camera::SetPosition(float x, float y, float z)
{
    m_pos = glm::vec3(x,y,z);
}

void Camera::SetTarget(glm::vec3 target)
{
    m_target = target;
}

void Camera::SetTarget(float x, float y, float z)
{
    m_target = glm::vec3(x,y,z);
}

void Camera::SetUp(glm::vec3 up)
{
    m_up = up;
}

void Camera::SetUp(float x, float y, float z)
{
    m_up = glm::vec3(x,y,z);
}

void Camera::SetFOV(float fov)
{
    m_fov = fov;
}

void Camera::SetDepthRange(float nearPlane, float farPlane)
{
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::SetAspectRatio(float aspect)
{
    m_aspect = aspect;
}

void Camera::Update()
{
    m_viewMatrix = glm::lookAt(m_pos, m_target, m_up);
    m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
}

const float *Camera::GetViewMatrix()
{
    return &m_viewMatrix[0][0];
}

const float *Camera::GetProjectionMatrix()
{
    return &m_projectionMatrix[0][0];
}