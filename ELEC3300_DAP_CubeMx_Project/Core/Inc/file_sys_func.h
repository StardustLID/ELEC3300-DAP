#ifndef __FILE_SYS_FUNC_H
#define	__FILE_SYS_FUNC_H

#include "fatfs.h"

#define NUM_OF_SCAN_FILE_MAX	100

extern FIL myFILE;
extern FRESULT res;
extern UINT fnum;

typedef enum file_ending{
	NOT_REACH_END,
	END_OF_FILE
}file_ending;

extern char filelist[NUM_OF_SCAN_FILE_MAX][_MAX_LFN];

FRESULT scan_file(const TCHAR* path);
void find_file_type(char* file_name, char* output_file_type);
file_ending file_read_for_wav(void* buff,	UINT buf_size, uint32_t* read_size, const uint32_t file_size);
#endif