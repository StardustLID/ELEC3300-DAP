#include "main.h"
#include "fatfs.h"
#include "lcdtp.h"
#include <string.h>
#include "file_sys_func.h"

char filelist[NUM_OF_SCAN_FILE_MAX][_MAX_LFN] = {0};

FRESULT scan_file(const TCHAR* path){
	DIR dir;
	FILINFO fno;
	FRESULT res;
	
	res = f_opendir(&dir, path);
	if(res == FR_OK){
		char string[256] = {0};
		int num_of_file = 0;
		while(1){
			res = f_readdir(&dir,&fno);
			if(*(fno.fname) == '.') continue;
			if(fno.fname[0] == 0 || res != FR_OK){
				break;
			}
			strcpy(filelist[num_of_file], fno.fname);
			sprintf(string, "name: %s", filelist[num_of_file]);
			LCD_DrawString(0,num_of_file*20,string);
			
			char file_type[10] = {0};
			find_file_type(filelist[num_of_file], file_type);
			sprintf(string, "file type: %s", file_type);
			LCD_DrawString(0,num_of_file*20 + 60,string);
			
			num_of_file++;
		}
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
