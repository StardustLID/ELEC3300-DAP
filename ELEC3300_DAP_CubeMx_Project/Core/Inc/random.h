#ifndef __RANDOM_H
#define	__RANDOM_H

#include "stm32f4xx_hal.h"

extern RNG_HandleTypeDef hrng;

uint32_t getRandomNumber(void);

uint8_t getRandomSongId(uint8_t numSongs);

#endif
