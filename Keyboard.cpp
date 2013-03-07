#include "Keyboard.h"
#include "Button.h"
#include "Handlers.h"
#include "Mesh.h"

Keyboard::Keyboard(KeyboardHandler *pHandler, KeyboardData &keys)
{
    m_pHandler = pHandler;
    m_data.numKeys = keys.numKeys;
    m_data.pKeyMesh = keys.pKeyMesh;
    m_data.pNotes = new float[m_data.numKeys];
    memcpy((void*)m_data.pNotes,keys.pNotes, sizeof(float)*keys.numKeys);
    m_ppButtons = new Button*[keys.numKeys];

    float minX, maxX, dontCare;
    keys.pKeyMesh->GetAABB().GetExtents(minX, dontCare, dontCare, maxX, dontCare, dontCare);

    float keyWidth = (maxX - minX) * 1.1; //10% padding
    float totalWidth = keyWidth * (m_data.numKeys-1);

    for(int i = 0; i < m_data.numKeys; i++)
    {
        m_ppButtons[i] = new Button(this);

        //Position the buttons
        float xPosition = (keyWidth * i) - (totalWidth / 2);

        m_ppButtons[i]->SetMesh(keys.pKeyMesh);
        m_ppButtons[i]->SetPosition(xPosition, 0, 0);
        m_ppButtons[i]->SetUpColor(0.8,0.8,0.8);
        m_ppButtons[i]->SetDownColor(0.9,1.0,0.9);
        AddChild(m_ppButtons[i]);
    }
}

void Keyboard::OnButtonPressed(Button *pButton)
{
    for(int i = 0; i < m_data.numKeys; i++)
    {
        if(pButton == m_ppButtons[i])
        {
            m_pHandler->OnKeyboardNotePlayed(this, m_data.pNotes[i]);
        }
    }
}

Keyboard::~Keyboard()
{
    for(int i = 0; i < m_data.numKeys; i++)
    {
        delete m_ppButtons[i];
    }
    delete[] m_ppButtons;
}