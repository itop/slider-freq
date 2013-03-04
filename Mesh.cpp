#include "Mesh.h"

#include <GL/glew.h>
#include <GL/GL.h>

#include <glm/glm.hpp>

Mesh::Mesh()
{
    m_vboId = 0;
    m_iboId = 0;
    m_meshTransform = glm::mat4(1.0, 0.0, 0.0, 0.0,
                                0.0, 1.0, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0,
                                0.0, 0.0, 0.0, 1.0);
}

Mesh::~Mesh()
{}

void Mesh::SetMeshTransform(const float *mat44)
{
    m_meshTransform = glm::mat4(mat44[ 0], mat44[ 1], mat44[ 2], mat44[ 3],
                                mat44[ 4], mat44[ 5], mat44[ 6], mat44[ 7],
                                mat44[ 8], mat44[ 9], mat44[10], mat44[11],
                                mat44[12], mat44[13], mat44[14], mat44[15]);
}

const glm::mat4& Mesh::GetMeshTransform() const
{
    return m_meshTransform;
}

void Mesh::GetMeshTransform(float *mat44) const
{
    mat44[ 0] = m_meshTransform[0][0];
    mat44[ 1] = m_meshTransform[0][1];
    mat44[ 2] = m_meshTransform[0][2];
    mat44[ 3] = m_meshTransform[0][3];
    mat44[ 4] = m_meshTransform[1][0];
    mat44[ 5] = m_meshTransform[1][1];
    mat44[ 6] = m_meshTransform[1][2];
    mat44[ 7] = m_meshTransform[1][3];
    mat44[ 8] = m_meshTransform[2][0];
    mat44[ 9] = m_meshTransform[2][1];
    mat44[10] = m_meshTransform[2][2];
    mat44[11] = m_meshTransform[2][3];
    mat44[12] = m_meshTransform[3][0];
    mat44[13] = m_meshTransform[3][1];
    mat44[14] = m_meshTransform[3][2];
    mat44[15] = m_meshTransform[3][3];
}

void Mesh::SetVBOFromArray(float *pData, unsigned int count)
{
    if(!m_vboId)
    {
        glGenBuffers(1, &m_vboId);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(float), pData, GL_STATIC_DRAW);

    m_vertexCount = count;
}

void Mesh::SetIBOFromArray(unsigned int *pData, unsigned int count)
{
    if(!m_iboId)
    {
        glGenBuffers(1, &m_iboId);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), pData, GL_STATIC_DRAW);

    m_indexCount = count;
}

void Mesh::SetAABB(AABB bbox)
{
    m_boundingBox = bbox;
}

const AABB &Mesh::GetAABB() const
{
    return m_boundingBox;
}

void Mesh::SetIBO(GLuint ibo, unsigned int count)
{
    m_iboId = ibo;
    m_indexCount = count;
}

void Mesh::SetVBO(GLuint vbo, unsigned int count)
{
    m_vboId = vbo;
    m_vertexCount = count;
}

GLuint Mesh::GetIBO() const
{
    return m_iboId;
}

GLuint Mesh::GetVBO() const
{
    return m_vboId;
}

unsigned int Mesh::GetIndexCount() const
{
    return m_indexCount;
}

unsigned int Mesh::GetVertexCount() const
{
    return m_vertexCount;
}