#include <stdio.h>
#include <string.h>

#include "main.h"
#include "menu.h"
#include "volume_bar.h"
#include "random.h"

#define WELCOME_DELAY 1000
#define INPUT_DELAY 80

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;

extern volatile uint8_t menu_id;
extern volatile uint8_t song_id;
extern volatile uint8_t btn_0_flag;
extern volatile uint8_t btn_1_flag;
extern volatile uint8_t btn_2_flag;
extern uint8_t play_flag;

extern volatile uint16_t playtimeElapsed;

static void _createButton(uint16_t x, uint16_t y, const char* pStr, uint16_t usColor_Btn, uint16_t usColor_Text);
static void _renderButton(const Button *btn);
static void _formatSongItem(char* songItem, char** fileNames, uint8_t* fileTypes, uint8_t songId);


void MENU_Welcome() {
	LCD_Clear(0, 0, 240, 320, DARK);
	LCD_DrawString_Color(4, 80, "ELEC3300 Digital Audio Player", DARK, CYAN);
	LCD_DrawString_Color(88, 96, "Group 27", DARK, CYAN);
	HAL_Delay(WELCOME_DELAY);
}

// main loop of the DAP program
void MENU_Main() {
	LCD_Clear(0, 0, 240, 320, DARK);
	btn_0_flag = 0;
	btn_1_flag = 0;
	btn_2_flag = 0;
	_renderButton(&btn_PlaySongMenu);
	_renderButton(&btn_FatfsMenu);

	while (1) {
		if (btn_2_flag != 0) {
			btn_2_flag = 0;
		} else if (btn_1_flag) {
			btn_1_flag = 0;
			menu_id = 1;
			return;
		}
	}
}

void MENU_SelectSong(uint8_t numSongs, char** fileNames, uint8_t* fileTypes) {
	LCD_Clear(0, 0, 240, 320, DARK);
	btn_0_flag = 0;
	btn_1_flag = 0;
	btn_2_flag = 0;

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
		if (btn_2_flag != 0) {
			btn_2_flag = 0;
			menu_id = 2; // set menu id
			HAL_Delay(INPUT_DELAY);
			return; // select current song
		} else if (btn_1_flag) {
			_formatSongItem(songItem, fileNames, fileTypes, song_id);
			LCD_DrawString_Color(0, 16*(song_id+4), songItem, BLACK, CYAN);

			song_id = (song_id + 1) % numSongs;
			_formatSongItem(songItem, fileNames, fileTypes, song_id);
			LCD_DrawString_Color(0, 16*(song_id+4), songItem, WHITE, BLUE);
			HAL_Delay(INPUT_DELAY);
			btn_1_flag = 0;
		}
	}
}

void MENU_PlaySong(uint8_t numSongs, char** fileNames, uint8_t* fileTypes) {
	LCD_Clear(0, 0, 240, 320, DARK);
	btn_0_flag = 0;
	btn_1_flag = 0;
	btn_2_flag = 0;

	LCD_DrawString_Color(40, 80, fileNames[song_id], DARK, CYAN);
	_renderButton(&btn_backward);
	_renderButton(&btn_play);
	_renderButton(&btn_forward);
	_renderButton(&btn_random);

	VOL_CreateVolBar();
	HAL_Delay(100);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
	// if (!play_flag) {
	// 	HAL_TIM_Base_Start_IT(&htim6);

	// 	wav_read_header(fileNames[song_id]);
	// 	wav_play_music(&hi2s3, &hi2c1, fileNames[song_id]);
	// }
	
	while (1) {
	// 	if ( ucXPT2046_TouchFlag == 1 ) {
	// 		strType_XPT2046_Coordinate tempCoor = getTouchedPoint();
	// 		const Point touchPt = {tempCoor.x, tempCoor.y};
	// 		if (_btnTouched(&touchPt, &btn_play)) {
	// 			if (play_flag == 0) HAL_TIM_Base_Start_IT(songTimer);
	// 			else HAL_TIM_Base_Stop_IT(songTimer);
	// 			play_flag = !play_flag;
	// 		} else if (_btnTouched(&touchPt, &btn_forward)) {
	// 			playtimeElapsed += 5;
	// 		} else if (_btnTouched(&touchPt, &btn_backward)) {
	// 			if (playtimeElapsed < 5) playtimeElapsed = 0; // avoid underflow
	// 			else playtimeElapsed -= 5;
	// 		}
	// 		ucXPT2046_TouchFlag = 0;
	// 		HAL_Delay(INPUT_DELAY);
	// 	}
		MENU_UpdatePlayTime();

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
		
		if (btn_2_flag) {
			btn_2_flag = 0;
			codec_play_pause();
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

