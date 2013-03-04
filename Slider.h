#ifndef SLIDER_H
#define SLIDER_H

#include "Node.h"
class Mesh;
class MainApp;

class Slider : public Node {
public:
    Slider(MainApp *app);
    ~Slider();

    void SetRange(float start, float end);
    float GetValue();

    //Inherited from Node
    virtual void OnMouseDown(HitData hit);
    virtual void OnMouseUp(HitData hit);
    virtual void OnMouseOver(HitData hit);
    virtual void Update();

    void SetKnobMesh(Mesh *pKnob);
    void SetKnobColor(float r, float g, float b);

    //Puts the knob in the appropriate place matching the value
    void PlaceKnob();

private:
    float m_fStart;
    float m_fEnd;
    float m_fNormalizedPosition;
    float m_fKnobHeight;
    float m_fKnobDepth;
    float m_fHeight;
    Node *m_pKnob;
    MainApp *m_pApp;
};

#endif