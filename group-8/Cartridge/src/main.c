#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./syscalls/events.h"
#include "./syscalls/graphics.h"
#include "./syscalls/input.h"
#include "./syscalls/state.h"
#include "./syscalls/threads.h"
#include "./syscalls/time.h"
#include "./usefulValues.h"
#include "./spriteData.h"

// Player
#define GRAVITY 0.05
#define FAST_FALL_FORCE 0.8
#define JUMP_FORCE 3
#define GROUND_Y 200
#define DINO_SPRITE_INDEX 0
#define DINO_CONTROL_STRUCTURE_INDEX 1
#define DINO_HEIGHT 32
#define DINO_WIDTH 32
#define JUMP_KEY U_KEY
#define FAST_FALL_KEY J_KEY

// Obstacles
typedef struct {
    float x;
    float y;
    float width;
    float height;
} Obstacle;

#define MAX_OBSTACLES 5
#define OBSTACLE_SPRITE_INDEX 8
#define OBSTACLE_MOVEMENT_SPEED -1
#define OBSTACLE_SPAWN_MIN_SPEED 200
#define OBSTACLE_SPAWN_MAX_SPEED 500
Obstacle obstacles[MAX_OBSTACLES];
volatile int obstacle_spawn_speed = 300;
int numObstacles = 0;
float obstacleSpawnTimer = 0;

volatile int last_global = 0;
volatile int global = 0;
volatile int controller_status = 0;
volatile int last_reset;
volatile int reset;

uint16_t dinoX = 50;
volatile float dinoY = GROUND_Y;
volatile float dinoVelocityY = 0;
volatile int isJumping = 0;
uint16_t dinoZ = 1;

#define LCG_A 1664525
#define LCG_C 1013904223
#define LCG_M 4294967296 // 2^32

static uint32_t prng_state;

void seedPRNG(uint32_t seed) {
    prng_state = seed;
}

uint32_t prng() {
    prng_state = (LCG_A * prng_state + LCG_C) % LCG_M;
    return prng_state;
}

int numDigits(long long int number) {
    int digits = 0;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits ? digits : 1;
}

int getDigit(long long int number, int digitIndex) {
    while (digitIndex--) {
        number /= 10;
    }
    return number % 10;
}

void displayNumberUsingSprites(uint16_t x, uint16_t y, uint16_t z, long long int number) {
    int length = numDigits(number);
    uint32_t currentControlStructure = 250;
    for (int i = 0; i < length; i++) {
        int digit = getDigit(number, length - i - 1);

        uint16_t spriteIndex = (digit == 0) ? 25 : 16 + digit - 1;

        drawSprite(x + (i * 32), y, z, spriteIndex, MEDIUM_T, 0, currentControlStructure--);

        if (currentControlStructure < 200) break;
    }
}

void spawnObstacle() {
    if (numObstacles < MAX_OBSTACLES) {
        Obstacle newObstacle = {SCREEN_WIDTH, GROUND_Y, DINO_HEIGHT, DINO_WIDTH}; // Example values
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

int checkCollision() {
    for (int i = 0; i < numObstacles; i++) {
        if (dinoX < obstacles[i].x + obstacles[i].width &&
            dinoX + DINO_WIDTH > obstacles[i].x &&
            dinoY < obstacles[i].y + obstacles[i].height &&
            dinoY + DINO_HEIGHT > obstacles[i].y) {
            return 1;
        }
    }
    return 0;
}

void initializeGame() {
    loadSprites(SPRITE_DATA);
    clearTextArea(0, 0, TEXT_WIDTH, TEXT_HEIGHT);
    setGraphicsMode(GRAPHICS_MODE);
}

void drawInitialState() {
    drawSprite(0, 0, 0, 0, BACKGROUND_T, 0, 0);
    drawSprite(dinoX, (int)dinoY, dinoZ, DINO_SPRITE_INDEX, MEDIUM_T, 0, DINO_CONTROL_STRUCTURE_INDEX);
}

void updateGameState() {
    controller_status = getController();
    if (controller_status & JUMP_KEY) {
        if (!isJumping) {
            isJumping = 1;
            dinoVelocityY = -JUMP_FORCE;
        }
    }

    if ((controller_status & FAST_FALL_KEY) && isJumping) {
        dinoVelocityY += FAST_FALL_FORCE;
    } else if (isJumping) {
        dinoVelocityY += GRAVITY;
    }

    dinoY += dinoVelocityY;

    if (dinoY >= GROUND_Y) {
        dinoY = GROUND_Y;
        isJumping = 0;
    }
}

void renderGame() {
    drawSprite(dinoX, (int)dinoY, 1, DINO_SPRITE_INDEX, MEDIUM_T, 0, DINO_CONTROL_STRUCTURE_INDEX);
}

int main() {
    initializeGame();
    drawInitialState();
    last_reset = getReset();

    while (1) {
        reset = getReset();
        global = getTicks();
        if (global != last_global) {
            updateGameState();
            updateObstacles();

            if (checkCollision()) {
                setGraphicsMode(TEXT_MODE);
                drawText(2, 2, "You lost!");
            }

            renderGame();
            drawObstacles();
            last_global = global;
            displayNumberUsingSprites(30, 150, 3, (long long int) global);
        }
    }
    return 0;
}
