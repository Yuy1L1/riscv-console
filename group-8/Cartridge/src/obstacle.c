#include "obstacle.h"
#include "./syscalls/graphics.h"
#include "./usefulValues.h"
#include "utils.h"
#include "constants.h"

Obstacle obstacles[MAX_OBSTACLES];
volatile int obstacle_spawn_speed = 300;
int numObstacles = 0;
float obstacleSpawnTimer = 0;

void spawnObstacle() {
    if (numObstacles < MAX_OBSTACLES) {
        Obstacle newObstacle = {SCREEN_WIDTH, GROUND_Y, DINO_HEIGHT, DINO_WIDTH};
        obstacles[numObstacles++] = newObstacle;
    }
    obstacle_spawn_speed = OBSTACLE_SPAWN_MIN_SPEED + (prng() % (OBSTACLE_SPAWN_MAX_SPEED - OBSTACLE_SPAWN_MIN_SPEED + 1));
}

void updateObstacles() {
    obstacleSpawnTimer += 1;
    if (obstacleSpawnTimer >= obstacle_spawn_speed) {
        spawnObstacle();
        obstacleSpawnTimer = 0;
    }

    for (int i = 0; i < numObstacles; i++) {
        obstacles[i].x += OBSTACLE_MOVEMENT_SPEED;
        if (obstacles[i].x < -obstacles[i].width) {
            obstacles[i] = obstacles[numObstacles - 1];
            numObstacles--;
        }
    }
}

void drawObstacles() {
    for (int i = 0; i < numObstacles; i++) {
        int controlIndex = 2 + i;
        drawSprite((int)obstacles[i].x, (int)obstacles[i].y, 1, OBSTACLE_SPRITE_INDEX, MEDIUM_T, 0, controlIndex);
    }
}

int checkCollision(uint16_t dinoX, uint16_t dinoY, uint16_t dinoWidth, uint16_t dinoHeight) {
    for (int i = 0; i < numObstacles; i++) {
        if (dinoX < obstacles[i].x + obstacles[i].width &&
            dinoX + dinoWidth > obstacles[i].x &&
            dinoY < obstacles[i].y + obstacles[i].height &&
            dinoY + dinoHeight > obstacles[i].y) {
            return 1;
        }
    }
    return 0;
}

void clearObstacles() {
    numObstacles = 0;
    obstacleSpawnTimer = 0;
    obstacle_spawn_speed = OBSTACLE_SPAWN_MIN_SPEED;

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i] = (Obstacle){0, 0, 0, 0};
    }
}
