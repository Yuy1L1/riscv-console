// THIS IS A T REX GAME

#include <stdlib.h>
#include <ncurses.h>

struct dino{
    uint32_t pos_x;
    uint32_t pos_y;
    // TODO how to get shape in?
};


struct cactus{
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t size;
    // TODO how to get shape in?
};

struct bird{
    uint32_t pos_x;
    uint32_t pos_y;
    // TODO how to get shape in?
};


void setGameFrame();

// main sprites
void drawDino(uint32_t pos_x, uint32_t pos_y);
void eraseDino(uint32_t pos_x, uint32_t pos_y);
void drawSmallCactus(struct cactus small_cactus);
// void drawBigCactus(struct cactus big_cactus);
void drawBird(struct bird flybird);

// background sprites
void drawCloud();
void drawGround();

void jump(struct dino);

int checkStatus(struct dino my_dino, struct cactus small_cactus, struct bird fly_bird);

void initGame();

void endGame();

