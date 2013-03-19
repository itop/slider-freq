#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <vector>

//An object that can be animated
class Animatable {
public:
    virtual void OnAnimationComplete() = 0;
    virtual void OnAnimationUpdated() = 0;
};

class Animator {
public:
    static Animator* GetInstance();

    void Update(float delta);
    void AnimateFloat(Animatable *pObj, float *prop, float duration, float start, float end);
    void AnimateInt(Animatable *pObj, int *prop, float duration, int start, int end);

private:
    struct AnimationInformation
    {
        Animatable *pObj;
        float fStart;
        float fEnd;
        float duration;
        float startTime;
        int iStart;
        int iEnd;
        float *pFProp;
        int *pIProp;

        AnimationInformation() : pFProp(0), pIProp(0), pObj(0) {}
    };

    Animator();
    float m_time;
    std::vector<AnimationInformation> m_objects;
};

#endif