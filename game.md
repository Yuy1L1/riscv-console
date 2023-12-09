# Game thoughts

Here are a few things to think about when implementing the game.

# Basic elements

## 1. game frame

meaning the rectangle box that the game happens. i.e. 600 px times 200 px.

## 2. dino
the actual character dinosaur that gamers control.

## 3. cactus

serving as obstacle that the dino jumps over it. We can have two variations. One is  small cactus, the other is big.

## 4. bird
serving as obstacle that the dino dives down to avoid it.

## 5. the ground
simple a line that always fill the lower part of the game frame

## 6. cloud
part of the background sprites

# Gamer control
The gamer controls the dino in two ways. One is jump up, the other is dive down. The dino will **jump up** if the gamer presses the **up** button and **dives down** if the game presses the **down** button.
During implementation, we can call **eraseSmallSprite()** to remove the pixels drawn for the dino at the original position. And **drawSmallSprite()** to the new place(i.e. 30 px higher than the original position) to make it looks like a "jump."

# Game Rules

- The rule of the game is very simple:
If the touches the cactus or bird, it dies. The game stops. If not, the game continues. During implementation, we need to keep track of the **position** of dino and the obstacle i.e. cactus. Once we see an **overlap** between pixels, we raise an error and stops the game. We can also print to the screen telling the gamer that the game fails. 

# Implementation

## Main Ideas
We want to demonstrate the following ability based on Prof's description:

-   Launch the application program when cartridge is entered
    
-   Access multi-button controller input
    
-   React to periodic timer
    
-   React to video interrupt
    
-   Draw to the background, large, medium and small sprites
Therefore, here are the proposed functions that should be implemented to the demonstrate the aforementioned abilities.


Parameters:

// create class dinosaur
struct dino{
int pos_x, pos_y;
int color;
};

//create class cactus
{
int pos_x, pos_y;
int size;
};

int game_status;

Functions:
1. setGameFrame()
2. drawSmallCactus()
this should be similar to **drawSmallSprite()**
3. drawBigCactus() -optional
this should be similar to **drawMediumSprite()**
4. drawBird()
this should be similar to **drawSmallSprite()** also
5. drawCloud()
this should be similar to **drawBackgroundSprite()**. Need to contain the pos of the cloud in the upper section of the game frame
6. drawDino(struct dino)
this should be similar to **drawSmallSprite()** also
7. drawGround()
this should be similar to **drawBackgroundSprite()** 
8. initGame()
The game starts in text mode. Call **draw text()** to print "START" to the screen. With the gamer by hitting any keyboard button, **Listen for keyboard interrupt**
Then it changes to **setGraphicsMode()**, calling setGameFrame(), drawGround(), drawDino() drawCloud() and drawSmallCactus().
9. void jump(struct dino, int new_pos_x, int new_pos_y)
call eraseDino() first and then drawDino()
10. bool failDetector()
Set to false by default. If true, end the game by ** some sort of interrupt??**.
pls refer to checkgame(int  y, int  x, int  diY, int  diX)
12. endGame()
in **setTextMode**, print to screen saying "FAILED" using **drawText**


## Refresh Rate

how to create an effect that the game frame is "moving forward"?
