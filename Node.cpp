#include "Node.h"
#include "MainApp.h"
#include "Mesh.h"

#include <GL/glew.h>
#include <GL/gl.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

Node::Node()
{
    m_bTransformDirty = true;
    m_bBoxDirty = true;
    m_localOpacity = 1.0;
    m_pParent = NULL;
    m_pos = glm::vec3(0.0, 0.0, 0.0);
    m_rot = glm::vec3(0.0, 0.0, 0.0);
    m_scale = glm::vec3(1.0, 1.0, 1.0);
    m_transform = glm::mat4(1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0);
    m_pMesh = NULL;
    m_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
}

void Node::SetPosition(glm::vec3 pos)
{
    m_pos = pos;
    SetTransformDirty();
}

void Node::SetPosition(float x, float y, float z)
{
    m_pos = glm::vec3(x,y,z);
    SetTransformDirty();
    SetBoxDirty();
}

void Node::SetRotation(glm::vec3 rot)
{
    m_rot = rot;
    SetTransformDirty();
    SetBoxDirty();
}

void Node::SetRotation(float x, float y, float z)
{
    m_rot = glm::vec3(x,y,z);
    SetTransformDirty();
    SetBoxDirty();
}

void Node::SetScale(glm::vec3 scale)
{
    m_scale = scale;
    SetTransformDirty();
    SetBoxDirty();
}

void Node::SetScale(float x, float y, float z)
{
    m_scale = glm::vec3(x,y,z);
    SetTransformDirty();
    SetBoxDirty();
}

const float* Node::GetTransformArray()
{
	return &m_transform[0][0];
}

const glm::mat4 &Node::GetTransformMatrix() const
{
    return m_transform;
}

void Node::AddChild(Node *pNode)
{
    pNode->m_pParent = this;
    m_children.push_back(pNode);

    SetBoxDirty();
}

Node* Node::GetParent()
{
    return m_pParent;
}

void Node::SetTransformDirty()
{
    m_bTransformDirty = true;
    
    //Dirty the children too
    std::vector<Node*>::const_iterator it = m_children.begin();
    std::vector<Node*>::const_iterator end = m_children.end();
    while(it != end)
    {
        (*it)->SetTransformDirty();
        ++it;
    }
}

void Node::SetBoxDirty()
{
    m_bBoxDirty = true;

    //Dirty the parent chain up to root
    Node *nd = m_pParent;
    while(nd)   //nd will be null when it reaches a node with no parent
    {
        nd->SetBoxDirty();
        nd = nd->GetParent();
    }
}

void Node::OnMouseDown(HitData hit)
{}

void Node::OnMouseOver(HitData hit)
{}

void Node::OnMouseUp(HitData hit)
{}

void Node::OnMouseIn()
{}

void Node::OnMouseOut()
{}

void Node::WorldToLocal(float wX, float wY, float wZ, float &lX, float &lY, float &lZ)
{
    glm::vec4 world = glm::vec4(wX, wY, wZ, 1.0);
    glm::mat4 invModel = glm::inverse(m_transform);

    glm::vec3 local = glm::vec3(invModel*world);

    lX = local.x;
    lY = local.y;
    lZ = local.z;
}

bool Node::GetHitList(float wRayOriginX, float wRayOriginY, float wRayOriginZ,
                      float wRayUnitX, float wRayUnitY, float wRayUnitZ,
                      std::vector<HitData> &hitList)
{
    float hitx, hity, hitz;
    
    bool hit = m_boundingBox.RayCollision(wRayOriginX, wRayOriginY, wRayOriginZ, 
                                            wRayUnitX,   wRayUnitY,   wRayUnitZ, 
                                                 hitx,        hity,        hitz);
    
    if(hit)
    {
        HitData dat;
        dat.pNode = this;
        dat.hitX = hitx;
        dat.hitY = hity;
        dat.hitZ = hitz;
        dat.dirX = wRayUnitX;
        dat.dirY = wRayUnitY;
        dat.dirZ = wRayUnitZ;
        hitList.push_back(dat);

        std::vector<Node*>::const_iterator it = m_children.begin();
        std::vector<Node*>::const_iterator end = m_children.end();
        while(it != end)
        {
            (*it)->GetHitList(wRayOriginX, wRayOriginY, wRayOriginZ, wRayUnitX, wRayUnitY, wRayUnitZ, hitList);
            ++it;
        }
    }

    return hit;
}

void Node::Update()
{
    //Update opacity
    if(m_pParent) 
    {
        m_color.a = m_pParent->GetOpacity() * m_localOpacity;
    }
    else
    {
        m_color.a = m_localOpacity;
    }

    UpdateTransform();

    std::vector<Node*>::const_iterator it = m_children.begin();
    std::vector<Node*>::const_iterator end = m_children.end();
    while(it != end)
    {
        (*it)->Update();
        ++it;
    }

    UpdateBoundingBox();
}

const AABB& Node::GetBoundingBox()
{
    return m_boundingBox;
}

void Node::UpdateBoundingBox()
{
    if(!m_bBoxDirty) return;

    int totalBoxes = m_children.size() + 1;
    AABB *boxes = new AABB[totalBoxes]; //All children + mesh

    if(m_pMesh)
    {
        boxes[0] = m_pMesh->GetAABB();

        //Transform the mesh bounding box into world space
        boxes[0].Transform(&m_transform[0][0]);
    }

    //Get all child bounding boxes
    std::vector<Node*>::const_iterator it = m_children.begin();
    std::vector<Node*>::const_iterator end = m_children.end();
    int i = 1;
    while(it != end)
    {
        boxes[i] = (*it)->GetBoundingBox();
        ++it;
        ++i;
    }

    m_boundingBox = AABB::Combine(boxes, totalBoxes);

    m_bBoxDirty = false;
}

void Node::UpdateTransform()
{
    if(!m_bTransformDirty) return;

    glm::mat4 parentTransform;

    if(m_pParent)
    {
        parentTransform = m_pParent->GetTransformMatrix();
    }
    else
    {
        parentTransform = glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    }

    glm::mat4 rotation;
    rotation = glm::rotate(rotation, m_rot.x, glm::vec3(1.0, 0.0, 0.0));
    rotation = glm::rotate(rotation, m_rot.y, glm::vec3(0.0, 1.0, 0.0));
    rotation = glm::rotate(rotation, m_rot.z, glm::vec3(0.0, 0.0, 1.0));

    glm::mat4 translation;
    translation = glm::translate(translation, m_pos);

    glm::mat4 scaling;
    scaling = glm::scale(scaling, m_scale);

    m_transform = parentTransform*translation*rotation*scaling;

    std::vector<Node*>::const_iterator it = m_children.begin();
    std::vector<Node*>::const_iterator end = m_children.end();

    m_bTransformDirty = false;
}

void Node::RegisterIntoList(MainApp *app)
{
    app->RegisterToDraw(this);

    std::vector<Node*>::const_iterator it = m_children.begin();
    std::vector<Node*>::const_iterator end = m_children.end();
    while(it != end)
    {
        (*it)->RegisterIntoList(app);
        ++it;
    } 
}

void Node::SetColor(float r, float g, float b)
{
    m_color.r = r;
    m_color.g = g;
    m_color.b = b;
}

void Node::SetOpacity(float opacity)
{
    m_localOpacity = opacity;
}

void Node::GetColor(float &r, float &g, float &b)
{
    r = m_color.r;
    g = m_color.g;
    b = m_color.b;
}

glm::vec3 Node::GetColor()
{
    return glm::vec3(m_color);
}

float Node::GetOpacity()
{
    return m_color.a;
}

const Mesh *Node::GetMesh()
{
    return m_pMesh;
}

void Node::SetMesh(Mesh *pMesh)
{
    m_pMesh = pMesh;
    SetBoxDirty();
}