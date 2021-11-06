#ifndef __MENU_H
#define	__MENU_H

#include "stm32f1xx_hal.h"
#include "lcdtp.h"

#define SONG_NAME_USP 240
#define BTN_HEIGHT 30

/**
 * entry point of the menu
 */
void MENU_Welocme(void);

/**
 * @note the menu when a song is being selected
 * @return index of the chosen song
 */
uint8_t MENU_SelectSong(void);

/**
 * @note the menu when a song is being played
 */
void MENU_PlaySong(void);

#endif 

