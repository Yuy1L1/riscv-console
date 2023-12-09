#include "game.h"
#include "sprite_management.h"
#include "text.h"


extern int game_status = 1;

void setGameFrame();

void drawDino(uint32_t pos_x, uint32_t pos_y)
{

}


void drawSmallCactus(struct cactus small_cactus);

void drawBigCactus(struct cactus big_cactus);
void drawBird(struct bird flybird);

void drawCloud();
void drawGround();

void jump(struct dino my_dino)
{
    uint32_t cur_pos_x = my_dino.pos_x;
    uint32_t cur_pos_y = my_dino.pos_y;

    eraseDino(cur_pos_x, cur_pos_y);

    uint32_t new_pos_x = cur_pos_x + 3;
    uint32_t new_pos_y = cur_pos_y + 50;

    drawDino(new_pos_x, new_pos_y);

    return;
}

int checkStatus(struct dino my_dino, struct cactus small_cactus, struct bird flybird)
{
    // TODO: need to know the size of cactus and bird somehow?
    // check for cactus
    if (my_dino.pos_y == small_cactus.pos_y) {
		if (abs((my_dino.pos_x +14)-small_cactus.pos_x) <= 4) {
			return -1;
		}
	}
    // check for birds
    if (my_dino.pos_y == flybird.pos_y) {
		if (abs((my_dino.pos_x +14)-flybird.pos_x) <= 4) {
			return -1;
		}
	}
	return 1;
}

void initGame(){
    setTextMode();
    setTextColorPalette();
    char input_str[] = "TREX GAME";
    drawText(input_str);
}


void endGame(){

}


void startEngine()
{    
    while (game_status == 1)
    {
        continue;
    }
    // 
    endGame();
}