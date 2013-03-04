#ifndef BUTTON_H
#define BUTTON_H

#include "Node.h"

class MainApp;

class Button : public Node {
public:
    Button(MainApp *app);
    virtual ~Button();

    //Inherited from Node
    virtual void OnMouseDown(HitData hit);
    virtual void OnMouseUp(HitData hit);
    virtual void OnMouseOut();

    void SetUpColor(float r, float g, float b);
    void SetDownColor(float r, float g, float b);

private:
    MainApp *m_pApp;
    glm::vec3 m_upColor;
    glm::vec3 m_downColor;
};

#endif