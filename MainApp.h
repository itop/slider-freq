#ifndef MAINAPP_H
#define MAINAPP_H

#include <vector>
#include <list>
#include "Node.h"
#include "Modulator.h"
#include "Handlers.h"

class Activity;
class Button;
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
    void Update(float frameTimeS);

    //Shut down the application
    void ShutDown();

    //Events
    void OnMouseMove(float x, float y);
    void OnMouseDown(float x, float y, MOUSE_BUTTON btn);
    void OnMouseUp(float x, float y, MOUSE_BUTTON btn);
    void OnResize(float width, float height);

    void RegisterToDraw(Node *pNode);

    void OnPlayComplete();

    //Add a note to the queue, if playImmediately is set to true, the queue will be emptied and replaced with this note
    void QueueNote(float note, bool playImmediately = false);

private:
    //Returns a ray represented by origin: rOx,rOy,rOz direction: rDx, rDy, rDz based on the mouse coords
    void GetRay(float mouseX, float mouseY, float &rOx, float &rOy, float &rOz, float &rDx, float &rDy, float &rDz);

    //Returns a list of hit nodes based on mouseX and mouseY
    void UpdateHitList(float mouseX, float mouseY);

    void GenerateSound();

    float m_winW;
    float m_winH;
    float m_fVolume;
    float m_fFrequency;

    const Modulators::Modulator *m_pModulator;

    Activity *m_pCurrentActivity;
    std::list<Activity*> m_activities;

    bool m_bPlaying;
    Camera *m_pCamera;
    Node *m_pSceneTree;
    Renderer *m_pRenderer;
    SoundGenerator *m_pSoundGen;
    ShaderProgram *m_pNormalShader;
    ShaderProgram *m_pVisualizationShader;
    std::vector<Node*> m_drawList;
    std::vector<Node::HitData> m_hitList;
    std::list<float> m_noteList;
};

#endif