#include "Renderer.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Node.h"

#include <GL/glew.h>
#include <vector>

Renderer::Renderer()
{}

Renderer::~Renderer()
{}

void Renderer::SetViewport(float x, float y, float width, float height)
{
    glViewport(x,y,width,height);
}

void Renderer::SetShader(ShaderProgram *pProgram)
{
    m_pActiveShader = pProgram;
}

void Renderer::SetCamera(Camera *pCamera)
{
    m_pActiveCamera = pCamera;
}

void Renderer::Render(const std::vector<Node*> &nodes)
{
    /*
        Per-frame set-up
    */

    //Projection matrix
    glUniformMatrix4fv(m_pActiveShader->GetProjectionMatrixLocation(), 1, GL_FALSE, m_pActiveCamera->GetProjectionMatrix());

    //View matrix
    glUniformMatrix4fv(m_pActiveShader->GetViewMatrixLocation(), 1, GL_FALSE, m_pActiveCamera->GetViewMatrix());

    std::vector<Node*>::const_iterator it = nodes.cbegin();
    std::vector<Node*>::const_iterator end = nodes.cend();

    while(it != end)
    {
        Render((*it));
        ++it;
    }
}

void Renderer::Render(Node *pNode)
{
    /*
        Per-node set-up
    */

    //Model matrix
    glUniformMatrix4fv(m_pActiveShader->GetModelMatrixLocation(), 1, GL_FALSE, pNode->GetTransform());

    //Vertex attributes
    glEnableVertexAttribArray(m_pActiveShader->GetPositionLocation());
    glVertexAttribPointer(m_pActiveShader->GetPositionLocation(), 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    //Vertex buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, pNode->GetVBO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pNode->GetIBO());

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid*)0);
}