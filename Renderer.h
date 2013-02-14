#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

//Forward declaration
class ShaderProgram;
class Camera;
class Node;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void SetViewport(float x, float y, float width, float height);
    void SetShader(ShaderProgram *pProgram);
    void SetCamera(Camera *pCamera);
    void Render(const std::vector<Node*> &nodes);

private:
    void Render(Node *pNode);

    ShaderProgram *m_pActiveShader;
    Camera *m_pActiveCamera;
};

#endif