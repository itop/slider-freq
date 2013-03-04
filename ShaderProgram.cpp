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

GLuint ShaderProgram::GetHandle()
{
    return m_uShaderProgram;
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
}

void ShaderProgram::Use()
{
    glUseProgram(m_uShaderProgram);
}

