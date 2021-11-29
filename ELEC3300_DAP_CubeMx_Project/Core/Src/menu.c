#include <stdio.h>
#include <string.h>

#include "main.h"
#include "menu.h"
#include "player_func.h"
#include "switch.h"
#include "volume_bar.h"
#include "random.h"

#define WELCOME_DELAY 1000
#define INPUT_DELAY 80

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;

extern volatile uint8_t menu_id;
extern volatile uint8_t song_id;
extern uint8_t play_flag;
extern uint8_t bands[5];

uint32_t encoder_value = 0;
volatile uint16_t playtimeElapsed = 0; // in seconds
volatile uint8_t inPlayMenu = 0;

static void _createButton(uint16_t x, uint16_t y, const char* pStr, uint16_t usColor_Btn, uint16_t usColor_Text);
static void _renderButton(const Button *btn);
static void _formatSongItem(char* songItem, char** fileNames, uint8_t* fileTypes, uint8_t songId);
static void _formatBandItem(char* bandItem, uint8_t bandId, uint8_t bandVal);

void MENU_Welcome() {
	LCD_Clear(0, 0, 240, 320, DARK);
	LCD_DrawString_Color(4, 80, "ELEC3300 Digital Audio Player", DARK, CYAN);
	LCD_DrawString_Color(88, 96, "Group 27", DARK, CYAN);
	HAL_Delay(WELCOME_DELAY);
}

// main loop of the DAP program
void MENU_Main() {
	LCD_Clear(0, 0, 240, 320, DARK);
	btnFlagReset();

	LCD_DrawString_Color(0, 0, "Main Menu", DARK, CYAN);

	static char menuItem[][31] = {"1. Select song", "2. Set equalizer"};
	static uint8_t menuMap[2] = {1, 2};

	for (uint8_t i = 0; i < 2; i++) {
		LCD_DrawString_Color(0, HEIGHT_EN_CHAR*(i+1), menuItem[i], DARK, CYAN);
	}

	menu_id = 0;
	LCD_DrawString_Color(0, HEIGHT_EN_CHAR, menuItem[0], WHITE, BLUE);

	while (1) {
		if (btnFlag[2]) {
			HAL_Delay(INPUT_DELAY);
			btnFlag[2] = 0;
			menu_id = menuMap[menu_id];
			return;
		} else if (btnFlag[1]) {
			LCD_DrawString_Color(0, HEIGHT_EN_CHAR*(menu_id+1), menuItem[menu_id], DARK, CYAN);
			menu_id = (menu_id + 1) % 2;
			LCD_DrawString_Color(0, HEIGHT_EN_CHAR*(menu_id+1), menuItem[menu_id], WHITE, BLUE);
			HAL_Delay(INPUT_DELAY);
			btnFlag[1] = 0;
		}
	}
}

void MENU_SelectSong(uint8_t numSongs, char** fileNames, uint8_t* fileTypes) {
	LCD_Clear(0, 0, 240, 320, DARK);
	btnFlagReset();

	LCD_DrawString_Color(0, 0, "Song Menu", DARK, CYAN);
	LCD_DrawString_Color(0, 16, "Select a song:", DARK, CYAN);
	LCD_DrawString_Color(0, 48, "Song Name", DARK, CYAN);
	LCD_DrawString_Color(208, 48, "Type", DARK, CYAN);

	char songItem[31];

	for (uint8_t i = 0; i < numSongs; i++) {
		_formatSongItem(songItem, fileNames, fileTypes, i);
		LCD_DrawString_Color(0, 16*(i+4), songItem, DARK, CYAN);
	}

	song_id = 0;
	_formatSongItem(songItem, fileNames, fileTypes, 0);
	LCD_DrawString_Color(0, 64, songItem, WHITE, BLUE);

	// poll for button input
	while (1) {
		if (btnFlag[2] != 0) {
			HAL_Delay(INPUT_DELAY);
			btnFlag[2] = 0;
			menu_id = 3;
			return;
		} else if (btnFlag[1]) {
			_formatSongItem(songItem, fileNames, fileTypes, song_id);
			LCD_DrawString_Color(0, 16*(song_id+4), songItem, BLACK, CYAN);

			song_id = (song_id + 1) % numSongs;
			_formatSongItem(songItem, fileNames, fileTypes, song_id);
			LCD_DrawString_Color(0, 16*(song_id+4), songItem, WHITE, BLUE);
			HAL_Delay(INPUT_DELAY);
			btnFlag[1] = 0;
		}

		char temp[10];
		sprintf(temp, "%ld", switches[1].holdTime);
		LCD_DrawString_Color(0, 160, temp, DARK, CYAN);
		HAL_Delay(50);
		sprintf(temp, "%ld", switches[1].idleTime);
		LCD_DrawString_Color(0, 176, temp, DARK, CYAN);
		HAL_Delay(50);
	}
}

void MENU_PlaySong(uint8_t numSongs, char** fileNames, uint8_t* fileTypes) {
	LCD_Clear(0, 0, 240, 320, DARK);
	btnFlagReset();
	inPlayMenu = 1;

	LCD_DrawString_Color(40, 80, fileNames[song_id], DARK, CYAN);
	_renderButton(&btn_backward);
	_renderButton(&btn_play);
	_renderButton(&btn_forward);
	_renderButton(&btn_random);

	VOL_CreateVolBar();
	HAL_Delay(50);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
	if (!play_flag) {
		HAL_TIM_Base_Start_IT(&htim6);

		wav_read_header(fileNames[song_id]);
		wav_play_music(&hi2s3, &hi2c1, fileNames[song_id]);
	}
	
	while (1) {
		uint32_t enc_prev = encoder_value;
		encoder_value = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim5));
		
		if (encoder_value > enc_prev && volume < 92) {
			VOL_UpdateVolBar(volume, true);
			volume++;
			HAL_Delay(40);
		} else if (encoder_value < enc_prev && volume > 0) {
			VOL_UpdateVolBar(volume, false);
			volume--;
			HAL_Delay(40);
		}
	}
}

void MENU_Equalizer() {
	LCD_Clear(0, 0, 240, 320, DARK);
	btnFlagReset();

	LCD_DrawString_Color(0, 0, "Equalizer Menu", DARK, CYAN);
	LCD_DrawString_Color(0, HEIGHT_EN_CHAR, "Tune amplifier gain (+-12 dB):", DARK, CYAN);
	LCD_DrawString_Color(0, HEIGHT_EN_CHAR*3, "Band  Freq (Hz)  dB", DARK, CYAN);
	uint8_t bandId = 0;
	char bandItem[22];
	get_eeprom_eq(bands);

	for (uint8_t i = 0; i < 5; i++) {
		_formatBandItem(bandItem, i, bands[i]);
		LCD_DrawString_Color(0, HEIGHT_EN_CHAR*(i+4), bandItem, DARK, CYAN);
	}

	_formatBandItem(bandItem, 0, bands[0]);
	LCD_DrawString_Color(0, HEIGHT_EN_CHAR*4, bandItem, WHITE, BLUE);
	
	menu_id = 0;

	// poll for button input
	while (1) {
		if (btnFlag[2]) {
			btnFlag[2] = 0;

			// long press
			if (switches[2].holdTime > LONG_PRESS_DELAY) {
				equalizer_tuning(bands[0], bands[1], bands[2], bands[3], bands[4]);
				return;
			}

			if (bands[bandId] >= 24) continue;
			bands[bandId]++;
			_formatBandItem(bandItem, bandId, bands[bandId]);
			LCD_DrawString_Color(0, HEIGHT_EN_CHAR*(bandId+4), bandItem, WHITE, BLUE);
			HAL_Delay(INPUT_DELAY);
		} else if (btnFlag[1] == 1) {
			btnFlag[1] = 0;
			_formatBandItem(bandItem, bandId, bands[bandId]);
			LCD_DrawString_Color(0, HEIGHT_EN_CHAR*(bandId+4), bandItem, DARK, CYAN);
			bandId = (bandId + 1) % 5;
			_formatBandItem(bandItem, bandId, bands[bandId]);
			LCD_DrawString_Color(0, HEIGHT_EN_CHAR*(bandId+4), bandItem, WHITE, BLUE);
			HAL_Delay(INPUT_DELAY);
		} else if (btnFlag[0]) {
			btnFlag[0] = 0;
			
			// long press
			if (switches[0].holdTime > LONG_PRESS_DELAY) {
				return;
			}

			if (bands[bandId] <= 0) continue;
			bands[bandId]--;
			_formatBandItem(bandItem, bandId, bands[bandId]);
			LCD_DrawString_Color(0, HEIGHT_EN_CHAR*(bandId+4), bandItem, WHITE, BLUE);
			HAL_Delay(INPUT_DELAY);
		}
	}
}

void MENU_UpdatePlayTime(void) {
	char time_mmss[6];
	sprintf(time_mmss, "%02d:%02d", playtimeElapsed / 60, playtimeElapsed % 60);
	LCD_DrawString_Color(0, 0, time_mmss, DARK, CYAN);
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

static void _formatSongItem(char* songItem, char** fileNames, uint8_t* fileTypes, uint8_t songId) {
	char extension[6];
	if (fileTypes[songId] == 1) {
		strcpy(extension, "mp3");
	} else if (fileTypes[songId] == 2) {
		strcpy(extension, "wav");
	} else if (fileTypes[songId] == 3) {
		strcpy(extension, "flac");
	} else {
		strcpy(extension, "BUG");
	}
	sprintf(songItem, "%d. %-22s %s", songId, fileNames[songId], extension);
}

static void _formatBandItem(char* bandItem, uint8_t bandId, uint8_t bandVal) {
	sprintf(bandItem, "%-5d %-10s %-3d", bandId + 1, freqs[bandId], bandVal - 12); // display as -12 to 12, implicit 0 to 24
}
