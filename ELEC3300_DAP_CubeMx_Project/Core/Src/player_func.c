#include "main.h"
#include "player_func.h"
#include "file_sys_func.h"
#include "codec.h"
#include "mp3_decoder.h"
#include "wav_decoder.h"
#include "ff.h"
#include "lcdtp.h"
#include "eeprom.h"

void play_song(){
	HAL_I2S_DMAResume(&hi2s3);
	update_play_flag(1);
}

void pause_song(){
	HAL_I2S_DMAPause(&hi2s3);
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
void equalizer_tuning(uint8_t band1, uint8_t band2, uint8_t band3, uint8_t band4, uint8_t band5){
	codec_eq(band1, band2, band3, band4, band5);
}

void player_display_cover(char* file_name){
	char path[sizeof("0:/MUSIC_COVER/") + _MAX_LFN] = {0};
	
	strcat(path, "0:/MUSIC_COVER/");
	strcat(path, file_name);

	char ReadBuffer[2] = {0};
	f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);

	for(int i = 0; i < 200; i++){
		for(int j = 0; j < 200; j++){
			f_read(&myFILE, &ReadBuffer, 2, &fnum);
			LCD_DrawDot(25+j,10+i, (uint16_t)((ReadBuffer[1] << 8)| (ReadBuffer[0])));
		}
	}
	f_close(&myFILE);
}

uint8_t write_song_to_play_list(uint8_t song, uint8_t tar_play_list){
	//eeprom_earse_all();
	
	uint8_t num = get_eeprom_num_of_playlist();
	volatile uint16_t address = get_eeprom_num_of_playlist_address();
	uint8_t last_play_list = 0;
	uint8_t data = 0;
	uint16_t address_for_modify = 0;
	uint16_t last_address_for_playlist = 0;
	uint8_t playlist_address = 0;
	uint8_t playlist_size = 0;
	
	
	if(tar_play_list > num){
		num++;
		eeprom_write(&hi2c2,EEPROM_NUM_OF_PLAYLIST, &num);
		HAL_Delay(2);
	}
	
	address++;//move to first playlist
	
	while(address < 256){
		HAL_I2C_Mem_Read(&hi2c2,EEPROM_DEVICE_ADDRESS,address, 1, &data, 1, 50);
		HAL_Delay(2);
		if(data == 0x00 && last_address_for_playlist == 0){
			last_address_for_playlist = address;
			break;
		}
		
		uint8_t playlist = (data & 0xE0) >> 5;
		uint8_t tmpe_size_temp = data & 0x1F;
		
		if(playlist != tar_play_list){
			if(playlist == 0 || playlist == 0x7){
				data |= (tar_play_list << 5);
			}
		}
		else{
			playlist_address = address;
			playlist_size = tmpe_size_temp;
			address_for_modify = address + (tmpe_size_temp + 1);
			//address = address_for_modify;
			tmpe_size_temp += 1;
			if(tmpe_size_temp > 63)
				return 0;
		}
		address += (playlist_size + 1);
	}

	if(address_for_modify == 0)
			address_for_modify = last_address_for_playlist + 1;
	if(playlist_address == 0)
		playlist_address = last_address_for_playlist;
	
	data = 0;
	data |=  (tar_play_list << 5);
	data += playlist_size + 1;
	
	/*
	char string[10] = {0};
	sprintf(string, "data: %x", data);
	LCD_DrawString(0,100, string);
	*/
	
	HAL_I2C_Mem_Write(&hi2c2, EEPROM_DEVICE_ADDRESS, playlist_address, 1, &data,1,10);
	HAL_Delay(2);
	
	uint8_t* temp_list = (uint8_t*)malloc((last_address_for_playlist - address_for_modify)*sizeof(uint8_t));
	for(int i = 0; i < last_address_for_playlist - address_for_modify; i++){
		HAL_I2C_Mem_Read(&hi2c2,EEPROM_DEVICE_ADDRESS, address_for_modify + i, 1, temp_list + i, 1, 50);
		HAL_Delay(2);
	}

	HAL_I2C_Mem_Write(&hi2c2,EEPROM_DEVICE_ADDRESS, address_for_modify, 1, &song, 1, 50);
	HAL_Delay(2);
	
	
	uint8_t data_temp = 0;
	
	HAL_I2C_Mem_Read(&hi2c2,EEPROM_DEVICE_ADDRESS, address_for_modify - 1, 1, &data_temp, 1, 50);
	HAL_Delay(2);
	
	/*
	sprintf(string, "data: %x", data_temp);
	LCD_DrawString(0,0, string);
	
	HAL_I2C_Mem_Read(&hi2c2,EEPROM_DEVICE_ADDRESS, address_for_modify, 1, &data_temp, 1, 50);
	HAL_Delay(2);
	
	sprintf(string, "data: %x", data_temp);
	LCD_DrawString(0,20, string);

	sprintf(string, "last: %x", last_address_for_playlist);
	LCD_DrawString(0,40, string);
	
	sprintf(string, "mod: %x", address_for_modify);
	LCD_DrawString(0,60, string);
	
	sprintf(string, "playlist_address: %x", playlist_address);
	LCD_DrawString(0,80, string);
	*/
	
	
	
	for(int i = 1; i < last_address_for_playlist - address_for_modify; i++){
		HAL_I2C_Mem_Write(&hi2c2,EEPROM_DEVICE_ADDRESS, address_for_modify + i, 1, temp_list + i - 1, 1, 50);
		/*
		sprintf(string, "data: %x", *(temp_list + i));
		LCD_DrawString(0,120 + i*20, string);
		*/
		HAL_Delay(2);
	}
	return 1;
}

uint8_t* read_playlist(uint8_t playlist_num, uint8_t* size){
	volatile uint16_t address = get_eeprom_num_of_playlist_address() + 1;
	uint8_t* temp_list;
	uint8_t while_flag = 0;
	uint8_t found_flag = 0;
	uint8_t data = 0;
	uint8_t num_song = 0;
	*size = 0;
	
	while(1){
		HAL_I2C_Mem_Read(&hi2c2, EEPROM_DEVICE_ADDRESS, address, 1, &data, 1, 10);
		HAL_Delay(2);
		uint8_t playlist = (data & 0xE0) >> 5;
		uint8_t size_temp = data & 0x1F;
		
		if(playlist == playlist_num && found_flag == 0){
			found_flag = 1;
			temp_list = (uint8_t*)malloc(size_temp*sizeof(uint8_t));
			*size = size_temp;
			address++;
		}
		else if(found_flag == 1){
			*(temp_list + num_song) = data;
			num_song++;
			if(num_song >= *size){
				return temp_list;
			}
			address++;
		}
		else if(found_flag == 0){
			address += size_temp + 1;
		}
		else if(data == 0){
			return 0;
		}
	}
}
