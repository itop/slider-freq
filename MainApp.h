#ifndef MAINAPP_H
#define MAINAPP_H

class Camera;
class Node;
class ShaderProgram;

//Encapsulates the application data
class MainApp 
{
public:
    MainApp();

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
    ShaderProgram *m_pShader;
    float m_width;
    float m_height;
};

#endif