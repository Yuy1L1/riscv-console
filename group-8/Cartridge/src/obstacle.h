#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdint.h>

typedef enum {
    GROUND_OBSTACLE,
    AIR_OBSTACLE
} ObstacleType;

typedef struct {
    float x;
    float y;
    float width;
    float height;
    ObstacleType type;
} Obstacle;

#define MAX_OBSTACLES 5
#define OBSTACLE_SPRITE_INDEX 8
#define OBSTACLE_AIR_SPRITE_INDEX 10
#define OBSTACLE_MOVEMENT_SPEED -1
#define OBSTACLE_SPAWN_MIN_SPEED 200
#define OBSTACLE_SPAWN_MAX_SPEED 500
#define AIR_OBSTACLE_CHANCE 100

void spawnObstacle();
void updateObstacles();
void drawObstacles();
int checkCollision(uint16_t dinoX, uint16_t dinoY, uint16_t dinoWidth, uint16_t dinoHeight);

#endif // OBSTACLE_H
