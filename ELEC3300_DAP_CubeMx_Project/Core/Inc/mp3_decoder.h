#ifndef __MP3_DECODER_H
#define __MP3_DECODER_H


#define MP3_READ_BUF_SIZE 4096
#define MP3_OUT_BUF_SIZE 4608//2304*2
#define MP3_HALF_OUT_BUF_SIZE 2304


typedef struct mp3_id_frame{
	uint32_t size;
	char data[50];
}mp3_id_frame;

typedef struct mp3_tag_header{
	uint8_t ID3_ver;
	uint8_t flag; // 0xabc0 0000, a:Unsynchronisation, b:Extended header, c:Experimental indicator
	uint32_t total_tag_size;
}mp3_tag_header;

typedef struct mp3_tag_frame_header{
	uint8_t frame_ID[4];
	uint32_t tag_size;
	uint16_t flag;
}mp3_tag_frame_header;

typedef struct mp3_type{
	uint8_t volume;
	uint32_t sample_rate;
	uint32_t output_buf_size;
}mp3_type;

#define MP3_FRAME_ID_ALBUM_TITLE			"TALB"
#define MP3_FRAME_ID_SONG_TITLE				"TIT2"
#define MP3_FRAME_ID_LYRICIST					"TEXT"
#define MP3_FRAME_ID_TRACK_NUMBER			"TRCK"	
#define MP3_FRAME_ID_UNSYNC_LYRICIST	"USLT"
#define MP3_FRAME_ID_UNSYNC_LYRICIST	"SYLT"


void mp3_read_header(char* file_name);
void mp3_play_music(I2S_HandleTypeDef* hi2s, I2C_HandleTypeDef *hi2c,const uint8_t* file_name);
uint32_t mp3_get_size();
uint8_t mp3_get_play_flag();
void mp3_buf_pos_update(uint16_t pos);
#endif
