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
#include "Activity.h"
#include "Animations.h"

//Uncomment this to see the FPS in the console output
//#define PRINT_FPS

#ifdef PRINT_FPS
#include <stdio.h>
#endif

const char transformVertexShader[] = 
    "uniform mat4 projection;                           \n"
    "uniform mat4 view;                                 \n"
    "uniform mat4 model;                                \n"
    "attribute vec4 position;                           \n"
    "void main() {                                      \n"
    "   gl_Position = projection*view*model*position;   \n"
    "}                                                  \n";

const char colorFragmentShader[] =
    "uniform vec4 color;                                \n"
    "void main() {                                      \n"
    "   gl_FragColor = color;                           \n"
    "}                                                  \n";


MainApp::MainApp()
{
    m_bPlaying = false;
    m_pCurrentActivity = NULL;
    m_pModulator = Modulators::GetGuitar();
    m_pCamera = NULL;
    m_pSceneTree = NULL;
    m_pRenderer = NULL;
    m_fVolume = 0.0;
    m_fFrequency = 0.0;
}

MainApp::~MainApp()
{
    delete m_pCamera;
    delete m_pCurrentActivity;
    delete m_pRenderer;
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

    if(!m_pNormalShader) m_pNormalShader = new ShaderProgram();
    m_pNormalShader->Create();
    m_pNormalShader->CompileFragmentShaderFromSource(colorFragmentShader);
    m_pNormalShader->CompileVertexShaderFromSource(transformVertexShader);
    m_pNormalShader->Link();
    

    if(!m_pRenderer) m_pRenderer = new Renderer();
    m_pRenderer->SetCamera(m_pCamera);
    m_pRenderer->SetShader(m_pNormalShader);

    //Initialize sound generator
    if(!m_pSoundGen) m_pSoundGen = new SoundGenerator();

    m_pSoundGen->Format(44100, 16);
    m_pSoundGen->PrepareDevice();

    //Interval activity
	m_pCurrentActivity = new IntervalActivity(this, Notes::A4, Notes::D5);
    m_pSceneTree = m_pCurrentActivity->GetRootNode();

    //Start the activity
    m_pCurrentActivity->OnActivityStart();

    m_fVolume = 1;
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

void MainApp::OnPlayComplete()
{
    m_bPlaying = false;

    //Play the next element in the queue
    if(m_noteList.size() > 0)
    {
        m_fFrequency = m_noteList.front();
        m_noteList.pop_front();
    }
    else
    {
        m_fFrequency = 0;
    }
}

void MainApp::QueueNote(float note, bool playImmediately /* = false */)
{
    if(playImmediately)
    {
        m_noteList.clear();
    }
    m_noteList.push_back(note);

    GenerateSound();
}

void MainApp::GenerateSound()
{
    if(m_bPlaying) return; //Already playing
    if(!m_pSoundGen) return;

    if(m_noteList.size() > 0)
        m_fFrequency = m_noteList.front();
    else
        return; //Nothing to play

    m_pSoundGen->Stop();
    m_pSoundGen->FillBuffer(m_fFrequency, m_fVolume, m_pModulator);
    m_pSoundGen->Play();

    m_pRenderer->SetWaveTexture(m_fFrequency, m_pModulator);
    m_pRenderer->ResetTime();

    m_bPlaying = true;
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

    m_pRenderer->SetShader(m_pNormalShader);
    m_pRenderer->Render(m_drawList);
    m_pRenderer->Update(frameTimeS);

    Animator::GetInstance()->Update(frameTimeS);

    GenerateSound();
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