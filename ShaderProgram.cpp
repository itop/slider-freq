#include "ShaderProgram.h"

#include <GL/glew.h>
#include <GL/gl.h> 

ShaderProgram::ShaderProgram() : 
m_uVertexShader(0),
m_uFragmentShader(0), 
m_uShaderProgram(0)
{
}

static const GLint neg_one = -1;

void ShaderProgram::Create()
{
    //Already have one
    if(m_uShaderProgram) return;
    m_uShaderProgram = glCreateProgram();
}

void ShaderProgram::CompileFragmentShaderFromSource(const char *src)
{
    if(!m_uFragmentShader)
    {
        m_uFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    glShaderSource(m_uFragmentShader, 1, &src, &neg_one);
    glCompileShader(m_uFragmentShader);
    glAttachShader(m_uShaderProgram, m_uFragmentShader);
}

void ShaderProgram::CompileVertexShaderFromSource(const char *src)
{
    if(!m_uVertexShader)
    {
        m_uVertexShader = glCreateShader(GL_VERTEX_SHADER);
    }
    glShaderSource(m_uVertexShader, 1, &src, &neg_one);
    glCompileShader(m_uVertexShader);
    glAttachShader(m_uShaderProgram, m_uVertexShader);
}

void ShaderProgram::Link()
{
    glLinkProgram(m_uShaderProgram);
    m_modelMatrixLocation = glGetUniformLocation(m_uShaderProgram, "model");
    m_projectionMatrixLocation = glGetUniformLocation(m_uShaderProgram, "projection");
    m_viewMatrixLocation = glGetUniformLocation(m_uShaderProgram, "view");
    m_positionLocation = glGetAttribLocation(m_uShaderProgram, "position");
}

GLint ShaderProgram::GetModelMatrixLocation()
{
    return m_modelMatrixLocation;
}

GLint ShaderProgram::GetViewMatrixLocation()
{
    return m_viewMatrixLocation;
}

GLint ShaderProgram::GetProjectionMatrixLocation()
{
    return m_projectionMatrixLocation;
}

GLint ShaderProgram::GetPositionLocation()
{
    return m_positionLocation;
}

void ShaderProgram::Use()
{
    glUseProgram(m_uShaderProgram);
}

