#include "Node.h"

#include <GL/glew.h>
#include <GL/gl.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

Node::Node()
{
    m_bDirty = true;
    m_pParent = NULL;
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

void Node::SetPosition(float x, float y, float z)
{
    m_pos = glm::vec3(x,y,z);
}

void Node::SetRotation(glm::vec3 rot)
{
    m_rot = rot;
}

void Node::SetRotation(float x, float y, float z)
{
    m_rot = glm::vec3(x,y,z);
}

void Node::SetScale(glm::vec3 scale)
{
    m_scale = scale;
}

void Node::SetScale(float x, float y, float z)
{
    m_scale = glm::vec3(x,y,z);
}

const float* Node::GetTransform()
{
	return &m_transform[0][0];
}

void Node::AddChild(Node *pNode)
{
    pNode->m_pParent = this;
    m_children.push_back(pNode);
}

void Node::SetDirty()
{
    m_bDirty = true;
    
    //Dirty the children too
    std::vector<Node*>::const_iterator it = m_children.begin();
    std::vector<Node*>::const_iterator end = m_children.end();
    while(it != end)
    {
        (*it)->SetDirty();
        ++it;
    }
}

void Node::UpdateTransform()
{
    if(!m_bDirty) return;

    glm::mat4 parentTransform;

    if(m_pParent)
    {
        const float *mat = m_pParent->GetTransform();
        parentTransform = glm::mat4(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
    }
    else
    {
        parentTransform = glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    }

    glm::mat4 rotation;
    rotation = glm::rotate(rotation, m_rot.x, glm::vec3(1.0, 0.0, 0.0));
    rotation = glm::rotate(rotation, m_rot.y, glm::vec3(0.0, 1.0, 0.0));
    rotation = glm::rotate(rotation, m_rot.z, glm::vec3(0.0, 0.0, 1.0));

    glm::mat4 translation;
    translation = glm::translate(translation, m_pos);

    glm::mat4 scaling;
    scaling = glm::scale(scaling, m_scale);

    m_transform = parentTransform*translation*rotation*scaling;

    std::vector<Node*>::const_iterator it = m_children.begin();
    std::vector<Node*>::const_iterator end = m_children.end();

    while(it != end)
    {
        (*it)->UpdateTransform();
        ++it;
    }
}

const Mesh *Node::GetMesh()
{
    return m_pMesh;
}

void Node::SetMesh(Mesh *pMesh)
{
    m_pMesh = pMesh;
}