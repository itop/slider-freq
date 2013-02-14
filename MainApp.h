#ifndef MAINAPP_H
#define MAINAPP_H

class Camera;
class Node;
class ShaderProgram;
class Renderer;

//Encapsulates the application data
class MainApp 
{
public:
    MainApp();
    ~MainApp();

    //Initialize the application data
    void Init();

    //Runs once per frame
    void Update();

    //Shut down the application
    void ShutDown();

    void OnResize(float width, float height);

private:
    Camera *m_pCamera;
    Node *m_pSceneTree;
    Renderer *m_pRenderer;
    float m_width;
    float m_height;
};

#endif