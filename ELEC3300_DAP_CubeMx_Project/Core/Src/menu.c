#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "xpt2046.h"

#define WELCOME_DELAY 1000
#define INPUT_DELAY 80

static void _createButton(uint16_t x, uint16_t y, const char* pStr, uint16_t usColor_Btn, uint16_t usColor_Text);
static void _renderButton(const Button *btn);
static void _refreshSong(uint8_t songIndex);
static void _clearLine(uint16_t usP);
static inline bool _btnTouched(const Point *touchPt, const Button *btn);

static TIM_HandleTypeDef *songTimer;

static bool playFlag = false;
static uint16_t playtimeElapsed = 0;

void MENU_Welcome() {
	LCD_Clear(0, 0, 240, 320, MAGENTA);
	LCD_DrawString(4, 80, "ELEC3300 Digital Audio Player");
	LCD_DrawString(88, 96, "Group 27");
	HAL_Delay(WELCOME_DELAY);
}

// main loop of the DAP program
void MENU_Main() {
	while (1) {
		LCD_Clear(0, 0, 240, 320, DARK);
		_renderButton(&btn_PlaySongMenu);
		_renderButton(&btn_FatfsMenu);
		while (1) {
			if (ucXPT2046_TouchFlag == 1) {
				strType_XPT2046_Coordinate tempCoor = getTouchedPoint();
				const Point touchPt = {tempCoor.x, tempCoor.y};
				if (_btnTouched(&touchPt, &btn_PlaySongMenu)) MENU_PlaySong();
				ucXPT2046_TouchFlag = 0;
			}
		}
	}
}

uint8_t MENU_SelectSong() {
	// TODO: songIndex is dummy input. Actually it depends on the file read from FAT32
	uint8_t songIndex = 0;
	// display the current song only, better UI if time allows
	_refreshSong(songIndex);

	// poll for button input
	while (1) {
		// if (nextSong()) {
		// 	_refreshSong(++songIndex);
		// }
		// if (chooseSong()) {
		// 	return songIndex;
		// }
	}
}

void MENU_PlaySong() {
	// TODO: replace with icons (format tbc, with DMA)
	LCD_Clear(0, 0, 240, 320, DARK);
	LCD_DrawString_Color(40, 80, "Now Playing: Dummy Song", DARK, CYAN);
	_renderButton(&btn_backward);
	_renderButton(&btn_play);
	_renderButton(&btn_forward);
	
	while (1) {
		if ( ucXPT2046_TouchFlag == 1 ) {
			strType_XPT2046_Coordinate tempCoor = getTouchedPoint();
			const Point touchPt = {tempCoor.x, tempCoor.y};
			if (_btnTouched(&touchPt, &btn_play)) {
				if (playFlag == 0) HAL_TIM_Base_Start_IT(songTimer);
				else HAL_TIM_Base_Stop_IT(songTimer);
				playFlag = !playFlag;
			} else if (_btnTouched(&touchPt, &btn_forward)) {
				playtimeElapsed += 5;
			} else if (_btnTouched(&touchPt, &btn_backward)) {
				if (playtimeElapsed < 5) playtimeElapsed = 0; // avoid underflow
				else playtimeElapsed -= 5;
			}
			ucXPT2046_TouchFlag = 0;
			MENU_UpdatePlayTime();
			HAL_Delay(INPUT_DELAY);
		}
	}
}

void MENU_UpdatePlayTime(void) {
	char time_mmss[6];
	sprintf(time_mmss, "%02d:%02d", playtimeElapsed / 60, playtimeElapsed % 60);
	LCD_DrawString_Color(0, 0, time_mmss, GREEN, BLUE);
}

void MENU_SetSongTimer(TIM_HandleTypeDef *htim) {
	songTimer = htim;
}

void MENU_incrementPlayTimeElapsed(void) {
	playtimeElapsed++;
}

/* helper function */
// assume button has fixed size now
static void _createButton(uint16_t x, uint16_t y, const char* pStr, uint16_t usColor_Btn, uint16_t usColor_Text) {
	// button
	LCD_Clear(x, y, strlen(pStr)*WIDTH_EN_CHAR, BTN_HEIGHT, usColor_Btn);
	// text
	LCD_DrawString_Color(x, y + (BTN_HEIGHT - HEIGHT_EN_CHAR)/2, pStr, usColor_Btn, usColor_Text);
}

static void _renderButton(const Button *btn) {
	_createButton(btn->pos.x, btn->pos.y, btn->text, btn->usColor_Btn, btn->usColor_Text);
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

static inline bool _btnTouched(const Point *touchPt, const Button *btn) {
	return touchPt->x > btn->pos.x && touchPt->x < btn->pos.x + btn->width && touchPt->y > btn->pos.y && touchPt->y < btn->pos.y + btn->height;
}
