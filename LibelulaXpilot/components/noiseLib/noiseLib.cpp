#include <bootloader_random.h>
#include "noiseLib.hpp"

static int randomBuffer[256];
static unsigned int listPointer = 0;

float getWhiteNoiseNumber(int maxLimit)
{
    int number = randomBuffer[listPointer];
    listPointer++;
    return (float)(number % maxLimit) / 100;
}

void randomInit()
{
    bootloader_random_enable();
    bootloader_fill_random(&randomBuffer, sizeof(randomBuffer));
}