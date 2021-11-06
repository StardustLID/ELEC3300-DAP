#include "menu.h"
#include "stdio.h"

static void _refreshSong(uint8_t songIndex);
static void _clearLine(uint16_t usP);
static inline uint8_t nextSong(void);
static inline uint8_t chooseSong(void);

void MENU_Main() {
	LCD_DrawString(0, 0, "ELEC3300 Digital Audio Player");
	LCD_DrawString(0, 16, "Group 27");

	LCD_DrawString(0, 96, "Select Song");
}

uint8_t MENU_SelectSong() {
	// TODO: songIndex is dummy input. Actually it depends on the file read from FAT32
	uint8_t songIndex = 0;
	// display the current song only, better UI if time allows
	_refreshSong(songIndex);

	// poll for button input
	while (1) {
		if (nextSong()) {
			_refreshSong(++songIndex);
		}
		if (chooseSong()) {
			// TODO: clear the menu
			return songIndex;
		}
	}
}

/* helper function */
static void _refreshSong(uint8_t songIndex) {
	_clearLine(SONG_NAME_USP);
	char songName[15];
	sprintf(songName, "dummy song %d", songIndex);
	LCD_DrawString(0, SONG_NAME_USP, songName);
}

static void _clearLine(uint16_t usP) {
	LCD_DrawString(0, usP, "                              "); // 30 whitespace
}

/* buttons */
static inline uint8_t nextSong() {
	// dummy
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET; // PA0
}

static inline uint8_t chooseSong() {
	// dummy
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET; // PC13
}
