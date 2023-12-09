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
#include "obstacle.h"
#include "utils.h"
#include "constants.h"

// Player
#define GRAVITY 0.05
#define FAST_FALL_FORCE 0.8
#define JUMP_FORCE 3
#define DINO_SPRITE_INDEX 3
#define DINO_CROUCHING_SPRITE_INDEX 4
#define DINO_CONTROL_STRUCTURE_INDEX 1
#define JUMP_KEY W_KEY
#define FAST_FALL_KEY X_KEY

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
volatile uint16_t currentDinoSpriteIndex = DINO_SPRITE_INDEX;
volatile uint16_t currentDinoHeight = DINO_HEIGHT;
volatile uint32_t highScore = 0;
volatile uint32_t curScore = 0;
volatile int isCollided = 0;


void initializeGame() {
    loadSprites(SPRITE_DATA);
    clearTextArea(0, 0, TEXT_WIDTH, TEXT_HEIGHT);
    setGraphicsMode(GRAPHICS_MODE);
    clearObstacles();
    curScore = 0;
}

void drawInitialState() {
    drawSprite(0, 0, 0, 0, BACKGROUND_T, 0, 0);
    drawSprite(dinoX, (int)dinoY, dinoZ, DINO_SPRITE_INDEX, MEDIUM_T, 0, DINO_CONTROL_STRUCTURE_INDEX);
}

void updateGameState() {
    if (isCollided) {
        return;
    }

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

        if (controller_status & FAST_FALL_KEY) {
            currentDinoSpriteIndex = DINO_CROUCHING_SPRITE_INDEX;
            currentDinoHeight = (DINO_HEIGHT / 2) - AIR_OBSTACLE_OFFSET;
        } else {
            currentDinoSpriteIndex = DINO_SPRITE_INDEX;
            currentDinoHeight = DINO_HEIGHT;
        }
    }
    curScore++;
}

void renderGame() {
    drawSprite(dinoX, (int)dinoY, 1, currentDinoSpriteIndex, MEDIUM_T, 0, DINO_CONTROL_STRUCTURE_INDEX);
}

int main() {
    initializeGame();
    drawInitialState();
    last_reset = getReset();

    while (1) {
        reset = getReset();
        global = getTicks();
        controller_status = getController();

        if (global != last_global) {
            if (!isCollided) {
                updateGameState();
                updateObstacles();
            }

            if (checkCollision(dinoX, dinoY, DINO_WIDTH, currentDinoHeight) && !isCollided) {
                isCollided = 1;
                if (curScore > highScore) {
                    highScore = curScore;
                }
            }

            if (isCollided && (controller_status & JUMP_KEY)) {
                isCollided = 0;
                isJumping = 0;
                dinoY = GROUND_Y;
                dinoVelocityY = 0;

                initializeGame();
                drawInitialState();
            }

            if (!isCollided) {
                renderGame();
                drawObstacles();
            }
            last_global = global;
            displayNumberUsingSprites(30, 10, 3, (long long int) curScore, 0);
            displayNumberUsingSprites(30, 60, 3, (long long int) highScore, 1);
        }
    }
    return 0;
}
