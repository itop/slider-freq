#ifndef __ACTIVITY_H_
#define __ACTIVITY_H_

#include "Handlers.h"
#include "Node.h"

class MainApp;

class Activity 
{
public:
    Activity(MainApp *pApp) : m_pApp(pApp)
    {
        m_pRootNode = new Node();
    }
    virtual ~Activity()
    {
        delete m_pRootNode;
        m_pRootNode = 0;
    }
    virtual Node *GetRootNode() { return m_pRootNode; }
    virtual void OnActivityStart() = 0;
    virtual void OnActivityEnd() = 0;

protected:
    MainApp *m_pApp;
    Node *m_pRootNode;
};

/*
    Interval training activity:

    * We will need a button for the user to push in order to hear these 2 notes (interval).
    * After pressing the button, the 2 notes will play one after the other each note will ring for a second.
    * Display the name of the playing note.
    * Slider has 1 octave range, on release plays first note followed by the slider value
    * If user gets it right, they "win"
*/
class IntervalActivity : public Activity, public ButtonHandler, public SliderHandler 
{
public:
    //Start note is the beginning of the slider, keyNote is the "answer"
    IntervalActivity(MainApp *pApp, float startNote, float keyNote);
    virtual ~IntervalActivity();

    virtual void OnActivityStart();
    virtual void OnActivityEnd();

    virtual void OnSliderReleased(Slider *pSlider);
    virtual void OnButtonPressed(Button *pButton);

protected:
    Slider *m_pFreqSlider;
    Button *m_pIntervalButton;

    float m_fStartNote;
    float m_fKeyNote;
};

#endif