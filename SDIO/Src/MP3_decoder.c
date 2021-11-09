#include "mp3common.h"
#include "main.h"
#include "fatfs.h"
#include "mp3_decoder.h"


inline void MP3_ID3_2v3_size_decode(mp3_tag_header* tag_header, uint8_t* raw_size){
	tag_header->total_tag_size = (raw_size[0] & 0x7F) << 21 | (raw_size[1] & 0x7F) << 14  | (raw_size[2] & 0x7F) << 7 | (raw_size[3] & 0x7F);
}