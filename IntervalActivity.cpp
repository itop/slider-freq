#include "Activity.h"
#include "MainApp.h"
#include "Button.h"
#include "Slider.h"
#include "Mesh.h"

#define TOLERANCE 2.0f

IntervalActivity::IntervalActivity(MainApp *pApp, float startNote, float keyNote) : 
    Activity(pApp), 
    m_fStartNote(startNote), 
    m_fKeyNote(keyNote)
{

    //Initialize a square object
    GLfloat cubeVertices[24] =   { 
        -0.5, -0.5,  0.5,  //0
        0.5, -0.5,  0.5,  //1
        -0.5,  0.5,  0.5,  //2
        0.5,  0.5,  0.5,  //3
        0.5,  0.5, -0.5,  //4
        0.5, -0.5, -0.5,  //5
        -0.5, -0.5, -0.5,  //6
        -0.5,  0.5, -0.5,  //7
    };

    GLuint cubeIndices[26] = {0, 1, 2, 3, 3, 1, 4, 5, 5, 6, 4, 7, 7, 6, 2, 0, 0, 6, 1, 5, 5, 2, 2, 3, 7, 4};

    /*
        Make the slider mesh
    */
    Mesh *pSliderMesh = new Mesh();
    pSliderMesh->SetVBOFromArray(cubeVertices, 24);
    pSliderMesh->SetIBOFromArray(cubeIndices, 26);

    float scaleMatrix[] = { 100.0,   0.0,   0.0,   0.0, 
                              0.0, 500.0,   0.0,   0.0,
                              0.0,   0.0,  10.0,   0.0,
                              0.0,   0.0,   0.0,   1.0 };

    pSliderMesh->SetMeshTransform(scaleMatrix);
    pSliderMesh->SetAABB(AABB(0.0, 0.0, 0.0, 100.0, 500.0, 10.0));

    /*
        Make the slider knob mesh
    */
    Mesh *pSliderKnobMesh = new Mesh();
    pSliderKnobMesh->SetVBO(pSliderMesh->GetVBO(), 24); //re-use the cube
    pSliderKnobMesh->SetIBO(pSliderMesh->GetIBO(), 26);

    scaleMatrix[0]  = 80.0;
    scaleMatrix[5]  = 40.0;
    scaleMatrix[10] = 10.0;

    pSliderKnobMesh->SetMeshTransform(scaleMatrix);
    pSliderKnobMesh->SetAABB(AABB(0.0, 0.0, 0.0, 80.0, 40.0, 10.0));

    /*
        Make the button mesh
    */
    Mesh *pButtonMesh = new Mesh();
    pButtonMesh->SetVBO(pSliderMesh->GetVBO(), 24); //re-use the cube
    pButtonMesh->SetIBO(pSliderMesh->GetIBO(), 26);
    
    scaleMatrix[0]  = 80.0;
    scaleMatrix[5]  = 60.0;
    scaleMatrix[10] = 10.0;

    pButtonMesh->SetMeshTransform(scaleMatrix);
    pButtonMesh->SetAABB(AABB(0.0, 0.0, 0.0, 80.0, 60.0, 10.0));

    /*
        Set up the nodes
    */
    m_pFreqSlider = new Slider(this);
    m_pFreqSlider->SetMesh(pSliderMesh);
    m_pFreqSlider->SetColor(0.8,0.8,0.8);
    m_pFreqSlider->SetKnobMesh(pSliderKnobMesh);
    m_pFreqSlider->SetKnobColor(1.0,1.0,1.0);
    m_pFreqSlider->SetRange(m_fStartNote, 2.0*m_fStartNote);

    m_pIntervalButton = new Button(this);
    m_pIntervalButton->SetMesh(pButtonMesh);
    m_pIntervalButton->SetUpColor(0.8,0.8,0.8);
    m_pIntervalButton->SetDownColor(0.8,1.0,0.8);

    /*
        Build and position the tree
    */
    m_pRootNode->AddChild(m_pFreqSlider);
    m_pRootNode->AddChild(m_pIntervalButton);

    m_pFreqSlider->SetPosition(250, 0, 0);
    m_pFreqSlider->SetRotation(0, -60, 0);
    m_pIntervalButton->SetPosition(0, 100, 0);
}

IntervalActivity::~IntervalActivity()
{}

void IntervalActivity::OnActivityStart()
{
    //Set up
    //\todo Animate elements in
}

void IntervalActivity::OnActivityEnd()
{
    //Teardown
    //\todo Animate elements out
}

void IntervalActivity::OnButtonPressed(Button *pButton)
{
    if(pButton == m_pIntervalButton)
    {
        m_pApp->QueueNote(m_fStartNote, true);
        m_pApp->QueueNote(m_fKeyNote);
    }
}

void IntervalActivity::OnSliderReleased(Slider *pSlider)
{
    if(pSlider == m_pFreqSlider)
    {
        m_pApp->QueueNote(m_fStartNote, true);

        float value = pSlider->GetValue();
        m_pApp->QueueNote(value);

        if(abs(value - m_fKeyNote) < TOLERANCE)
        {
            m_pFreqSlider->SetColor(0.0,1.0,0.0);
        }
    }
}