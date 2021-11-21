#ifndef __FILE_SYS_FUNC_H
#define	__FILE_SYS_FUNC_H

#define NUM_OF_SCAN_FILE_MAX	100

extern char filelist[NUM_OF_SCAN_FILE_MAX][_MAX_LFN];

FRESULT scan_file(const TCHAR* path, uint8_t* numSongs, char** fileNames, char** fileTypes);
void find_file_type(char* file_name, char* output_file_type);
#endif
