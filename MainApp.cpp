#include "MainApp.h"
#include "Camera.h"
#include "Node.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SoundGenerator.h"

const char vertexShader[] = 
    "uniform mat4 projection;                           \n"
    "uniform mat4 view;                                 \n"
    "uniform mat4 model;                                \n"
    "attribute vec4 position;                           \n"
    "void main() {                                      \n"
    "    gl_Position = projection*view*model*position;  \n"
    "}                                                  \n";

const char fragmentShader[] =
    "void main() {                                      \n"
    "   gl_FragColor = vec4(1.0,1.0,1.0,1.0);           \n"
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
    m_pCamera->SetPosition(0, 0, -800);
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
    GLuint squareVBO, squareIBO;

    GLfloat squareVertices[8] = {-0.5, -0.5,
                                -0.5,  0.5,
                                 0.5,  0.5,
                                 0.5, -0.5};
    GLuint squareIndices[4] = {0, 1, 3, 2};

    //Will leak if Init is called multiple times
    Mesh *pSquareMesh = new Mesh();
    pSquareMesh->SetVBOFromArray(squareVertices, 8);
    pSquareMesh->SetIBOFromArray(squareIndices, 4);
    
    if(!m_pSceneTree) m_pSceneTree = new Node;
    m_pSceneTree->SetPosition(0,0,0);
    m_pSceneTree->SetScale(100,100,1);
    m_pSceneTree->SetMesh(pSquareMesh);
    m_pSceneTree->UpdateTransform();

    //Initialize sound generator
    if(!m_pSoundGen) m_pSoundGen = new SoundGenerator();

    m_pSoundGen->Format(44100, 16);
    m_pSoundGen->PrepareDevice();
}  

void MainApp::OnMouseDown(float x, float y, MOUSE_BUTTON btn)
{
}

void MainApp::OnMouseUp(float x, float y, MOUSE_BUTTON btn)
{
}

void MainApp::OnMouseMove(float x, float y)
{
    if(m_pSoundGen)
    {
        m_pSoundGen->FillBuffer(x, 1.0);
        m_pSoundGen->Play();
    }
}

void MainApp::OnResize(float width, float height)
{
    float aspect = (float)width/height;

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
    std::vector<Node*> drawList;
    drawList.push_back(m_pSceneTree);

    static int c = 0;
    m_pSceneTree->SetPosition(0, 100 * m_pSoundGen->GetWave()[c+=10 % 44100], 0);
    m_pSceneTree->UpdateTransform();

    m_pRenderer->Render(drawList);
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