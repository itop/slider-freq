#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GL/GL.h>

class Mesh {
public:
    Mesh();
    ~Mesh();

    void SetVBO(GLuint vbo);
    void SetIBO(GLuint ibo);

    void SetVBOFromArray(float *pData, unsigned int count);
    void SetIBOFromArray(unsigned int *pData, unsigned int count);

    GLuint GetVBO() const;
    GLuint GetIBO() const;

private:
    GLuint m_vboId;
    GLuint m_iboId;
};

#endif