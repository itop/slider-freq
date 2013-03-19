#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <vector>

#include "AABB.h"
#include "Animations.h"

//Forward declaration
class MainApp;
class Mesh;

class Node : public Animatable {
public:

    struct HitData {
        Node *pNode;
        float hitX;
        float hitY;
        float hitZ;
        float dirX;
        float dirY;
        float dirZ;
    };

    Node();
    virtual ~Node();

    void AddChild(Node *pNode);

    void SetPosition(glm::vec3 pos);
    void SetPosition(float x, float y, float z);
    void AnimateDeltaPosition(float deltaX, float deltaY, float deltaZ, float durationS);

    void SetMesh(Mesh *pMesh);
    
    void SetRotation(glm::vec3 rot);
    //In degrees
    void SetRotation(float pitch, float yaw, float roll);
    void AnimateDeltaRotation(float deltaPitch, float deltaYaw, float deltaRoll, float durationS);

    void SetScale(glm::vec3 scale);
    void SetScale(float x, float y, float z);
    void AnimateDeltaScale(float deltaX, float deltaY, float deltaZ, float durationS);

    void RegisterIntoList(MainApp *app);

    //Return a list of child nodes that were hit, and their hit locations
    bool GetHitList(float wRayOriginX, float wRayOriginY, float wRayOriginZ,
                    float wRayUnitX, float wRayUnitY, float wRayUnitZ,
                    std::vector<HitData> &hitList);

    //Subclasses MUST call the super class implementation using Node::Update()
    //Update() is called every frame
    virtual void Update();

    //From Animatable
    virtual void OnAnimationComplete();
    virtual void OnAnimationUpdated();

    Node* GetParent();

    void SetColor(float r, float g, float b);
    void SetOpacity(float opacity);
    void AnimateDeltaOpacity(float deltaOpacity, float durationS);

    glm::vec3 GetColor();
    void GetColor(float &r, float &g, float &b);
    
    float GetOpacity();

    const AABB& GetBoundingBox();

	const float *GetTransformArray();
    const glm::mat4 &GetTransformMatrix() const;
    const Mesh *GetMesh();

    void WorldToLocal(float wX, float wY, float wZ, float &lX, float &lY, float &lZ);

    /*
        Input related functions,

        To be overridden in subclasses to do specific operations when a node
        is interacted with
    */
    //Called when a node is clicked
    virtual void OnMouseDown(HitData hit);

    //Called when a node is released
    virtual void OnMouseUp(HitData hit);

    //Called when mouse is hovering on top of the node
    virtual void OnMouseOver(HitData hit);

    //Called when the cursor enters the node
    virtual void OnMouseIn();

    //Called when the cursor exits the node
    virtual void OnMouseOut();

protected:
    void UpdateTransform();
    void UpdateBoundingBox();
    void SetTransformDirty();
    void SetBoxDirty();

    AABB m_boundingBox;
    bool m_bTransformDirty;
    bool m_bBoxDirty;
    Node *m_pParent;
    Mesh *m_pMesh;
    std::vector<Node*> m_children;
    
    glm::vec4 m_color;
    float m_localOpacity;
    glm::vec3 m_pos;
    glm::vec3 m_rot;
    glm::vec3 m_scale;
    glm::mat4 m_transform;
};

#endif