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
FRESULT scan_file(const TCHAR* path, uint8_t* numSongs, char** fileNames, uint8_t* fileTypes) {
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
			fileNames[numFiles] = malloc((strlen(fno.fname) + 1) * sizeof(char));
			strcpy(fileNames[numFiles], fno.fname);

			// file types / extensions
			char fileType_buffer[10] = "";
			find_file_type(fileNames[numFiles], fileType_buffer);
			
			char temp[5] = "";
			if (strcmp(fileType_buffer, ".mp3") == 0) {
				fileTypes[numFiles] = 1;
			} else if (strcmp(fileType_buffer, ".wav") == 0) {
				fileTypes[numFiles] = 2;
			} else if (strcmp(fileType_buffer, ".flac") == 0) {
				fileTypes[numFiles] = 3;
			} else {
				fileTypes[numFiles] = 99;
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
	*(output_file_type + i) = '\0';
}

void obtain_file_name_bin(char* file_name, char* file_name_with_bin){
	char* ptr = file_name;
	uint8_t i = 0;
	while(*ptr != '.'){
		*(file_name_with_bin + i) = *ptr;
		ptr++;
		i++;
	}

	strcat(file_name_with_bin, ".bin");
}
