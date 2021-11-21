#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "lcdtp.h"
#include "file_sys_func.h"

char filelist[NUM_OF_SCAN_FILE_MAX][_MAX_LFN] = {0};

// assumption: all song files are stored in `path`
FRESULT scan_file(const TCHAR* path, uint8_t* numSongs, char** fileNames, char** fileTypes) {
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
			// LCD_DrawString(0, 32, fileNames[numFiles]); // debug use

			// file types / extensions
			char fileType_buffer[10] = {0};
			find_file_type(fileNames[numFiles], fileType_buffer);
			// fileTypes[numFiles] = malloc(strlen(fileType_buffer) + 1);
			// strcpy(fileTypes[numFiles], fileType_buffer);
			LCD_DrawString(0, numFiles*20 + 60, fileType_buffer);
			// LCD_DrawString(0, numFiles*20 + 60, fileTypes[numFiles]); // debug use
			HAL_Delay(300); // debug use

			numFiles++;
		}
		
		// update file info to main
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

//f_lseek(&myFILE, f_tell(&myFILE) - 4); f_tell return current read/write pointer value
