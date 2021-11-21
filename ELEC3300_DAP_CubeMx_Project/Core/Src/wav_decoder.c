#include "mp3common.h"
#include "main.h"
#include "fatfs.h"
#include "lcd.h"
#include <string.h>
#include "wav_decoder.h"
#include "codec.h"
#include "file_sys_func.h"

wav_tag_header wav_tag = {0};
uint8_t wav_play_flag = 0;
uint16_t wav_buf_pos = 0;

void wav_read_header(char* file_name){
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	char string[1024] = {0};
	
	strcat(path, "0:/MUSIC/");
	strcat(path, file_name);

	char ReadBuffer[256] = {0};
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
	
	f_read(&myFILE, ReadBuffer, 4, &fnum);
	*(ReadBuffer + 4) = '\0';
	if(strcmp(ReadBuffer, WAV_ID_FACT) == 0){
		f_read(&myFILE, ReadBuffer, 4, &fnum);
		wav_tag.fact_chunk.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
		f_lseek(&myFILE, f_tell(&myFILE) + wav_tag.fact_chunk.size);//skip whole fact chunk
		
		if(wav_tag.fact_chunk.size % 2){
			//if wav_tag.fact_chunk.size is odd number, file will pad 1byte '0'
			f_lseek(&myFILE, f_tell(&myFILE)+1); // skip the 1 byte '0'
		}
		
		f_read(&myFILE, ReadBuffer, 4, &fnum); // read next ID
		*(ReadBuffer + 4) = '\0';
	}
	
	if(strcmp(ReadBuffer, WAV_ID_LIST) == 0){
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
		
		
		f_read(&myFILE, ReadBuffer, 4, &fnum); // read next ID
		*(ReadBuffer + 4) = '\0';
	}
	
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
	
	if(strcmp(ReadBuffer, WAV_ID_DATA) == 0){
		f_read(&myFILE, ReadBuffer, 4, &fnum);
		wav_tag.data_chunk.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
		wav_tag.data_chunk.data_offest = f_tell(&myFILE);
		// the read pointer now is pointed to the audio data
		// ** play the file ** //
	}
	
	f_close(&myFILE);
}

void wav_play_music(I2S_HandleTypeDef *hi2s, I2C_HandleTypeDef *hi2c, char *file_name) {
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	char string[128] = {0};

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
	f_read(&myFILE, codec_out_buffer + AUDIO_HALF_BUFFER_SIZE, AUDIO_BUFFER_SIZE, &fnum);

	coded_i2s_set_up(hi2s, hi2c, wav_get_sample_rate(), wav_get_bit_per_sample());
	HAL_I2S_Transmit_DMA(hi2s, codec_out_buffer, AUDIO_BUFFER_SIZE);

	while (!(f_eof(&myFILE))) {
		if (file_read_flag) {
			// HAL_I2S_Transmit_DMA(hi2s, &(buffer[AUDIO_HALF_BUFFER_SIZE - wav_buf_pos]), AUDIO_BUFFER_SIZE);
			f_read(&myFILE, codec_out_buffer + wav_buf_pos, AUDIO_BUFFER_SIZE, &fnum);	// TODO: put it DMA cplt callback, but not working for now
			// HAL_I2S_Transmit(hi2s, buffer[buffer_flag], AUDIO_BUFFER_SIZE, 1000000);
			HAL_I2S_Transmit_DMA(hi2s, codec_out_buffer, AUDIO_BUFFER_SIZE);
			file_read_flag = 0;
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		}
	}
	HAL_I2S_DMAStop(hi2s);
	f_close(&myFILE);
	wav_play_flag = 0;
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

uint8_t wav_get_play_flag(){
	return wav_play_flag;
}

void wav_buf_pos_update(uint16_t pos){
	wav_buf_pos = pos;
}