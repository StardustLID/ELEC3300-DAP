#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "main.h"
#include "fatfs.h"
#include "lcdtp.h"
#include "file_sys_func.h"
#include "codec.h"

char filelist[NUM_OF_SCAN_FILE_MAX][_MAX_LFN] = {0};
FIL myFILE;
FRESULT res;
UINT fnum;

// assumption: all song files are stored in `path`
// FRESULT scan_file(const TCHAR* path){
FRESULT scan_file(const TCHAR* path, uint8_t* numSongs, char** fileNames, uint8_t** fileTypes) {
	DIR dir;
	FILINFO fno;
	FRESULT res;
	
	// open directory
	res = f_opendir(&dir, path);
	if(res == FR_OK){
		uint8_t numFiles = 0;
		
		// scan for files
		while (1) {
			res = f_readdir(&dir,&fno);
			if(*(fno.fname) == '.') continue;
			if(fno.fname[0] == 0 || res != FR_OK){
				break;
			}

			// file names
			fileNames[numFiles] = malloc(strlen(fno.fname) + 1);
			strcpy(fileNames[numFiles], fno.fname);

			// file types / extensions
			char fileType_buffer[10] = {0};
			find_file_type(fileNames[numFiles], fileType_buffer);
			
			fileTypes[numFiles] = malloc(sizeof(uint8_t));
			if (strcmp(fileType_buffer, ".mp3")) {
				*fileTypes[numFiles] = 1;
			} else if (strcmp(fileType_buffer, ".wav")) {
				*fileTypes[numFiles] = 2;
			} else if (strcmp(fileType_buffer, ".flac")) {
				*fileTypes[numFiles] = 3;
			}

			numFiles++;
		}
		
		*numSongs = numFiles;
	}

	return res;
}


void find_file_type(char* file_name, char* output_file_type){
	char* ptr = file_name;
	uint8_t found_the_dot = 0, i = 0;
	while(*ptr != '\0'){
		if(!found_the_dot){
			if(*ptr == '.'){
				*(output_file_type + i) = *ptr;
				i++;
				found_the_dot = 1;
			}
		}
		else{
			*(output_file_type + i) = *ptr;
			i++;
		}
		ptr++;
	}
}

file_ending file_read_for_wav(void* buff,	UINT buf_size, uint32_t* read_size, const uint32_t file_size){
	f_read(&myFILE, buff, buf_size, &fnum);
	*read_size += AUDIO_BUFFER_SIZE;
	HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	if(*read_size >= file_size){
		f_close(&myFILE);
	}
	
}
