#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class AABB
{
public:
    AABB();
    AABB(float x, float y, float z, float width, float height, float depth);
    ~AABB();

    //Expects a 16 float array
    void Transform(const float *mat44);

    void GetExtents(float &minX, float &minY, float &minZ, float &maxX, float &maxY, float &maxZ) const;

    //Static function to combine boxes together
    static AABB Combine(AABB *boxes, unsigned int count);

    bool RayCollision(float rx, float ry, float rz, float dx, float dy, float dz, float &px, float &py, float &pz) const;

private:
    void AxisAlign();
    glm::vec3 minExtents;
    glm::vec3 maxExtents;
};

#endif