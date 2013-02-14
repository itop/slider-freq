#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include "MainApp.h"
#include "Camera.h"
#include "Node.h"
#include "ShaderProgram.h"

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
    m_pShader = NULL;
}

void MainApp::Init()
{
    glClearColor(0.0,0.0,0.0,1.0); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    if(!m_pCamera) m_pCamera = new Camera();
    m_pCamera->SetPosition(glm::vec3(0.0, 0.0, -10.0));
    m_pCamera->SetTarget(glm::vec3(0.0, 0.0, 0.0));
    m_pCamera->SetUp(glm::vec3(0.0, 1.0, 0.0));
	m_pCamera->Update();

    if(!m_pShader) m_pShader = new ShaderProgram();
    m_pShader->Create();
    m_pShader->CompileFragmentShaderFromSource(fragmentShader);
    m_pShader->CompileVertexShaderFromSource(vertexShader);
    m_pShader->Link();
    m_pShader->Use();

    //Initialize a square object
    GLuint squareVBO, squareIBO;

    GLfloat squareVertices[8] = {-0.5, -0.5,
                                -0.5,  0.5,
                                 0.5,  0.5,
                                 0.5, -0.5};
    GLuint squareIndices[4] = {0, 1, 3, 2};
    glGenBuffers(1, &squareVBO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, 32, squareVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &squareIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 16, squareIndices, GL_STATIC_DRAW);

    m_pSceneTree = new Node;
    m_pSceneTree->SetPosition(glm::vec3(0.0, 0.0, 0.0));
    m_pSceneTree->SetIBO(squareIBO);
    m_pSceneTree->SetVBO(squareVBO);
}  

void MainApp::OnResize(float width, float height)
{
    glUniformMatrix4fv(m_pShader->GetProjectionMatrixLocation(), 1, GL_FALSE, &glm::perspective(45.0f, m_width/m_height, 0.1f, 100.0f)[0][0]);
    glViewport(0, 0, width, height);
}

void MainApp::Update()
{
    //Clear the existing screen data
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GLuint indices = m_pSceneTree->GetIBO();
    GLuint vertices = m_pSceneTree->GetVBO();

    glUniformMatrix4fv(m_pShader->GetViewMatrixLocation(), 1, GL_FALSE, &m_pCamera->GetMatrix()[0][0]);
    GLfloat identity[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
	glUniformMatrix4fv(m_pShader->GetModelMatrixLocation(), 1, GL_FALSE, &m_pSceneTree->GetTransform()[0][0]);

    glEnableVertexAttribArray(m_pShader->GetPositionLocation());
    glVertexAttribPointer(m_pShader->GetPositionLocation(), 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid*)0);
}

void MainApp::ShutDown()
{
    //Clean up allocated memory
}