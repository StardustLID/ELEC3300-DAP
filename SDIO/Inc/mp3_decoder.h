#ifndef __MP3_DECODER_H
#define __MP3_DECODER_H


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

inline void MP3_ID3_2v3_size_decode(mp3_tag_header* tag_header, uint8_t* raw_size);

#define MP3_FRAME_ID_ALBUM_TITLE			"TALB"
#define MP3_FRAME_ID_SONG_TITLE				"TIT2"
#define MP3_FRAME_ID_LYRICIST					"TEXT"
#define MP3_FRAME_ID_TRACK_NUMBER			"TRCK"	
#define MP3_FRAME_ID_UNSYNC_LYRICIST	"USLT"
#define MP3_FRAME_ID_UNSYNC_LYRICIST	"SYLT"



#endif
