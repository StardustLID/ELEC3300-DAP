#ifndef __MP3_DECODER_H
#define __MP3_DECODER_H


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

#define MP3_FRAME_ID_ALBUM_TITLE			"TALB"
#define MP3_FRAME_ID_SONG_TITLE				"TIT2"
#define MP3_FRAME_ID_LYRICIST					"TEXT"
#define MP3_FRAME_ID_TRACK_NUMBER			"TRCK"	
#define MP3_FRAME_ID_UNSYNC_LYRICIST	"USLT"
#define MP3_FRAME_ID_UNSYNC_LYRICIST	"SYLT"


void mp3_read_header(char* file_name);
void mp3_play_music(const uint8_t* file_name);

#endif
