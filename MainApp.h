#ifndef MAINAPP_H
#define MAINAPP_H

class Camera;
class Node;
class ShaderProgram;
class Renderer;
class SoundGenerator;

//Encapsulates the application data
class MainApp 
{
public:
    enum MOUSE_BUTTON {
        MOUSE_LEFT,
        MOUSE_MIDDLE,
        MOUSE_RIGHT
    };

    MainApp();
    ~MainApp();

    //Initialize the application data
    void Init();

    //Runs once per frame
    void Update();

    //Shut down the application
    void ShutDown();

    //Events
    void OnMouseMove(float x, float y);
    void OnMouseDown(float x, float y, MOUSE_BUTTON btn);
    void OnMouseUp(float x, float y, MOUSE_BUTTON btn);
    void OnResize(float width, float height);

private:
    Camera *m_pCamera;
    Node *m_pSceneTree;
    Renderer *m_pRenderer;
    SoundGenerator *m_pSoundGen;
};

#endif