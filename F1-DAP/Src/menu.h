#ifndef __MENU_H
#define	__MENU_H

#include "stm32f1xx_hal.h"
#include "lcd.h"

#define SONG_NAME_USP 240

/**
 * entry point of the menu
 */
void MENU_Main(void);

/**
 * @note the menu when a song is being played
 * @return index of the chosen song
 */
uint8_t MENU_SelectSong(void);

#endif 

