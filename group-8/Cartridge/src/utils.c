#include "utils.h"
#include "./syscalls/graphics.h"
#include "./usefulValues.h"

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

void displayNumberUsingSprites(uint16_t x, uint16_t y, uint16_t z, long long int number, int displayHighScore) {
    static const uint16_t textSpriteIndices[] = {32, 33, 34, 32, 6, 26, 27, 28, 29, 30, 31};
    uint32_t currentControlStructure = displayHighScore ? 255 : 220;

    int startIndex = displayHighScore ? 0 : 5;
    int textLength = displayHighScore ? 11 : 6;

    for (int i = startIndex; i < startIndex + textLength; ++i) {
        uint16_t spriteIndex = textSpriteIndices[i];
        drawSprite(x + ((i - startIndex) * 32), y, z, spriteIndex, MEDIUM_T, 0, currentControlStructure--);
    }

    x += textLength * 32;
    int length = numDigits(number);
    for (int i = 0; i < length; ++i) {
        int digit = getDigit(number, length - i - 1);
        uint16_t spriteIndex = (digit == 0) ? 25 : 16 + digit - 1;
        drawSprite(x + (i * 32), y, z, spriteIndex, MEDIUM_T, 0, currentControlStructure--);
    }
}
