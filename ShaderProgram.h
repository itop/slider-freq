#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include <GL/gl.h> 

class ShaderProgram
{
public:
    ShaderProgram();
    void Create();
    void CompileFragmentShaderFromSource(const char *src);
    void CompileVertexShaderFromSource(const char *src);
    void Link();
    void Use();
    GLint GetModelMatrixLocation();
    GLint GetViewMatrixLocation();
    GLint GetProjectionMatrixLocation();
    GLint GetPositionLocation();

private:
    GLuint m_uVertexShader;
    GLuint m_uFragmentShader;
    GLuint m_uShaderProgram;

    GLint m_modelMatrixLocation;
    GLint m_viewMatrixLocation;
    GLint m_projectionMatrixLocation;
    GLint m_positionLocation;
};

#endif