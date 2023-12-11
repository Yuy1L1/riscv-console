#include "cloud.h"
#include "./syscalls/graphics.h"
#include "./usefulValues.h"
#include "utils.h"
#include "constants.h"

Cloud clouds[MAX_CLOUDS];
int cloud_spawn_speed = 500; 
float cloudSpawnTimer = 0;
int cloudHeadIndex = 0;
int cloudTailIndex = 0;

void spawnCloud() {
    if ((cloudTailIndex + 1) % MAX_CLOUDS != cloudHeadIndex) {
        float cloudHeight = CLOUD_MIN_HEIGHT + (prng() % (CLOUD_MAX_HEIGHT - CLOUD_MIN_HEIGHT + 1));
        Cloud newCloud = {SCREEN_WIDTH, cloudHeight};
        clouds[cloudTailIndex] = newCloud;
        cloudTailIndex = (cloudTailIndex + 1) % MAX_CLOUDS;

        cloud_spawn_speed = CLOUD_SPAWN_MIN_SPEED + (prng() % (CLOUD_SPAWN_MAX_SPEED - CLOUD_SPAWN_MIN_SPEED + 1));
    }
}

void updateClouds(float extraSpeed) {
    cloudSpawnTimer += 1;
    if (cloudSpawnTimer >= cloud_spawn_speed) {
        spawnCloud();
        cloudSpawnTimer = 0;
    }

    for (int i = cloudHeadIndex; i != cloudTailIndex; i = (i + 1) % MAX_CLOUDS) {
        clouds[i].x += (CLOUD_MOVEMENT_SPEED - extraSpeed);
        if (clouds[i].x < -64) {
            cloudHeadIndex = (cloudHeadIndex + 1) % MAX_CLOUDS;
            drawSprite((int)clouds[i].x, (int)clouds[i].y, 1, CLOUD_SPRITE_INDEX, LARGE_T, 0, CLOUD_SPRITE_OFFSET + i);
        }
    }
}

void drawClouds() {
    for (int i = cloudHeadIndex; i != cloudTailIndex; i = (i + 1) % MAX_CLOUDS) {
        drawSprite((int)clouds[i].x, (int)clouds[i].y, 1, CLOUD_SPRITE_INDEX, LARGE_T, 0, CLOUD_SPRITE_OFFSET + i);
    }
}
