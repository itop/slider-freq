#include "Mesh.h"

#include <GL/glew.h>
#include <GL/GL.h>

Mesh::Mesh()
{
    m_vboId = 0;
    m_iboId = 0;
}

Mesh::~Mesh()
{}

void Mesh::SetVBOFromArray(float *pData, unsigned int count)
{
    if(!m_vboId)
    {
        glGenBuffers(1, &m_vboId);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(float), pData, GL_STATIC_DRAW);
}

void Mesh::SetIBOFromArray(unsigned int *pData, unsigned int count)
{
    if(!m_iboId)
    {
        glGenBuffers(1, &m_iboId);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), pData, GL_STATIC_DRAW);
}

void Mesh::SetIBO(GLuint ibo)
{
    m_iboId = ibo;
}

void Mesh::SetVBO(GLuint vbo)
{
    m_vboId = vbo;
}

GLuint Mesh::GetIBO() const
{
    return m_iboId;
}

GLuint Mesh::GetVBO() const
{
    return m_vboId;
}