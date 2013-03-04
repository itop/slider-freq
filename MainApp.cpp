#include "MainApp.h"
#include "Camera.h"
#include "Node.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SoundGenerator.h"
#include "TransformUtilities.h"

#include "Slider.h"

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
    m_pCamera = NULL;
    m_pSceneTree = NULL;
    m_pRenderer = NULL;
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
    
    if(!m_pSceneTree) m_pSceneTree = new Node;
    if(!m_pSlider) m_pSlider = new Slider;

    //Build the tree structure
    m_pSceneTree->AddChild(m_pSlider);

    //Set up the nodes
    m_pSlider->SetPosition(250,0,0);
    m_pSlider->SetRotation(0.0,-45.0, 0.0);
    m_pSlider->SetScale(1.0, 1.0, 1.0);
    m_pSlider->SetMesh(pSliderMesh);
    m_pSlider->SetKnobMesh(pSliderKnobMesh);
    m_pSlider->SetKnobColor(0.5,0.5,0.2);
    m_pSlider->SetRange(400, 1200);
    m_pSlider->SetColor(0.2,0.2,0.5);
    m_pSlider->SetOpacity(1.0);

    //Update all of the nodes
    m_pSceneTree->Update();

    //Initialize sound generator
    if(!m_pSoundGen) m_pSoundGen = new SoundGenerator();

    m_pSoundGen->Format(44100, 16);
    m_pSoundGen->PrepareDevice();
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
    UpdateHitList(x,y);

    std::vector<Node::HitData>::const_iterator it = m_hitList.begin();
    std::vector<Node::HitData>::const_iterator end = m_hitList.end();
    while(it != end)
    {
        Node::HitData data = (*it);
        data.pNode->OnMouseOver(data);
        ++it;
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

void MainApp::Update()
{   
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