#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Notes.h"
#include "Node.h"
#include "Handlers.h"

class Button;
class Mesh;

class Keyboard : public ButtonHandler, public Node {

public:
    struct KeyboardData
    {
        Mesh *pKeyMesh;
        unsigned int numKeys;
        const float *pNotes;
    };

    //You are NOT responsible for freeing the memory in keys, Keyboard will take ownership
    Keyboard(KeyboardHandler *pHandler, KeyboardData &keys);
    ~Keyboard();

    Button *GetButtons();

    virtual void OnButtonPressed(Button *pButton);

private:
    Button **m_ppButtons;
    KeyboardHandler *m_pHandler;
    KeyboardData m_data;
};

#endif