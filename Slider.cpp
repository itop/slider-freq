#include "Slider.h"
#include "Mesh.h"
#include "AABB.h"
#include "MainApp.h"

Slider::Slider(MainApp *app)
{
    m_bMouseDown = false;
    m_pApp = app;
    m_pKnob = new Node;
    AddChild(m_pKnob);
    m_fStart = 0;
    m_fEnd = 0;
    m_fNormalizedPosition = 0;
}

Slider::~Slider()
{
    delete m_pKnob;
    m_pKnob = NULL;
}

void Slider::SetKnobColor(float r, float g, float b)
{
    m_pKnob->SetColor(r,g,b);
}

void Slider::SetRange(float start, float end)
{
    m_fStart = start;
    m_fEnd   = end;
}

void Slider::Update()
{
    if(!m_pMesh) return; //Can't do anything without our local height

    AABB sliderBox = m_pMesh->GetAABB();

    float dontCare;
    float minY, maxY;
    sliderBox.GetExtents(dontCare, minY, dontCare, dontCare, maxY, dontCare);

    m_fHeight = maxY - minY;

    PlaceKnob();

    Node::Update();
}

void Slider::PlaceKnob()
{
    //Position the knob based on our value
    m_pKnob->SetPosition(0.0, (m_fHeight - m_fKnobHeight)*m_fNormalizedPosition - (m_fHeight - m_fKnobHeight) / 2.0, m_fKnobDepth);
}

float Slider::GetValue()
{
    return m_fStart + m_fNormalizedPosition*m_fEnd;
}

void Slider::SetKnobMesh(Mesh *pKnob)
{
    m_pKnob->SetMesh(pKnob);
    AABB knobBox = pKnob->GetAABB();

    float dontCare;
    float minY, maxY, minZ, maxZ;
    knobBox.GetExtents(dontCare, minY, minZ, dontCare, maxY, maxZ);

    m_fKnobDepth = maxZ - minZ;
    m_fKnobHeight = maxY - minY;
}

void Slider::OnMouseIn()
{
    SetOpacity(1.0);
}

void Slider::OnMouseOut()
{
    SetOpacity(0.5);

    //Treat mouse out like a release
    HitData dummy;
    dummy.pNode = this;
    OnMouseUp(dummy);
}

void Slider::OnMouseDown(HitData hit)
{
    if(m_bMouseDown) return; //Mouse is already down

    if(!m_pMesh) return;

    float lx,ly,lz;
    WorldToLocal(hit.hitX, hit.hitY, hit.hitZ, lx, ly, lz);

    float dx,dy,dz;
    WorldToLocal(hit.hitX + hit.dirX, hit.hitY + hit.dirY, hit.hitZ + hit.dirZ, dx, dy, dz);
    //Re-normalize unit vector
    dx -= lx;
    dy -= ly;
    dz -= lz;
    float mag = sqrt(dx*dx + dy*dy + dz*dz);
    dx /= mag;
    dy /= mag;
    dz /= mag;

    float hitx, hity, hitz;
    if(m_pMesh->GetAABB().RayCollision(lx, ly, lz, dx, dy, dz, hitx, hity, hitz))
    {
        m_bMouseDown = true;
        m_fNormalizedPosition = hity/m_fHeight + 0.5; //hity is from -m_fHeight to m_fHeight

        //Clamp
        m_fNormalizedPosition = m_fNormalizedPosition < 0 ? 0 : m_fNormalizedPosition;
        m_fNormalizedPosition = m_fNormalizedPosition > 1 ? 1 : m_fNormalizedPosition;

        PlaceKnob();
    }   
}

void Slider::OnMouseOver(HitData hit)
{
    if(!m_pMesh) return;

    if(!m_bMouseDown) return; //Only do this if the mouse is held down

    float lx,ly,lz;
    WorldToLocal(hit.hitX, hit.hitY, hit.hitZ, lx, ly, lz);

    float dx,dy,dz;
    WorldToLocal(hit.hitX + hit.dirX, hit.hitY + hit.dirY, hit.hitZ + hit.dirZ, dx, dy, dz);
    //Re-normalize unit vector
    dx -= lx;
    dy -= ly;
    dz -= lz;
    float mag = sqrt(dx*dx + dy*dy + dz*dz);
    dx /= mag;
    dy /= mag;
    dz /= mag;

    float hitx, hity, hitz;
    if(m_pMesh->GetAABB().RayCollision(lx, ly, lz, dx, dy, dz, hitx, hity, hitz))
    {
        m_fNormalizedPosition = hity/m_fHeight + 0.5; //hity is from -m_fHeight to m_fHeight

        //Clamp
        m_fNormalizedPosition = m_fNormalizedPosition < 0 ? 0 : m_fNormalizedPosition;
        m_fNormalizedPosition = m_fNormalizedPosition > 1 ? 1 : m_fNormalizedPosition;

        PlaceKnob();
    }
}

void Slider::OnMouseUp(HitData hit)
{
    if(!m_bMouseDown) return; //Mouse wasn't pressed in the slider
    m_bMouseDown = false;

    if(m_pApp)
        m_pApp->OnSliderReleased(this);
}