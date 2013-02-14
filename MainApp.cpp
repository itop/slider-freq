#include "MainApp.h"
#include "Camera.h"
#include "Node.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Mesh.h"

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
    m_pCamera->SetDepthRange(0.1, 100.0);
    m_pCamera->SetAspectRatio(1.0);
    m_pCamera->SetPosition(0, 0, -10);
    m_pCamera->SetTarget(0, 0, 0);
    m_pCamera->SetUp(0, 1, 0);
	m_pCamera->Update();

    ShaderProgram *pShader = new ShaderProgram();
    pShader->Create();
    pShader->CompileFragmentShaderFromSource(fragmentShader);
    pShader->CompileVertexShaderFromSource(vertexShader);
    pShader->Link();
    pShader->Use();

    m_pRenderer = new Renderer();
    m_pRenderer->SetCamera(m_pCamera);
    m_pRenderer->SetShader(pShader);

    //Initialize a square object
    GLuint squareVBO, squareIBO;

    GLfloat squareVertices[8] = {-0.5, -0.5,
                                -0.5,  0.5,
                                 0.5,  0.5,
                                 0.5, -0.5};
    GLuint squareIndices[4] = {0, 1, 3, 2};

    Mesh *pSquareMesh = new Mesh();
    pSquareMesh->SetVBOFromArray(squareVertices, 8);
    pSquareMesh->SetIBOFromArray(squareIndices, 4);

    m_pSceneTree = new Node;
    m_pSceneTree->SetPosition(0,0,0);
    m_pSceneTree->SetMesh(pSquareMesh);
}  

void MainApp::OnResize(float width, float height)
{
    m_pRenderer->SetViewport(0,0,width,height);
    m_pCamera->SetAspectRatio(width/height);
    m_pCamera->Update();
}

void MainApp::Update()
{   
    std::vector<Node*> drawList;
    drawList.push_back(m_pSceneTree);
    m_pRenderer->Render(drawList);
}

void MainApp::ShutDown()
{
    //Clean up allocated memory
    if(m_pCamera) delete m_pCamera;
    if(m_pSceneTree) delete m_pSceneTree;
    if(m_pRenderer) delete m_pRenderer;

    m_pCamera = NULL;
    m_pSceneTree = NULL;
    m_pRenderer = NULL;
}