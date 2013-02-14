#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

class Camera
{
public:
    Camera();
    void SetPosition(glm::vec3 pos);
    void SetTarget(glm::vec3 target);
    void SetUp(glm::vec3 up);
    glm::mat4 GetMatrix();
    void Update();

private:
    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::vec3 m_up;
    glm::mat4 m_matrix;
};

#endif