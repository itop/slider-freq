#ifndef MAINAPP_H
#define MAINAPP_H

#include <vector>
#include "Node.h"
#include "Modulator.h"
#include "Handlers.h"

class Button;
class Camera;
class ShaderProgram;
class Renderer;
class SoundGenerator;
class Slider;

//Encapsulates the application data
class MainApp : public ButtonHandler , public KeyboardHandler, public SliderHandler
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

    void OnSliderReleased(Slider *pSlider);

    //From ButtonHandler
    virtual void OnButtonPressed(Button *pButton);

    //From KeyboardHandler
    virtual void OnKeyboardNotePlayed(Keyboard *pKeyboard, const float &note);

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

    Camera *m_pCamera;
    Node *m_pSceneTree;
    Slider *m_pFreqSlider;
    Slider *m_pVolSlider;
    Keyboard *m_pKeyboard;
    Button *m_pGuitarButton;
    Button *m_pPianoButton;
    Renderer *m_pRenderer;
    SoundGenerator *m_pSoundGen;
    std::vector<Node*> m_drawList;
    std::vector<Node::HitData> m_hitList;
};

#endif