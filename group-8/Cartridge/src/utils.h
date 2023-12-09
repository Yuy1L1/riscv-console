#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define LCG_A 1664525
#define LCG_C 1013904223
#define LCG_M 4294967296 // 2^32

void seedPRNG(uint32_t seed);
uint32_t prng();
int numDigits(long long int number);
int getDigit(long long int number, int digitIndex);
void displayNumberUsingSprites(uint16_t x, uint16_t y, uint16_t z, long long int number, int displayHighScore);

#endif // UTILS_H
