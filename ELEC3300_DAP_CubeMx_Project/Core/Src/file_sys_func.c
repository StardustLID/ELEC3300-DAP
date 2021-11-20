#include "main.h"
#include "fatfs.h"
#include "lcd.h"
#include <string.h>
#include "file_sys_func.h"
#include "codec.h"

char filelist[NUM_OF_SCAN_FILE_MAX][_MAX_LFN] = {0};
FIL myFILE;
FRESULT res;
UINT fnum;

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
file_ending file_read_for_wav(void* buff,	UINT buf_size, uint32_t* read_size, const uint32_t file_size){
	f_read(&myFILE, buff, buf_size, &fnum);
	*read_size += AUDIO_BUFFER_SIZE;
	HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	if(*read_size >= file_size){
		f_close(&myFILE);
	}
	
}