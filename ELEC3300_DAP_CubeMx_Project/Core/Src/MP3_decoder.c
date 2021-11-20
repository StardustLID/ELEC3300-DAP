#include "mp3common.h"
#include "main.h"
#include "fatfs.h"
#include "mp3_decoder.h"
#include <string.h>
#include "file_sys_func.h"

mp3_tag_header mp3_tag = {0};

void mp3_read_header(char* file_name){
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	char string[1024] = {0};
	
	strcat(path, "0:/MUSIC/");
	strcat(path, file_name);
	
	char ReadBuffer[256] = {0};
	f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);
	
	f_lseek(&myFILE, 3); // skip "ID3"
	f_read(&myFILE, ReadBuffer, 1, &fnum);
	mp3_tag.ID3_ver = ReadBuffer[0];
	f_read(&myFILE, ReadBuffer, 2, &fnum);
	mp3_tag.flag = ReadBuffer[0];
	f_read(&myFILE, ReadBuffer, 2, &fnum);
	mp3_tag.total_tag_size = (ReadBuffer[0] & 0x7F) << 21 | (ReadBuffer[1] & 0x7F) << 14  | (ReadBuffer[2] & 0x7F) << 7 | (ReadBuffer[3] & 0x7F);
}

void mp3_play_music(const uint8_t* file_name){
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	char string[128] = {0};
	
	strcat(path, "0:/MUSIC/");
	strcat(path, file_name);
	
	f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);
}