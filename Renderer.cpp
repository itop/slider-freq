#include "Renderer.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Node.h"
#include "Mesh.h"

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

    //Get uniform locations
    GLuint handle = m_pActiveShader->GetHandle();
    GLint lViewMatrix   = glGetUniformLocation(handle, "view");
    GLint lProjMatrix   = glGetUniformLocation(handle, "projection");

    //Projection matrix
    glUniformMatrix4fv(lProjMatrix, 1, GL_FALSE, m_pActiveCamera->GetProjectionMatrix());

    //View matrix
    glUniformMatrix4fv(lViewMatrix, 1, GL_FALSE, m_pActiveCamera->GetViewMatrix());

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

    const Mesh *pMesh = pNode->GetMesh();

    //Nothing to render
    if(!pMesh)
    {
        return;
    }

    glm::vec4 color = glm::vec4(pNode->GetColor(), pNode->GetOpacity());

    //Get uniform locations
    GLuint handle = m_pActiveShader->GetHandle();
    GLint lColor        = glGetUniformLocation(handle, "color");
    GLint lModelMatrix  = glGetUniformLocation(handle, "model");

    //Get attribute locations
    GLint lPosition     = glGetAttribLocation(handle, "position");

    /*
        Set uniforms
    */

    //Color
    glUniform4fv(lColor, 1, &color[0]);

    //Model matrix - Mesh transform combined with node transform
    glm::mat4 meshT = pMesh->GetMeshTransform();
    glm::mat4 nodeT = pNode->GetTransformMatrix();
    glm::mat4 model = nodeT*meshT;
    glUniformMatrix4fv(lModelMatrix, 1, GL_FALSE, &model[0][0]);

    //Vertex attributes
    glEnableVertexAttribArray(lPosition);
    glVertexAttribPointer(lPosition, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    //Vertex buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetIBO());

    glDrawElements(GL_TRIANGLE_STRIP, pMesh->GetIndexCount(), GL_UNSIGNED_INT, (GLvoid*)0);
}