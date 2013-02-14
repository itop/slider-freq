#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

Camera::Camera()
{
    m_pos = glm::vec3(0.0,0.0,0.0);
    m_target = glm::vec3(0.0,0.0,0.0);
    m_up = glm::vec3(0.0,0.0,0.0);
    m_matrix = glm::mat4(1.0, 0.0, 0.0, 0.0,
                         0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0,
                         0.0, 0.0, 0.0, 1.0);
}

void Camera::SetPosition(glm::vec3 pos)
{
    m_pos = pos;
}

void Camera::SetTarget(glm::vec3 target)
{
    m_target = target;
}

void Camera::SetUp(glm::vec3 up)
{
    m_up = up;
}

void Camera::Update()
{
    m_matrix = glm::lookAt(m_pos, m_target, m_up);
}

glm::mat4 Camera::GetMatrix()
{
    return m_matrix;
}