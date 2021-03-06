#ifndef __FILE_SYS_FUNC_H
#define	__FILE_SYS_FUNC_H

#include "fatfs.h"

#define NUM_OF_SCAN_FILE_MAX	10

extern FIL myFILE;
extern FRESULT res;
extern UINT fnum;

typedef enum file_ending{
	NOT_REACH_END,
	END_OF_FILE
}file_ending;

extern char filelist[NUM_OF_SCAN_FILE_MAX][_MAX_LFN];

// FRESULT scan_file(const TCHAR* path);
FRESULT scan_file(const TCHAR* path, uint8_t* numSongs, char** fileNames, uint8_t* fileTypes);
void find_file_type(char* file_name, char* output_file_type);
void obtain_file_name_bin(char* file_name, char* file_name_with_bin);
#endif
