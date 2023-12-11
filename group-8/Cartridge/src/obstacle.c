#include "obstacle.h"
#include "./syscalls/graphics.h"
#include "./usefulValues.h"
#include "utils.h"
#include "constants.h"

Obstacle obstacles[MAX_OBSTACLES];
volatile int obstacle_spawn_speed = 300;
float obstacleSpawnTimer = 0;
int headIndex = 0;
int tailIndex = 0;

void spawnObstacle() {
    if ((tailIndex + 1) % MAX_OBSTACLES != headIndex) {
        ObstacleType type = (prng() % 100 < AIR_OBSTACLE_CHANCE) ? AIR_OBSTACLE : GROUND_OBSTACLE;
        float y = type == AIR_OBSTACLE ? GROUND_Y - ((float)DINO_HEIGHT/2) - AIR_OBSTACLE_OFFSET: GROUND_Y;
        float height = DINO_HEIGHT;

        Obstacle newObstacle = {SCREEN_WIDTH, y, DINO_WIDTH, height, type};
        obstacles[tailIndex] = newObstacle;
        tailIndex = (tailIndex + 1) % MAX_OBSTACLES;
    }
    obstacle_spawn_speed = OBSTACLE_SPAWN_MIN_SPEED + (prng() % (OBSTACLE_SPAWN_MAX_SPEED - OBSTACLE_SPAWN_MIN_SPEED + 1));
}

void updateObstacles(float extraSpeed) {
    obstacleSpawnTimer += 1;
    if (obstacleSpawnTimer >= obstacle_spawn_speed) {
        spawnObstacle();
        obstacleSpawnTimer = 0;
    }

    for (int i = headIndex; i != tailIndex; i = (i + 1) % MAX_OBSTACLES) {
        obstacles[i].x += (OBSTACLE_MOVEMENT_SPEED - extraSpeed);
        if (obstacles[i].x < 0 - obstacles[i].width) {
            obstacles[i].type = CLEAR_OBSTACLE;
            headIndex = (headIndex + 1) % MAX_OBSTACLES;
            drawSprite((int)obstacles[i].x, (int)obstacles[i].y, 1, CLEAR_SPRITE_INDEX, MEDIUM_T, 0, OBSTACLE_SPRITE_OFFSET + i);
        }
    }
}

uint16_t mapObstacleSprite(ObstacleType type) {
    switch (type) {
        case GROUND_OBSTACLE: return OBSTACLE_SPRITE_INDEX;
        case AIR_OBSTACLE: return OBSTACLE_AIR_SPRITE_INDEX;
        case CLEAR_OBSTACLE: return CLEAR_SPRITE_INDEX;
        default: return 0;
    }
}

void drawObstacles() {
    for (int i = headIndex; i != tailIndex; i = (i + 1) % MAX_OBSTACLES) {
        int controlIndex = OBSTACLE_SPRITE_OFFSET + i;
        if (obstacles[i].type == CLEAR_OBSTACLE) {
            clearSprite(MEDIUM_T, controlIndex);
        } else {
            uint16_t spriteIndex = mapObstacleSprite(obstacles[i].type);
            drawSprite((int)obstacles[i].x, (int)obstacles[i].y, 1, spriteIndex, MEDIUM_T, 0, controlIndex);
        }
    }
}

int checkCollision(uint16_t dinoX, uint16_t dinoY, uint16_t dinoWidth, uint16_t dinoHeight) {
    for (int i = headIndex; i != tailIndex; i = (i + 1) % MAX_OBSTACLES) {
        if (dinoX < obstacles[i].x + obstacles[i].width &&
            dinoX + dinoWidth > obstacles[i].x) {

            if (obstacles[i].type == AIR_OBSTACLE) {
                if (dinoY + dinoHeight > obstacles[i].y + obstacles[i].height) {
                    return 1;
                }
            } else {
                if (dinoY < obstacles[i].y + obstacles[i].height &&
                    dinoY + dinoHeight > obstacles[i].y) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void clearObstacles() {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].type = CLEAR_OBSTACLE;
    }
    drawObstacles();
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i] = (Obstacle){0, 0, 0, 0, CLEAR_OBSTACLE};
    }
    obstacleSpawnTimer = 0;
    obstacle_spawn_speed = OBSTACLE_SPAWN_MIN_SPEED;
    headIndex = 0;
    tailIndex = 0;
}