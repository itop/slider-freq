#include "Node.h"

#include <GL/glew.h>
#include <GL/gl.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

Node::Node()
{
    m_pos = glm::vec3(0.0, 0.0, 0.0);
    m_rot = glm::vec3(0.0, 0.0, 0.0);
    m_scale = glm::vec3(0.0, 0.0, 0.0);
    m_transform = glm::mat4(1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0);
}

void Node::SetPosition(glm::vec3 pos)
{
    m_pos = pos;
}

void Node::SetRotation(glm::vec3 rot)
{
    m_rot = rot;
}

void Node::SetScale(glm::vec3 scale)
{
    m_scale = scale;
}

glm::mat4 Node::GetTransform()
{
	return m_transform;
}

void Node::UpdateTransform()
{
    glm::mat4 rotation;
    glm::rotate(rotation, m_rot.x, glm::vec3(0.0, 0.0, 1.0));
    glm::rotate(rotation, m_rot.y, glm::vec3(0.0, 1.0, 0.0));
    glm::rotate(rotation, m_rot.z, glm::vec3(1.0, 0.0, 0.0));

    glm::mat4 translation;
    glm::translate(translation, m_pos);

    glm::mat4 scaling;
    glm::scale(scaling, m_scale);

    m_transform = translation*rotation*scaling;
}

void Node::SetIBO(GLuint ibo)
{
    m_uIBOId = ibo;
}

void Node::SetVBO(GLuint vbo)
{
    m_uVBOId = vbo;
}

GLuint Node::GetIBO()
{
    return m_uIBOId;
}

GLuint Node::GetVBO()
{
    return m_uVBOId;
}