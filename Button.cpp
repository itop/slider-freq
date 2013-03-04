#include "Button.h"
#include "Mesh.h"
#include "MainApp.h"


Button::Button(MainApp *app)
{
    m_pApp = app;
}

Button::~Button()
{

}

void Button::OnMouseDown(HitData hit)
{
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
        SetColor(m_downColor.r, m_downColor.g, m_downColor.b);

        m_pApp->OnButtonPressed(this);
    }   
}

void Button::SetUpColor(float r, float g, float b)
{
    m_upColor = glm::vec3(r,g,b);
    SetColor(r,g,b); //Default is up
}

void Button::SetDownColor(float r, float g, float b)
{
    m_downColor = glm::vec3(r,g,b);
}

void Button::OnMouseUp(HitData hit)
{
    SetColor(m_upColor.r, m_upColor.g, m_upColor.b);
}

void Button::OnMouseOut()
{
    //Treat like OnMouseUp
    HitData dummy;
    dummy.pNode = this;
    OnMouseUp(dummy);
}