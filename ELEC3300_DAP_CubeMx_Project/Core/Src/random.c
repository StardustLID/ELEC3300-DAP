#include "random.h"

uint32_t getRandomNumber() {
	uint32_t x = 0;
	HAL_RNG_GenerateRandomNumber(&hrng, &x);
	return x;
}

uint8_t getRandomSongId(uint8_t numSongs) {
	return getRandomNumber() % numSongs;
}
