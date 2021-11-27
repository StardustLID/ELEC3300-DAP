#include "main.h"
#include "player_func.h"
#include "codec.h"
#include "mp3_player.h"
#include "wav_player.h"

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