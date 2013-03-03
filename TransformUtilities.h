#ifndef TRANSFORM_UTILITIES_H
#define TRANSFORM_UTILITIES_H

class Camera;

void WorldToScreen(const float &worldX, const float &worldY, const float &worldZ, const Camera *pCamera, const float screenW, const float screenH, float &screenX, float &screenY, float &screenZ);
void ScreenToWorld(const float &screenX, const float &screenY, const float &screenZ, const Camera *pCamera, const float screenW, const float screenH, float &worldX, float &worldY, float &worldZ);

#endif