#include "mp3common.h"
#include "main.h"
#include "fatfs.h"
#include "lcd.h"
#include <string.h>
#include "wav_decoder.h"
#include "file_sys_func.h"

wav_tag_header wav_tag = {0};

void wav_read_header(char* file_name){
	FIL myFILE;
	FRESULT res;
	int fnum;
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
		wav_tag.fact_chunk.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
		
uint8_t ID[4] = {0};
		uint32_t size = 0;
		
		while(strcmp(ReadBuffer, WAV_ID_END_OLIST) != 0){ // while condition can be strcmp(buf, "INFO") or the sub-chunk size
			f_read(&myFILE, ReadBuffer, 8, &fnum); // read the sub-chunk ID, size
			strncpy(ID, ReadBuffer, 4);
			size = (ReadBuffer[4] & 0xff) | (ReadBuffer[5] & 0xff) << 8 |  (ReadBuffer[6] & 0xff) << 16 | (ReadBuffer[7] & 0xff) << 24;
			
			// do if-else to check what is the type of sub-chunk
			// read the size
			// if sub_chunk's size is odd number, file will pad 1 byte '0', move the read pointer by 1
			// if not is the thing listed on .h define, skip it
			if(strcmp(ID, WAV_INFO_ID_ARTIST) == 0){
				f_read(&myFILE, ReadBuffer, size, &fnum); 
				strncpy(wav_tag.artist, ReadBuffer, size);
				
				if(size % 2){
					//if size is odd number, file will pad 1byte '0'
					f_lseek(&myFILE, f_tell(&myFILE)+1); // skip the 1 byte '0'
				}
				
			}
			else if(strcmp(ID, WAV_INFO_ID_TITLE) == 0){
				
			}
			else if(strcmp(ID, WAV_INFO_ID_ALBUM) == 0){
				
			}
			else if(strcmp(ID, WAV_INFO_ID_DATE) == 0){
				
			}
			else if(strcmp(ID, WAV_INFO_ID_GENRE) == 0){
				
			}
			else{
				
			}
			
			
		}
		
		
		f_read(&myFILE, ReadBuffer, 4, &fnum); // read next ID
		*(ReadBuffer + 4) = '\0';
	}
	
	if(strcmp(ReadBuffer, WAV_ID_DATA) == 0){
		f_read(&myFILE, ReadBuffer, 4, &fnum);
		wav_tag.data_chunk.size = (ReadBuffer[0] & 0xff) | (ReadBuffer[1] & 0xff) << 8 |  (ReadBuffer[2] & 0xff) << 16 | (ReadBuffer[3] & 0xff) << 24;
		// the read pointer now is pointed to the audio data
		// ** play the file ** //
	}
	
	
}