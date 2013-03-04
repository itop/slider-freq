#ifndef MAINAPP_H
#define MAINAPP_H

#include <vector>
#include "Node.h"

class Camera;
class ShaderProgram;
class Renderer;
class SoundGenerator;
class Slider;

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

    void RegisterToDraw(Node *pNode);

private:
    //Returns a ray represented by origin: rOx,rOy,rOz direction: rDx, rDy, rDz based on the mouse coords
    void GetRay(float mouseX, float mouseY, float &rOx, float &rOy, float &rOz, float &rDx, float &rDy, float &rDz);

    //Returns a list of hit nodes based on mouseX and mouseY
    void UpdateHitList(float mouseX, float mouseY);

    float m_winW;
    float m_winH;

    Camera *m_pCamera;
    Node *m_pSceneTree;
    Slider *m_pSlider;
    Renderer *m_pRenderer;
    SoundGenerator *m_pSoundGen;
    std::vector<Node*> m_drawList;
    std::vector<Node::HitData> m_hitList;
};

#endif