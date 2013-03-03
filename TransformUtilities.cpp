#include "TransformUtilities.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

void WorldToScreen(const float &worldX, const float &worldY, const float &worldZ, const Camera *pCamera, const float screenW, const float screenH, float &screenX, float &screenY, float &screenZ)
{  
    glm::vec3 world = glm::vec3(worldX, worldY, worldZ);
    const float *mat = pCamera->GetProjectionMatrix();
    glm::mat4 projectionMatrix = glm::mat4(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
    mat = pCamera->GetViewMatrix();
    glm::mat4 viewMatrix = glm::mat4(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);

    glm::vec3 screen = glm::project(world, viewMatrix, projectionMatrix, glm::vec4(0.0, 0.0, screenW, screenH));

    screenX = screen.x;
    screenY = screen.y;
    screenZ = screen.z;
}


void ScreenToWorld(const float &screenX, const float &screenY, const float &screenZ, const Camera *pCamera, const float screenW, const float screenH, float &worldX, float &worldY, float &worldZ)
{
    glm::vec3 screen = glm::vec3(screenX, screenY, screenZ);
    const float *mat = pCamera->GetProjectionMatrix();
    glm::mat4 projectionMatrix = glm::mat4(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);
    mat = pCamera->GetViewMatrix();
    glm::mat4 viewMatrix = glm::mat4(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8], mat[9], mat[10], mat[11], mat[12], mat[13], mat[14], mat[15]);

    glm::vec3 world = glm::unProject(screen, viewMatrix, projectionMatrix, glm::vec4(0.0, 0.0, screenW, screenH));

    worldX = world.x;
    worldY = world.y;
    worldZ = world.z;
}