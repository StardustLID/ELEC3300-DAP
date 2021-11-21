#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "lcdtp.h"
#include "file_sys_func.h"

char filelist[NUM_OF_SCAN_FILE_MAX][_MAX_LFN] = {0};

// assumption: all song files are stored in `path`
FRESULT scan_file(const TCHAR* path, uint8_t* numSongs, char** songName){
	DIR dir;
	FILINFO fno;
	FRESULT res;
	
	// open directory
	res = f_opendir(&dir, path);
	if(res == FR_OK){
		char string[256] = {0};
		uint8_t num_of_files = 0;
		
		// scan for files
		while(1){
			res = f_readdir(&dir,&fno);
			if(*(fno.fname) == '.') continue;
			if(fno.fname[0] == 0 || res != FR_OK){
				break;
			}
			// strcpy(filelist[num_of_files], fno.fname);
			// sprintf(string, "name: %s", filelist[num_of_files]);
			// LCD_DrawString(0,num_of_files*20,string);

			// char temp[256] = {0};
			// strcpy(string, fno.fname);
			// LCD_DrawString(0, 16, string);
			// &songName[num_of_files] = string;
			// LCD_DrawString(0, 32, songName[num_of_files]);
			// HAL_Delay(500);
			char* temp = malloc(strlen(fno.fname) + 1);
			strcpy(temp, fno.fname);
			char hi[5];
			sprintf(hi, "%d", num_of_files);
			LCD_DrawString(0, 16, hi);
			songName[num_of_files] = temp;
			LCD_DrawString(0, 32, *(songName + num_of_files));
			HAL_Delay(500);

			// char file_type[10] = {0};
			// find_file_type(filelist[num_of_files], file_type);
			// sprintf(string, "file type: %s", file_type);
			// LCD_DrawString(0,num_of_files*20 + 60,string);
			
			num_of_files++;
		}
		
		// update file info to main
		*numSongs = num_of_files;
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
