#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GL/GL.h>

#include <glm/glm.hpp>

#include "AABB.h"

class Mesh {
public:
    Mesh();
    ~Mesh();

    void SetVBO(GLuint vbo, unsigned int count);
    void SetIBO(GLuint ibo, unsigned int count);
    void SetAABB(AABB bbox);

    void SetMeshTransform(const float *mat44);
    const glm::mat4 &GetMeshTransform() const;
    void GetMeshTransform(float *mat44) const;

    void SetVBOFromArray(float *pData, unsigned int count);
    void SetIBOFromArray(unsigned int *pData, unsigned int count);

    GLuint GetVBO() const;
    GLuint GetIBO() const;
    const AABB &GetAABB() const;

    unsigned int GetVertexCount() const;
    unsigned int GetIndexCount() const;

private:
    AABB m_boundingBox;
    GLuint m_vboId;
    GLuint m_iboId;
    unsigned int m_indexCount;
    unsigned int m_vertexCount;
    glm::mat4 m_meshTransform;
};

#endif