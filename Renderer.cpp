#include "Renderer.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Node.h"
#include "Mesh.h"
#include "Modulator.h"

#include <GL/glew.h>
#include <vector>

const char waveVertexShader[] = 
    "attribute vec4 position;                           \n"
    "varying vec2 vTexCoord;                            \n"
    "void main() {                                      \n"
    "   vTexCoord = position.zw;                        \n"
    "   gl_Position = vec4(position.xy,1.0,1);          \n"
    "}                                                  \n";

const char waveFragmentShader[] =
    "uniform sampler1D waveTex;                         \n"
    "uniform float time;                                \n"
    "varying vec2 vTexCoord;                            \n"
    "void main() {                                      \n"
    "   float waveVal = texture1D(waveTex, vTexCoord.x);\n"
    "   waveVal = waveVal*2.0 - 1.0;                    \n"
    "   float myY = vTexCoord.y*4.0 - 2.0;              \n"
    "   float yDiff = max(1.0 - abs(myY - waveVal), 0.0);  \n"
    "   float xDiff = max(0.1 - abs(vTexCoord.x - time), 0.0)/0.1;\n"
    "   gl_FragColor = vec4(0.0,yDiff,0.0,xDiff*yDiff);\n"
    "}                                                  \n";

Renderer::Renderer()
{
    m_pVisualizationShader = new ShaderProgram();
    m_pVisualizationShader->Create();
    m_pVisualizationShader->CompileFragmentShaderFromSource(waveFragmentShader);
    m_pVisualizationShader->CompileVertexShaderFromSource(waveVertexShader);
    m_pVisualizationShader->Link();

    glBindAttribLocation(m_pVisualizationShader->GetHandle(), 0, "position");

    glGenTextures(1, &m_visualizationTexture);
    glGenBuffers(1, &m_visualizationVBO);
    glGenBuffers(1, &m_visualizationIBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_visualizationIBO);
    unsigned int indices[] = {0,1,2,3};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(int), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_visualizationVBO);
    float vbuf[] = {-1.0, -0.1, 0.0, 0.0, //XY UV
                     1.0, -0.1, 1.0, 0.0, 
                    -1.0,  0.1, 0.0, 1.0,
                     1.0,  0.1, 1.0, 1.0};
    glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), vbuf, GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_1D, m_visualizationTexture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}

Renderer::~Renderer()
{
    delete m_pVisualizationShader;

    glDeleteTextures(1, &m_visualizationTexture);
    glDeleteBuffers(1, &m_visualizationVBO);
    glDeleteBuffers(1, &m_visualizationIBO);
}

void Renderer::ResetTime()
{
    m_time = 0;
}

void Renderer::Update(float deltaS)
{
    m_time += deltaS;
}

void Renderer::SetWaveTexture(float frequency, const Modulators::Modulator *pMod)
{
    for(int i = 0; i < WAVE_SAMPLES; i++)
    {
        m_wave[i] = pMod->Modulate(frequency, (float)i/WAVE_SAMPLES);
    }

    glBindTexture(GL_TEXTURE_1D, m_visualizationTexture);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE, WAVE_SAMPLES, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_wave);
}

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
    if(!m_pActiveShader) return;

    m_pActiveShader->Use();

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

    m_pVisualizationShader->Use();

    glUniform1f(glGetUniformLocation(m_pVisualizationShader->GetHandle(), "time"), m_time);
    glUniform1i(glGetUniformLocation(m_pVisualizationShader->GetHandle(), "waveTex"), 0);
    
    glBindTexture(GL_TEXTURE_1D, m_visualizationTexture);
    glBindBuffer(GL_ARRAY_BUFFER, m_visualizationVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_visualizationIBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid*)0);
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

    //Vertex buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, pMesh->GetVBO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->GetIBO());
    glEnableVertexAttribArray(lPosition);
    glVertexAttribPointer(lPosition, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glDrawElements(GL_TRIANGLE_STRIP, pMesh->GetIndexCount(), GL_UNSIGNED_INT, (GLvoid*)0);
}