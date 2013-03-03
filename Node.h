#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <vector>

//Forward declaration
class MainApp;
class Mesh;

class Node {
public:
    Node();

    void AddChild(Node *pNode);

    void SetPosition(glm::vec3 pos);
    void SetPosition(float x, float y, float z);

    void SetMesh(Mesh *pMesh);
    
    void SetRotation(glm::vec3 rot);
    void SetRotation(float x, float y, float z);

    void SetScale(glm::vec3 scale);
    void SetScale(float x, float y, float z);

    void RegisterIntoList(MainApp *app);

    void UpdateTransform();

	const float *GetTransform();
    const Mesh *GetMesh();

private:
    void SetDirty();

    bool m_bDirty;
    Node *m_pParent;
    Mesh *m_pMesh;
    std::vector<Node*> m_children;

    glm::vec3 m_pos;
    glm::vec3 m_rot;
    glm::vec3 m_scale;
    glm::mat4 m_transform;
};

#endif