#include "AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include <algorithm>

AABB::AABB()
{
    
}

AABB::AABB(float x, float y, float z, float width, float height, float depth)
{
    minExtents.x = x - width/2.0;
    minExtents.y = y - height/2.0;
    minExtents.z = z - depth/2.0;

    maxExtents.x = x + width/2.0;
    maxExtents.y = y + height/2.0;
    maxExtents.z = z + depth/2.0;
}

AABB::~AABB()
{

}

bool AABB::RayCollision(float rx, float ry, float rz, float dx, float dy, float dz, float &px, float &py, float &pz) const
{
    float dirFracX, dirFracY, dirFracZ;
    dirFracX = 1.0f / dx;
    dirFracY = 1.0f / dy;
    dirFracZ = 1.0f / dz;

    float minX, minY, minZ, maxX, maxY, maxZ;
    GetExtents(minX, minY, minZ, maxX, maxY, maxZ);

    float t1 = (minX - rx)*dirFracX;
    float t2 = (maxX - rx)*dirFracX;
    float t3 = (minY - ry)*dirFracY;
    float t4 = (maxY - ry)*dirFracY;
    float t5 = (minZ - rz)*dirFracZ;
    float t6 = (maxZ - rz)*dirFracZ;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    float t;
    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behind us
    if (tmax < 0)
    {
        t = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        t = tmax;
        return false;
    }

    t = tmin;

    px = rx + dx*t;
    py = ry + dy*t;
    pz = rz + dz*t;

    return true;
}

void AABB::AxisAlign()
{
    //Re-axis-align the box by finding its min and max XYZs
    float minX =  10000000; //Really big
    float minY =  10000000;
    float minZ =  10000000;
    float maxX = -10000000; //Really small
    float maxY = -10000000;
    float maxZ = -10000000;

    if(minExtents.x < minX) minX = minExtents.x;
    if(maxExtents.x < minX) minX = maxExtents.x;
    if(minExtents.x < minX) minX = minExtents.x;
    if(maxExtents.y < minY) minY = maxExtents.y;
    if(minExtents.y < minY) minY = minExtents.y;
    if(maxExtents.y < minY) minY = maxExtents.y;
    if(maxExtents.z < minZ) minZ = maxExtents.z;
    if(minExtents.z < minZ) minZ = minExtents.z;
    if(maxExtents.z < minZ) minZ = maxExtents.z;

    if(minExtents.x > maxX) maxX = minExtents.x;
    if(maxExtents.x > maxX) maxX = maxExtents.x;
    if(minExtents.x > maxX) maxX = minExtents.x;
    if(maxExtents.y > maxY) maxY = maxExtents.y;
    if(minExtents.y > maxY) maxY = minExtents.y;
    if(maxExtents.y > maxY) maxY = maxExtents.y;
    if(maxExtents.z > maxZ) maxZ = maxExtents.z;
    if(minExtents.z > maxZ) maxZ = minExtents.z;
    if(maxExtents.z > maxZ) maxZ = maxExtents.z;

    minExtents = glm::vec3(minX, minY, minZ);
    maxExtents = glm::vec3(maxX, maxY, maxZ);
}

void AABB::Transform(const float *mat44)
{
    glm::mat4 mat(mat44[ 0], mat44[ 1], mat44[ 2], mat44[ 3],
                  mat44[ 4], mat44[ 5], mat44[ 6], mat44[ 7],
                  mat44[ 8], mat44[ 9], mat44[10], mat44[11],
                  mat44[12], mat44[13], mat44[14], mat44[15]);

    glm::vec4 transMin = mat*glm::vec4(minExtents, 1.0);
    glm::vec4 transMax = mat*glm::vec4(maxExtents, 1.0);

    minExtents = glm::vec3(transMin);
    maxExtents = glm::vec3(transMax);

    AxisAlign();
}

void AABB::GetExtents(float &minX, float &minY, float &minZ, float &maxX, float &maxY, float &maxZ) const
{
    minX = minExtents.x;
    minY = minExtents.y;
    minZ = minExtents.z;
    maxX = maxExtents.x;
    maxY = maxExtents.y;
    maxZ = maxExtents.z;
}

AABB AABB::Combine(AABB *boxes, unsigned int count)
{    
    float minX =  10000000; //Really big
    float minY =  10000000;
    float minZ =  10000000;
    float maxX = -10000000; //Really small
    float maxY = -10000000;
    float maxZ = -10000000;

    for(int i = 0; i < count; i++)
    {
        AABB &box = boxes[i];
        float bMinX, bMinY, bMinZ;
        float bMaxX, bMaxY, bMaxZ;

        box.GetExtents(bMinX, bMinY, bMinZ, bMaxX, bMaxY, bMaxZ);

        if(bMinX < minX) minX = bMinX;
        if(bMinY < minY) minY = bMinY;
        if(bMinZ < minZ) minZ = bMinZ;
        if(bMaxX > maxX) maxX = bMaxX;
        if(bMaxY > maxY) maxY = bMaxY;
        if(bMaxZ > maxZ) maxZ = bMaxZ;
    }

    return AABB( (minX + maxX) / 2.0, (minY + maxY) / 2.0, (minZ + maxZ) / 2.0, 
                 (maxX - minX)      , (maxY - minY)      , (maxZ - minZ)        );
}