#include "main.h"
#include "player_func.h"
#include "codec.h"
#include "mp3_decoder.h"
#include "wav_decoder.h"

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