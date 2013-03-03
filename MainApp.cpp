#include "MainApp.h"
#include "Camera.h"
#include "Node.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SoundGenerator.h"

#include <GL/glew.h>
#include <GL/gl.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

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
    if(m_pSoundGen)
    {
        m_pSoundGen->FillBuffer(x, 1.0);
        m_pSoundGen->Play();
    }
}

void MainApp::OnMouseUp(float x, float y, MOUSE_BUTTON btn)
{
}

void MainApp::OnMouseMove(float x, float y)
{
    if(m_pSceneTree)
    {
        /*
            To get the square to stay under the mouse, we must first project the origin of our world into the screen in order to get the screen z coordinate
            that corresponds to world z = 0

            Then we take the vector mouseX mouseY originZ and unproject it into the world to get our world coordinates for the mouse. We're using originZ because
            the mouse doesn't provide a z coordinate, and we are deciding that the square should stay on the plane z = 0;
        */

        //Take the projection matrix
        const float *mat = m_pCamera->GetProjectionMatrix();
        glm::mat4 projectionMatrix = glm::mat4(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
        mat = m_pCamera->GetViewMatrix();
        glm::mat4 viewMatrix = glm::mat4(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);

        glm::vec3 origin = glm::vec3(0,0,0);
        glm::vec4 viewport(0.0, 0.0, 800.0, 800.0);
        glm::vec3 screenOrigin = glm::project(origin, viewMatrix, projectionMatrix, viewport);
        
        //Mouse position in screen space
        glm::vec3 mouse = glm::vec3(x, 800 - y, screenOrigin.z);

        glm::vec3 mouseInWorld = glm::unProject(mouse, viewMatrix, projectionMatrix, viewport);
        
        m_pSceneTree->SetPosition(mouseInWorld.x, mouseInWorld.y, mouseInWorld.z);
        m_pSceneTree->UpdateTransform();
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