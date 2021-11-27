#ifndef __MENU_H
#define	__MENU_H

#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "lcdtp.h"
#include "point.h"
#include "file_sys_func.h"

#define SONG_NAME_USP 240
#define BTN_HEIGHT 30

extern uint32_t encoder_value;
extern uint8_t volume;

typedef struct {
	const Point pos;
    const uint16_t height;
    const uint16_t width;
	const char* text;
	const uint16_t usColor_Btn;
	const uint16_t usColor_Text;
} Button;

static Button btn_PlaySongMenu = {
	.pos = {40, LCD_DispWindow_PAGE / 2},
    .height = HEIGHT_EN_CHAR * 3,
    .width = 88,
	.text = " Play Song ",
	.usColor_Btn = CYAN,
	.usColor_Text = WHITE
};

static Button btn_FatfsMenu = {
	.pos = {136, LCD_DispWindow_PAGE / 2},
    .height = HEIGHT_EN_CHAR * 3,
    .width = 56,
	.text = " FATFS ",
	.usColor_Btn = CYAN,
	.usColor_Text = WHITE
};

static Button btn_play = {
	.pos = {92, SONG_NAME_USP},
    .height = HEIGHT_EN_CHAR * 3,
    .width = 56,
	.text = " Start ",
	.usColor_Btn = CYAN,
	.usColor_Text = WHITE
};

static Button btn_backward = {
	.pos = {48, SONG_NAME_USP},
    .height = HEIGHT_EN_CHAR * 3,
    .width = 32,
	.text = " << ",
	.usColor_Btn = CYAN,
	.usColor_Text = WHITE
};

static Button btn_forward = {
	.pos = {168, SONG_NAME_USP},
    .height = HEIGHT_EN_CHAR * 3,
    .width = 32,
	.text = " >> ",
	.usColor_Btn = CYAN,
	.usColor_Text = WHITE
};

/**
 * entry point of the menu
 */
void MENU_Welcome(void);

/**
 * main loop of the DAP program
 */
void MENU_Main(void);

/**
 * @note the menu when a song is being selected
 * @return index of the chosen song
 */
uint8_t MENU_SelectSong(uint8_t numSongs, char** fileNames, uint8_t* fileTypes);

/**
 * @note the menu when a song is being played
 */
void MENU_PlaySong(void);

void MENU_UpdatePlayTime(void);

void MENU_SetSongTimer(TIM_HandleTypeDef *htim);

void MENU_incrementPlayTimeElapsed(void);

#endif 

