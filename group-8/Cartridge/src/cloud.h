#ifndef CLOUD_H
#define CLOUD_H

#include <stdint.h>

typedef struct {
    float x;
    float y;
} Cloud;

#define MAX_CLOUDS 5
#define CLOUD_SPRITE_INDEX 2
#define CLOUD_MOVEMENT_SPEED -0.3
#define CLOUD_MIN_HEIGHT 20
#define CLOUD_MAX_HEIGHT 50
#define CLOUD_SPAWN_MIN_SPEED 300
#define CLOUD_SPAWN_MAX_SPEED 700

void spawnCloud();
void updateClouds(float extraSpeed);
void drawClouds();

#endif // CLOUD_H
