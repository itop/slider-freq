#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <vector>
#include "Modulator.h"

//Forward declaration
class ShaderProgram;
class Camera;
class Node;

#define WAVE_SAMPLES 256

class Renderer {
public:
    Renderer();
    ~Renderer();

    void SetViewport(float x, float y, float width, float height);
    void SetShader(ShaderProgram *pProgram);
    void SetCamera(Camera *pCamera);
    void Render(const std::vector<Node*> &nodes);
    void SetWaveTexture(float frequency, const Modulators::Modulator *pMod);
    void Update(float deltaS);
    void ResetTime();

private:
    void Render(Node *pNode);

    ShaderProgram *m_pActiveShader;
    Camera *m_pActiveCamera;
    ShaderProgram *m_pVisualizationShader;

    float m_time;
    char m_wave[WAVE_SAMPLES];

    GLuint m_visualizationVBO;
    GLuint m_visualizationIBO;
    GLuint m_visualizationTexture;
};

#endif