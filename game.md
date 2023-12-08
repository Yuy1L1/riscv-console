# Game thoughts

Here are a few things to think about when implementing the game.

# Basic elements

## 1. game frame

meaning the rectangle box that the game happens. i.e. 600 px times 200 px.

## 2. dino
the actual character dinosaur that gamers control.

## 3. cactus

serving as obstacle that the dino jumps over it.

## 4. bird
serving as obstacle that the dino dives down to avoid it.

## 5. the ground
simple a line that always fill the game frame

# Gamer control
The gamer controls the dino in two ways. One is jump up, the other is dive down. The dino will **jump up** if the gamer presses the **up** button and **dives down** if the game presses the **down** button.
During implementation, we can call **eraseSmallSprite()** to remove the pixels drawn for the dino at the original position. And **drawSmallSprite()** to the new place(i.e. 30 px higher than the original position) to make it looks like a "jump."

# Game Rules

- The rule of the game is very simple:
If the touches the cactus or bird, it dies. If not, the game continues. During implementation, we need to keep track of the **position** of dino and the obstacle i.e. cactus. Once we see an **overlap** between pixels, we raise an error and stops the game. We can also print to the screen telling the gamer that the game fails. 

# Other Variables to Consider

## Refresh Rate

how to create an effect that the game frame is "moving forward"?
