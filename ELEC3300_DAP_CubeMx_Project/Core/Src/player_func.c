#include "main.h"
#include "player_func.h"
#include "file_sys_func.h"
#include "codec.h"
#include "mp3_decoder.h"
#include "wav_decoder.h"
#include "ff.h"
#include "lcdtp.h"

void play_song(){
	HAL_I2S_DMAResume(&hi2s3);
	update_play_flag(1);
}

void pause_song(){
	HAL_I2S_DMAResume(&hi2s3);
	update_play_flag(0);
}

void song_forback_ward(int32_t sec){
	if(get_wav_play_flag()){
		wav_time_skip(sec);
	}
}

void equalizer_enable(uint8_t enable_flag){
	codec_eq_enable(enable_flag);
}

/*band value should between [0:24], min = 0. max = 24*/
void equalizer_tuning(int8_t band1, uint8_t band2, uint8_t band3, uint8_t band4, uint8_t band5){
	codec_eq(band1, band2, band3, band4, band5);
}

void player_display_cover(char* file_name){
	char path[sizeof("0:/MUSIC_COVER/") + _MAX_LFN] = {0};
	
	strcat(path, "0:/MUSIC_COVER/");
	strcat(path, file_name);

	char ReadBuffer[2] = {0};
	f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);

	for(int i = 0; i < 128; i++){
		for(int j = 0; j < 128; j++){
			f_read(&myFILE, &ReadBuffer, 2, &fnum);
			LCD_DrawDot(10+j,10+i, (uint16_t)((ReadBuffer[1] << 8)| (ReadBuffer[0])));
		}
	}
	f_close(&myFILE);
}