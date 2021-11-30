#include "mp3common.h"
#include "main.h"
#include "fatfs.h"
#include "lcdtp.h"
#include "menu.h"
#include <string.h>
#include "wav_decoder.h"
#include "codec.h"
#include "file_sys_func.h"

wav_tag_header wav_tag = {0};
uint8_t wav_play_flag = 0;
uint16_t wav_buf_pos = 0;

uint8_t wav_time_skip_flag = 0;
int32_t wav_skip_byte = 0;

void wav_read_header(char* file_name){
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	char string[1024] = {0};
	
	strcat(path, "0:/MUSIC/");
	strcat(path, file_name);

	char ReadBuffer[20] = {0};
	f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);
	
	//RIFF section
	f_lseek(&myFILE, 4); // skip "RIFF"
	f_read(&myFILE, ReadBuffer, 4, &fnum);
	wav_tag.file_size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
	
	f_lseek(&myFILE, f_tell(&myFILE)+8 ); // skip "WAVE", "fmt "
	f_read(&myFILE, ReadBuffer, 4, &fnum);
	wav_tag.fmt_chunk.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
	
	f_read(&myFILE, ReadBuffer, wav_tag.fmt_chunk.size, &fnum);
	wav_tag.fmt_chunk.codec_fmt = (ReadBuffer[0] & 0xff);
	wav_tag.fmt_chunk.sound_ch = (ReadBuffer[2] & 0xff);
	wav_tag.fmt_chunk.sample_rate = (ReadBuffer[4] & 0xff) | (ReadBuffer[5] & 0xff) << 8 |  (ReadBuffer[6] & 0xff) << 16 | (ReadBuffer[7] & 0xff) << 24; 
	wav_tag.fmt_chunk.byte_rate = (ReadBuffer[8] & 0xff) | (ReadBuffer[9] & 0xff) << 8 |  (ReadBuffer[10] & 0xff) << 16 | (ReadBuffer[11] & 0xff) << 24;
	wav_tag.fmt_chunk.block_align =  (ReadBuffer[12] & 0xff) | (ReadBuffer[13] & 0xff) << 8 ;
	wav_tag.fmt_chunk.bit_per_sample = (ReadBuffer[14] & 0xff) | (ReadBuffer[15] & 0xff) << 8 ;
	
	if(wav_tag.fmt_chunk.bit_per_sample % 2){
		//if bit_per_sample is odd number, file will pad 1byte '0'
		f_lseek(&myFILE, f_tell(&myFILE)+1); // skip the 1 byte '0'
	}
	
	/*
	sprintf(string, "codec_fmt: %d", wav_tag.fmt_chunk.codec_fmt);
	LCD_DrawString(0,130,string);
	sprintf(string, "sound_ch: %d", wav_tag.fmt_chunk.sound_ch);
	LCD_DrawString(0,150,string);
	sprintf(string, "sample_rate: %d", wav_tag.fmt_chunk.sample_rate);
	LCD_DrawString(0,170,string);
	sprintf(string, "byte_rate: %d", wav_tag.fmt_chunk.byte_rate);
	LCD_DrawString(0,190,string);
	sprintf(string, "block_align: %d", wav_tag.fmt_chunk.block_align);
	LCD_DrawString(0,210,string);
	sprintf(string, "bit_per_sample: %d", wav_tag.fmt_chunk.bit_per_sample);
	LCD_DrawString(0,230,string);
	*/
	/*
	while(!f_eof(&myFILE)){
		f_read(&myFILE, ReadBuffer, 4, &fnum);
		*(ReadBuffer + 4) = '\0';
		
		if(strcmp(ReadBuffer, WAV_ID_DATA) == 0){
			f_read(&myFILE, ReadBuffer, 4, &fnum);
			wav_tag.data_chunk.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
			wav_tag.data_chunk.data_offest = f_tell(&myFILE); // the read pointer now is pointed to the audio data
			if(wav_tag.data_chunk.size % 2){
				//if wav_tag.data_chunk.size is odd number, file will pad 1byte '0'
				f_lseek(&myFILE, f_tell(&myFILE)+1); // skip the 1 byte '0'
			}
		}
		
		else if(strcmp(ReadBuffer, WAV_ID_FACT) == 0){
			f_read(&myFILE, ReadBuffer, 4, &fnum);
			wav_tag.fact_chunk.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
			f_lseek(&myFILE, f_tell(&myFILE) + wav_tag.fact_chunk.size);//skip whole fact chunk
			
			if(wav_tag.fact_chunk.size % 2){
				//if wav_tag.fact_chunk.size is odd number, file will pad 1byte '0'
				f_lseek(&myFILE, f_tell(&myFILE)+1); // skip the 1 byte '0'
			}
		}
		
		else if(strcmp(ReadBuffer, WAV_ID_LIST) == 0){
			f_read(&myFILE, ReadBuffer, 4, &fnum);
			wav_tag.list_chunk.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
			
			char ID[5] = {0};
			uint32_t size = 0;
			int32_t num_data = 4;
			if(wav_tag.list_chunk.size % 2){
				num_data = 3;
			}
			f_lseek(&myFILE, f_tell(&myFILE)+4); // skip "INFO"
			while(num_data != wav_tag.list_chunk.size){ // while condition can be strcmp(buf, "INFO") or the sub-chunk size
				f_read(&myFILE, ReadBuffer, 8, &fnum); // read the sub-chunk ID, size
				strncpy(ID, ReadBuffer, 4);
				size = (ReadBuffer[4] & 0xff) | (ReadBuffer[5] & 0xff) << 8 |  (ReadBuffer[6] & 0xff) << 16 | (ReadBuffer[7] & 0xff) << 24;
				num_data += (size + 8);
				// do if-else to check what is the type of sub-chunk
				// read the size
				// if sub_chunk's size is odd number, file will pad 1 byte '0', move the read pointer by 1

				// if not is the thing listed on .h define, skip it
				if(strcmp(ID, WAV_INFO_ID_ARTIST) == 0){
					f_read(&myFILE, ReadBuffer, size, &fnum); 
					strncpy(wav_tag.list_chunk.artist, ReadBuffer, size);
				}
				else if(strcmp(ID, WAV_INFO_ID_TITLE) == 0){
					f_read(&myFILE, ReadBuffer, size, &fnum); 
					strncpy(wav_tag.list_chunk.title, ReadBuffer, size);
				}
				else if(strcmp(ID, WAV_INFO_ID_ALBUM) == 0){
					f_read(&myFILE, ReadBuffer, size, &fnum); 
					strncpy(wav_tag.list_chunk.album, ReadBuffer, size);
				}
				else if(strcmp(ID, WAV_INFO_ID_DATE) == 0){
					f_read(&myFILE, ReadBuffer, size, &fnum); 
					strncpy(wav_tag.list_chunk.Date, ReadBuffer, size);
				}
				else if(strcmp(ID, WAV_INFO_ID_GENRE) == 0){
					f_read(&myFILE, ReadBuffer, size, &fnum); 
					strncpy(wav_tag.list_chunk.genre, ReadBuffer, size);
				}
				else{
					f_lseek(&myFILE, f_tell(&myFILE)+size);
				}
				
				if(size % 2){
					//if size is odd number, file will pad 1byte '0'
					f_lseek(&myFILE, f_tell(&myFILE)+1); // skip the 1 byte '0'
					num_data++;
				}
			}//while
		}
		
		else if(strcpy(ReadBuffer, WAV_ID_END_OF_LIST) == 0){
			f_read(&myFILE, ReadBuffer, 4, &fnum);
			wav_tag.wav_pic.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
			f_read(&myFILE, ReadBuffer, 1, &fnum);
			wav_tag.wav_pic.encode_code = ReadBuffer[0];
			f_lseek(&myFILE, f_tell(&myFILE) + 2);
			wav_tag.wav_pic.data_size = wav_tag.wav_pic.size - 3;
			while(ReadBuffer[0] != '0'){
				f_read(&myFILE, ReadBuffer, 1, &fnum);
				wav_tag.wav_pic.data_size--;
			}
			wav_tag.wav_pic.data = malloc(wav_tag.wav_pic.data_size);
			if(wav_tag.wav_pic.data != 0){
				f_read(&myFILE, wav_tag.wav_pic.data, wav_tag.wav_pic.data_size, &fnum);
			}
		}
		else{
			f_read(&myFILE, ReadBuffer, 4, &fnum);
			uint32_t size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
			if(size % 2) size += 1;
			f_lseek(&myFILE, f_tell(&myFILE) + size);
		}
		
	}
	*/
	/*
	sprintf(string, "artist: %s", wav_tag.list_chunk.artist);
	LCD_DrawString(0,120,string);
	sprintf(string, "title: %s", wav_tag.list_chunk.title);
	LCD_DrawString(0,140,string);
	sprintf(string, "album: %s", wav_tag.list_chunk.album);
	LCD_DrawString(0,160,string);
	sprintf(string, "Date: %s", wav_tag.list_chunk.Date);
	LCD_DrawString(0,180,string);
	sprintf(string, "genre: %s", wav_tag.list_chunk.genre);
	LCD_DrawString(0,200,string);
	sprintf(string, "buffer: %s", ReadBuffer);
	LCD_DrawString(0,220,string);
	sprintf(string, "sound_ch: %d", wav_tag.fmt_chunk.sound_ch);
	LCD_DrawString(0,240,string);
	sprintf(string, "sample_rate: %d Hz", wav_tag.fmt_chunk.sample_rate);
	LCD_DrawString(0,260,string);
	sprintf(string, "bit_per_sample: %d", wav_tag.fmt_chunk.bit_per_sample);
	LCD_DrawString(0,280,string);
	//sprintf(string, "PCM: %d", wav_tag.fmt_chunk.codec_fmt);
	//LCD_DrawString(0,300,string);
	*/
	
	
	f_close(&myFILE);
}

void wav_play_music(I2S_HandleTypeDef *hi2s, I2C_HandleTypeDef *hi2c, char *file_name) {
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	// char string[128] = {0};

	strcat(path, "0:/MUSIC/");
	strcat(path, file_name);

	wav_play_flag = 1;

	f_open(&myFILE, path, FA_READ | FA_OPEN_EXISTING);

	if (res != FR_OK) {
		// cannot open file
		return;
	}

	f_lseek(&myFILE, wav_get_data_offest());  // jump to the music data
	f_read(&myFILE, codec_out_buffer, AUDIO_BUFFER_SIZE, &fnum);
	//f_read(&myFILE, codec_out_buffer + AUDIO_HALF_BUFFER_SIZE, AUDIO_HALF_BUFFER_SIZE, &fnum);
	//f_read(&myFILE, codec_out_buffer + AUDIO_HALF_BUFFER_SIZE, AUDIO_BUFFER_SIZE, &fnum);
	
	wav_play_flag = 1;
	update_play_flag(wav_play_flag);
	
	coded_i2s_set_up(hi2s, hi2c, wav_get_sample_rate(), wav_get_bit_per_sample());
	HAL_I2S_Transmit_DMA(hi2s, codec_out_buffer, AUDIO_BUFFER_SIZE);
	
	// sprintf(string, "ori: %d  ", wav_tag.data_chunk.data_offest);
	// LCD_DrawString(0,0,string);

	while (!(f_eof(&myFILE))) {
		if (file_read_flag) {
			// HAL_I2S_Transmit_DMA(hi2s, &(buffer[AUDIO_HALF_BUFFER_SIZE - wav_buf_pos]), AUDIO_BUFFER_SIZE);
			if(wav_time_skip_flag == 1){
				uint32_t cur_read_offset = f_tell(&myFILE);
				uint32_t tar_read_offset = cur_read_offset + wav_skip_byte;
				
				// sprintf(string, "cur: %d  ", cur_read_offset);
				// LCD_DrawString(0,20,string);
				
				// sprintf(string, "tar: %d  ", tar_read_offset);
				// LCD_DrawString(0,40,string);
				
				if(tar_read_offset <= wav_tag.data_chunk.data_offest){
					tar_read_offset = wav_tag.data_chunk.data_offest;
				}
				else if(tar_read_offset >= cur_read_offset + wav_tag.data_chunk.data_offest + wav_tag.data_chunk.size - AUDIO_BUFFER_SIZE){
					wav_time_skip_flag = 0;
					break;
				}
				
				f_lseek(&myFILE, tar_read_offset);
				wav_time_skip_flag = 0;
			}
			
			if(wav_tag.fmt_chunk.sound_ch == 1){
				f_read(&myFILE, codec_out_buffer + wav_buf_pos, AUDIO_HALF_BUFFER_SIZE, &fnum);	// TODO: put it DMA cplt callback, but not working for now
				for(int  i = AUDIO_HALF_BUFFER_SIZE - 1; i >= 0; i--){
					codec_out_buffer[wav_buf_pos + i * 2] = codec_out_buffer[wav_buf_pos + i];
					codec_out_buffer[wav_buf_pos + i * 2 + 1] = codec_out_buffer[wav_buf_pos + i];
				}	
			}
			else
				f_read(&myFILE, codec_out_buffer + wav_buf_pos, AUDIO_BUFFER_SIZE, &fnum);	// TODO: put it DMA cplt callback, but not working for now
			// HAL_I2S_Transmit(hi2s, buffer[buffer_flag], AUDIO_BUFFER_SIZE, 1000000);
			//HAL_I2S_Transmit_DMA(hi2s, codec_out_buffer, AUDIO_BUFFER_SIZE);
			file_read_flag = 0;
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		}
		
		uint32_t enc_prev = encoder_value;
		encoder_value = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim5));
		
		if (encoder_value > enc_prev && volume < 100) {
			VOL_UpdateVolBar(volume, true);
			volume++;
		} else if (encoder_value < enc_prev && volume > 0) {
			VOL_UpdateVolBar(volume, false);
			volume--;
		}
		codec_volume_update(&hi2c1, volume + 60);
	}
	HAL_I2S_DMAStop(hi2s);
	f_close(&myFILE);
	wav_play_flag = 0;
	update_play_flag(wav_play_flag);
}

void wav_time_skip(int32_t sec){
	wav_skip_byte = wav_tag.fmt_chunk.byte_rate * sec;
	wav_time_skip_flag = 1;
}

uint32_t wav_get_file_size(){
	return wav_tag.file_size + 8;
}

uint32_t wav_get_sample_rate(){
	return wav_tag.fmt_chunk.sample_rate;
}

uint16_t wav_get_bit_per_sample(){
	return wav_tag.fmt_chunk.bit_per_sample;
}

uint32_t wav_get_data_offest(){
	return wav_tag.data_chunk.data_offest;
}

uint8_t get_wav_play_flag(){
	return wav_play_flag;
}

void wav_buf_pos_update(uint16_t pos){
	wav_buf_pos = pos;
}
