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
	
	
}