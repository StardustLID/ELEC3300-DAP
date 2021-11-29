#ifndef __MENU_H
#define	__MENU_H

#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "lcdtp.h"
#include "point.h"
#include "file_sys_func.h"

#define SONG_NAME_USP 272
#define BTN_HEIGHT 30

extern uint32_t encoder_value;
extern uint8_t volume;
extern volatile uint16_t playtimeElapsed;
extern volatile uint8_t inPlayMenu;

typedef struct {
	const Point pos;
    const uint16_t height;
    const uint16_t width;
	const char* text;
} Button;

static Button btn_playpause = {
	.pos = {92, SONG_NAME_USP},
    .height = HEIGHT_EN_CHAR * 3,
    .width = 96,
	.text = " play/pause "
};

static Button btn_backward = {
	.pos = {48, SONG_NAME_USP},
    .height = HEIGHT_EN_CHAR * 3,
    .width = 32,
	.text = " << "
};

static Button btn_forward = {
	.pos = {200, SONG_NAME_USP},
    .height = HEIGHT_EN_CHAR * 3,
    .width = 32,
	.text = " >> "
};

static char freqs[][5] = {"100", "300", "875", "2400", "6900"};

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
 */
void MENU_SelectSong(uint8_t numSongs, char** fileNames, uint8_t* fileTypes);

/**
 * @note the menu when a song is being played
 */
void MENU_PlaySong(uint8_t numSongs, char** fileNames, uint8_t* fileTypes);

void MENU_Equalizer(void);
void MENU_UpdatePlayTime(void);
void MENU_FlashButton(const Button *btn);

#endif 
