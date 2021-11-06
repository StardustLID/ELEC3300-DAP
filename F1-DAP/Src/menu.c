#include "menu.h"
#include "stdio.h"
#include "string.h"

static void _createButton(uint16_t x, uint16_t y, const char* pStr, uint16_t usColor_Btn, uint16_t usColor_Text);
static void _refreshSong(uint8_t songIndex);
static void _clearLine(uint16_t usP);
static inline uint8_t nextSong(void);
static inline uint8_t chooseSong(void);

void MENU_Welcome() {
	LCD_Clear(0, 0, 240, 320, MAGENTA);
	LCD_DrawString(4, 80, "ELEC3300 Digital Audio Player");
	LCD_DrawString(88, 96, "Group 27");
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

void MENU_PlaySong() {
	// TODO: replace with icons (format tbc, with DMA)
	_createButton(68, SONG_NAME_USP, "-5s", CYAN, WHITE);
	_createButton(100, SONG_NAME_USP, "Pause", CYAN, WHITE);
	_createButton(148, SONG_NAME_USP, "+5s", CYAN, WHITE);
}

/* helper function */
// assume button has fixed size now
static void _createButton(uint16_t x, uint16_t y, const char* pStr, uint16_t usColor_Btn, uint16_t usColor_Text) {
	// button
	LCD_Clear(x, y, strlen(pStr)*WIDTH_EN_CHAR, BTN_HEIGHT, usColor_Btn);
	// text
	LCD_DrawString_Color(x, y + (BTN_HEIGHT - HEIGHT_EN_CHAR)/2, pStr, usColor_Btn, usColor_Text);
}

static void _refreshSong(uint8_t songIndex) {
	_clearLine(SONG_NAME_USP);
	char songName[15];
	sprintf(songName, "dummy song %d", songIndex);
	// LCD_DrawString(0, SONG_NAME_USP, songName);
	_createButton(0, SONG_NAME_USP, songName, MAGENTA, YELLOW);
}

static void _clearLine(uint16_t usP) {
	LCD_Clear(0, usP, 240, HEIGHT_EN_CHAR, WHITE);
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
