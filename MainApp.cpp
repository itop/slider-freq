#include "MainApp.h"
#include "Camera.h"
#include "Node.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SoundGenerator.h"
#include "TransformUtilities.h"
#include "Keyboard.h"

#include "Slider.h"
#include "Modulator.h"

#include "Button.h"

#include <vector>
#include <map>

#include "Notes.h"

//Uncomment this to see the FPS in the console output
//#define PRINT_FPS

#ifdef PRINT_FPS
#include <stdio.h>
#endif

const char vertexShader[] = 
    "uniform mat4 projection;                           \n"
    "uniform mat4 view;                                 \n"
    "uniform mat4 model;                                \n"
    "attribute vec4 position;                           \n"
    "void main() {                                      \n"
    "    gl_Position = projection*view*model*position;  \n"
    "}                                                  \n";

const char fragmentShader[] =
    "uniform vec4 color;                                \n"
    "void main() {                                      \n"
    "   gl_FragColor = color;                           \n"
    "}                                                  \n";

MainApp::MainApp()
{
    m_bPlaying = false;
    m_pGuitarButton = NULL;
    m_pPianoButton = NULL;
    m_pModulator = Modulators::GetPiano();
    m_pCamera = NULL;
    m_pSceneTree = NULL;
    m_pRenderer = NULL;
    m_fVolume = 0.0;
    m_fFrequency = 0.0;
}

MainApp::~MainApp()
{
}

void MainApp::Init()
{
    if(!m_pCamera) m_pCamera = new Camera();
    m_pCamera->SetFOV(45);
    m_pCamera->SetDepthRange(0.1, 1000.0);
    m_pCamera->SetAspectRatio(1.0);
    m_pCamera->SetPosition(0, 0, 800);
    m_pCamera->SetTarget(0, 0, 0);
    m_pCamera->SetUp(0, 1, 0);
	m_pCamera->Update();

    //Will leak if Init is called multiple times
    ShaderProgram *pShader = new ShaderProgram();
    pShader->Create();
    pShader->CompileFragmentShaderFromSource(fragmentShader);
    pShader->CompileVertexShaderFromSource(vertexShader);
    pShader->Link();
    pShader->Use();

    if(!m_pRenderer) m_pRenderer = new Renderer();
    m_pRenderer->SetCamera(m_pCamera);
    m_pRenderer->SetShader(pShader);

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
    pSliderKnobMesh->SetVBOFromArray(cubeVertices, 24);
    pSliderKnobMesh->SetIBOFromArray(cubeIndices, 26);

    scaleMatrix[0]  = 80.0;
    scaleMatrix[5]  = 40.0;
    scaleMatrix[10] = 10.0;

    pSliderKnobMesh->SetMeshTransform(scaleMatrix);
    pSliderKnobMesh->SetAABB(AABB(0.0, 0.0, 0.0, 80.0, 40.0, 10.0));

    /*
        Make the button mesh
    */
    Mesh *pButtonMesh = new Mesh();
    pButtonMesh->SetVBOFromArray(cubeVertices, 24);
    pButtonMesh->SetIBOFromArray(cubeIndices, 26);
    
    scaleMatrix[0]  = 80.0;
    scaleMatrix[5]  = 60.0;
    scaleMatrix[10] = 10.0;

    pButtonMesh->SetMeshTransform(scaleMatrix);
    pButtonMesh->SetAABB(AABB(0.0, 0.0, 0.0, 80.0, 60.0, 10.0));

    /*
        Make the keyboard button mesh
    */
    Mesh *pKeyboardButtonMesh = new Mesh();
    pKeyboardButtonMesh->SetVBOFromArray(cubeVertices, 24);
    pKeyboardButtonMesh->SetIBOFromArray(cubeIndices, 26);

    scaleMatrix[0] = 50.0;
    scaleMatrix[5] = 150.0;
    scaleMatrix[10] = 10.0;

    pKeyboardButtonMesh->SetMeshTransform(scaleMatrix);
    pKeyboardButtonMesh->SetAABB(AABB(0.0, 0.0, 0.0, 50.0, 150.0, 10.0));
    
    if(!m_pSceneTree)    m_pSceneTree    = new Node;
    if(!m_pFreqSlider)   m_pFreqSlider   = new Slider(this);
    if(!m_pVolSlider)    m_pVolSlider    = new Slider(this);
    if(!m_pPianoButton)  m_pPianoButton  = new Button(this);
    if(!m_pGuitarButton) m_pGuitarButton = new Button(this);

    //Keyboard requires some additional set-up
    float pNotes[] = { Notes::A4, Notes::AsBb4, Notes::B4, Notes::C4, Notes::CsDb4, Notes::D4, Notes::DsEb4 };
    Keyboard::KeyboardData keyData;
    keyData.numKeys = 7;
    keyData.pKeyMesh = pKeyboardButtonMesh;
    keyData.pNotes = pNotes;
    if(!m_pKeyboard)     m_pKeyboard     = new Keyboard(this, keyData);

    //Button container
    Node *pButtons = new Node;

    //Build the tree structure
    m_pSceneTree->AddChild(m_pFreqSlider);
    m_pSceneTree->AddChild(m_pVolSlider);
    m_pSceneTree->AddChild(pButtons);
        pButtons->AddChild(m_pPianoButton);
        pButtons->AddChild(m_pGuitarButton);
    m_pSceneTree->AddChild(m_pKeyboard);

    //Set up the nodes
    //Frequency slider
    m_pFreqSlider->SetPosition(250,0,0);
    m_pFreqSlider->SetRotation(0.0,-45.0, 0.0);
    m_pFreqSlider->SetMesh(pSliderMesh);
    m_pFreqSlider->SetKnobMesh(pSliderKnobMesh);
    m_pFreqSlider->SetKnobColor(0.5,0.8,0.5);
    m_pFreqSlider->SetRange(440, 1760);
    m_pFreqSlider->SetColor(0.25,0.4,0.25);
    m_pFreqSlider->SetOpacity(0.6);

    //Volume slider
    m_pVolSlider->SetPosition(-250,0,0);
    m_pVolSlider->SetRotation(0.0,45.0, 0.0);
    m_pVolSlider->SetMesh(pSliderMesh);
    m_pVolSlider->SetKnobMesh(pSliderKnobMesh);
    m_pVolSlider->SetKnobColor(0.6,0.6,0.8);
    m_pVolSlider->SetRange(0, 1);
    m_pVolSlider->SetColor(0.3,0.3,0.4);
    m_pVolSlider->SetOpacity(0.8);
    m_pVolSlider->SetNormalizedValue(0.5);

    //Button container
    pButtons->SetPosition(0.0, 200.0, 0.0);

    //Piano button
    m_pPianoButton->SetPosition(-100, 0.0, 0.0);
    m_pPianoButton->SetMesh(pButtonMesh);
    m_pPianoButton->SetUpColor(1.0,0.6,0.6);
    m_pPianoButton->SetDownColor(0.6,1.0,1.0);

    //Guitar button
    m_pGuitarButton->SetPosition( 100, 0.0, 0.0);
    m_pGuitarButton->SetMesh(pButtonMesh);
    m_pGuitarButton->SetUpColor(0.6,0.6,1.0);
    m_pGuitarButton->SetDownColor(1.0,1.0,0.6);

    //Keyboard
    m_pKeyboard->SetPosition(0, -200, 50);
    m_pKeyboard->SetRotation(-80,0,0);

    //Update all of the nodes
    m_pSceneTree->Update();

    //Initialize sound generator
    if(!m_pSoundGen) m_pSoundGen = new SoundGenerator();

    m_pSoundGen->Format(44100, 16);
    m_pSoundGen->PrepareDevice();
}  

void MainApp::OnPlayComplete()
{
    m_bPlaying = false;

    std::vector<SoundHandler*>::const_iterator it = m_soundHandlers.begin();
    std::vector<SoundHandler*>::const_iterator end = m_soundHandlers.end();

    while(it != end)
    {
        (*it)->OnSoundFinished(m_fFrequency);
        ++it;
    }
}

void MainApp::GetRay(float mouseX, float mouseY, float &rOx, float &rOy, float &rOz, float &rDx, float &rDy, float &rDz)
{
    if(!m_pCamera) return;

    float rayStartWX, rayStartWY, rayStartWZ;
    ScreenToWorld(mouseX, mouseY, 0, m_pCamera, m_winW, m_winH, rayStartWX, rayStartWY, rayStartWZ);

    float rayEndWX, rayEndWY, rayEndWZ;
    ScreenToWorld(mouseX, mouseY, 1, m_pCamera, m_winW, m_winH, rayEndWX, rayEndWY, rayEndWZ);

    //Normalize the ray direction
    float rayDirX, rayDirY, rayDirZ;
    rayDirX = rayEndWX - rayStartWX;
    rayDirY = rayEndWY - rayStartWY;
    rayDirZ = rayEndWZ - rayStartWZ;

    float rayMagnitude = sqrt(rayDirX*rayDirX + rayDirY*rayDirY + rayDirZ*rayDirZ);
    rayDirX /= rayMagnitude;
    rayDirY /= rayMagnitude;
    rayDirZ /= rayMagnitude;

    rOx = rayStartWX;
    rOy = rayStartWY;
    rOz = rayStartWZ;
    rDx = rayDirX;
    rDy = rayDirY;
    rDz = rayDirZ;
}

void MainApp::OnKeyboardNotePlayed(Keyboard *pKeyboard, const float &note)
{
    m_fFrequency = note;
    GenerateSound();
}

void MainApp::OnButtonPressed(Button *pButton)
{
    if(pButton == m_pPianoButton)
    {
        m_pModulator = Modulators::GetPiano();
    }

    if(pButton == m_pGuitarButton)
    {
        m_pModulator = Modulators::GetGuitar();
    }

    GenerateSound();
}

void MainApp::UpdateHitList(float mouseX, float mouseY)
{
    if(!m_pSceneTree) return;

    m_hitList.clear();

    float rx, ry, rz, rdx, rdy, rdz;
    GetRay(mouseX,m_winH - mouseY,rx,ry,rz,rdx,rdy,rdz);
    
    m_pSceneTree->GetHitList(rx, ry, rz, rdx, rdy, rdz, m_hitList);
}

void MainApp::OnMouseDown(float x, float y, MOUSE_BUTTON btn)
{
    UpdateHitList(x,y);

    std::vector<Node::HitData>::const_iterator it = m_hitList.begin();
    std::vector<Node::HitData>::const_iterator end = m_hitList.end();
    while(it != end)
    {
        Node::HitData data = (*it);
        data.pNode->OnMouseDown(data);
        ++it;
    }
}

void MainApp::GenerateSound()
{
    if(!m_pSoundGen) return;

    if(m_bPlaying) OnPlayComplete(); //We are interrupting the current sound
    m_bPlaying = true;

    m_pSoundGen->Stop();
    m_pSoundGen->FillBuffer(m_fFrequency, m_fVolume, m_pModulator);
    m_pSoundGen->Play();
}

void MainApp::OnSliderReleased(Slider *pSlider)
{
    if(pSlider == m_pFreqSlider)
    {
        m_fFrequency = pSlider->GetValue();
    }
    else if (pSlider == m_pVolSlider)
    {
        m_fVolume = pSlider->GetValue();
    }

    GenerateSound();
}

void MainApp::OnMouseUp(float x, float y, MOUSE_BUTTON btn)
{
    UpdateHitList(x,y);

    std::vector<Node::HitData>::const_iterator it = m_hitList.begin();
    std::vector<Node::HitData>::const_iterator end = m_hitList.end();
    while(it != end)
    {
        Node::HitData data = (*it);
        data.pNode->OnMouseUp(data);
        ++it;
    }
}

void MainApp::OnMouseMove(float x, float y)
{
    //Make a local copy of the list to compare ins and outs
    std::vector<Node::HitData> old = m_hitList;
    UpdateHitList(x,y);

    std::vector<Node::HitData>::const_iterator oit = old.begin();
    std::vector<Node::HitData>::const_iterator oend = old.end();
    std::vector<Node::HitData>::const_iterator it = m_hitList.begin();
    std::vector<Node::HitData>::const_iterator end = m_hitList.end();

    //Initializes to zero
    struct zeroInt
    {
        zeroInt() : val(0) {}
        int val;
    };

    std::map<Node*,zeroInt> refs;
    while(oit != oend)
    {
        refs[(*oit).pNode].val -= 1;
        ++oit;
    }

    it = m_hitList.begin();
    while(it != end)
    {
        Node::HitData data = (*it);
        refs[data.pNode].val += 1;
        data.pNode->OnMouseOver(data);
        ++it;
    }

    std::map<Node*,zeroInt>::const_iterator mit = refs.begin();
    std::map<Node*,zeroInt>::const_iterator mend = refs.end();

    while(mit != mend)
    {
        Node *pNode = (*mit).first;
        int val = (*mit).second.val;
        if(val < 0)
        {
            pNode->OnMouseOut();
        }
        else if(val > 0)
        {
            pNode->OnMouseIn();
        }
        ++mit;
    }
}

void MainApp::OnResize(float width, float height)
{
    m_winW = width;
    m_winH = height;

    float aspect = m_winW/m_winH;

    if(m_pRenderer)
    {
        m_pRenderer->SetViewport(0,0,width,height);
    }

    if(m_pCamera)
    {
        m_pCamera->SetAspectRatio(aspect);
        m_pCamera->Update();
    }
}

void MainApp::Update(float frameTimeS)
{   
#ifdef PRINT_FPS
    if(frameTimeS)
    {
        printf("Frame time: %1.5fms | FPS: %3.2f \n", frameTimeS*1000.0, 1.0/frameTimeS);
    }
#endif

    m_drawList.clear();
    m_pSceneTree->Update();
    m_pSceneTree->RegisterIntoList(this);
    m_pRenderer->Render(m_drawList);
}

void MainApp::RegisterToDraw(Node *pNode)
{
    m_drawList.push_back(pNode);
}

void MainApp::ShutDown()
{
    //Clean up allocated memory
    if(m_pCamera) delete m_pCamera;
    if(m_pSceneTree) delete m_pSceneTree;
    if(m_pRenderer) delete m_pRenderer;
    if(m_pSoundGen)
    {
        m_pSoundGen->Stop();
        m_pSoundGen->CloseDevice();
        delete m_pSoundGen;
    }

    m_pCamera = NULL;
    m_pSceneTree = NULL;
    m_pRenderer = NULL;
}