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
        ObstacleType type = (prng() % 100 < AIR_OBSTACLE_CHANCE) ? AIR_OBSTACLE : GROUND_OBSTACLE;
        float y = type == AIR_OBSTACLE ? GROUND_Y - ((float)DINO_HEIGHT/2) - AIR_OBSTACLE_OFFSET: GROUND_Y;
        float height = DINO_HEIGHT;

        Obstacle newObstacle = {SCREEN_WIDTH, y, DINO_WIDTH, height, type};
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
        int controlIndex = OBSTACLE_SPRITE_OFFSET + i;
        uint16_t spriteIndex = obstacles[i].type == AIR_OBSTACLE ? OBSTACLE_AIR_SPRITE_INDEX : OBSTACLE_SPRITE_INDEX;
        drawSprite((int)obstacles[i].x, (int)obstacles[i].y, 1, spriteIndex, MEDIUM_T, 0, controlIndex);
    }
}

int checkCollision(uint16_t dinoX, uint16_t dinoY, uint16_t dinoWidth, uint16_t dinoHeight) {
    for (int i = 0; i < numObstacles; i++) {
        if (dinoX < obstacles[i].x + obstacles[i].width &&
            dinoX + dinoWidth > obstacles[i].x) {

            if (obstacles[i].type == GROUND_OBSTACLE && 
                dinoY < obstacles[i].y + obstacles[i].height &&
                dinoY + dinoHeight > obstacles[i].y) {
                return 1;
            }

            if (obstacles[i].type == AIR_OBSTACLE &&
                dinoY + dinoHeight > obstacles[i].y) {
                return 1;
            }
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
