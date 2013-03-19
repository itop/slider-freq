#include "Animations.h"
#include <vector>
#include <algorithm>

Animator::Animator() : m_time(0.0f)
{}

Animator* Animator::GetInstance()
{
    static Animator ins;
    return &ins;
}

void Animator::AnimateFloat(Animatable *pObj, float *prop, float duration, float start, float end)
{
    AnimationInformation newAnim;
    newAnim.pObj = pObj;
    newAnim.pFProp = prop;
    newAnim.duration = duration;
    newAnim.startTime = m_time;
    newAnim.fStart = start;
    newAnim.fEnd = end;

    m_objects.push_back(newAnim);
}

void Animator::AnimateInt(Animatable *pObj, int *prop, float duration, int start, int end)
{
    AnimationInformation newAnim;
    newAnim.pObj = pObj;
    newAnim.pIProp = prop;
    newAnim.duration = duration;
    newAnim.startTime = m_time;
    newAnim.iStart = start;
    newAnim.iEnd = end;

    m_objects.push_back(newAnim);
}

void Animator::Update(float delta)
{
    m_time += delta;

    std::vector<AnimationInformation>::iterator i = m_objects.begin();

    while(i != m_objects.end())
    {
        AnimationInformation inf = *i;

        float lerp = (m_time - inf.startTime)/inf.duration;
        if(lerp > 1.0) lerp = 1.0;

        if(inf.pFProp) (*inf.pFProp) = inf.fStart + (inf.fEnd - inf.fStart)*lerp;
        if(inf.pIProp) (*inf.pIProp) = inf.iStart + (inf.iEnd - inf.iStart)*lerp;

        inf.pObj->OnAnimationUpdated();

        if(lerp == 1.0)
        {
            inf.pObj->OnAnimationComplete();
            i = m_objects.erase(i);
        }
        else
        {
            ++i;
        }
    }
}