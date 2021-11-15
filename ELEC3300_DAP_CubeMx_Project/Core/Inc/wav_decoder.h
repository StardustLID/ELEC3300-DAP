#ifndef __WAV_DECODER_H
#define __WAV_DECODER_H


typedef struct wav_fmt_chunk{
		// 4 byte: "fmt "
	uint32_t size; // size of fmt chunk - 8, 4 byte, example: from SD :"0x F4 FE 83 01", Real: 0x 01 83 FE F4
	
	uint8_t codec_fmt; // 2 byte on SD, example: from SD :"0x 01 00", Real: 0x 00 01, 01 represent PCM
	uint8_t sound_ch; // 2 byte on SD, example: from SD :"0x 01 00", Real: 0x 00 01, 01 represent mono, 02 represent stereo
	uint32_t sample_rate; // 4 byte, example: from SD :"0x 44 AC 00 00", Real: 0x AC 44 (44.1kHz)
	uint32_t byte_rate; // == SampleRate * NumChannels * BitsPerSample/8,  4 byte, example: from SD :"0x F4 FE 83 01", Real: 0x 01 83 FE F4, 
	uint16_t block_align; // == NumChannels * BitsPerSample/8, 2 byte on SD, example: from SD :"0x 01 00", Real: 0x 00 01
	uint16_t  bit_per_sample; // 2 byte on SD, example: from SD :"0x 01 00", Real: 0x 00 01
}wav_fmt_chunk;

typedef struct wav_fact_chunk{
	// 4 byte: "fact"
	uint32_t size; // size of fact_chunk - 8
}wav_fact_chunk;

typedef struct wav_list_chunk{
	// 4 byte: "LIST"
	uint32_t size; // size of list_chunk - 8
	char artist[54];
	char title[54];
	char album[54];
	char Date[16];
	char genre[32];
	// 4 byte: "INFO"
}wav_list_chunk;

typedef struct wav_data_chunk{
	// 4 byte: "data"
	uint32_t size; // size of data_chunk - 8
	uint32_t data_offest;
}wav_data_chunk;

typedef struct wav_tag_header{
	// 4 byte: "RIFF"
	uint32_t file_size; // == wav file size - 8, 4 byte, example: from SD :"0x F4 FE 83 01", Real: 0x 01 83 FE F4
	// 4 byte: "WAVE"
	// 4 byte: "fmt "
	wav_fmt_chunk  fmt_chunk;
	// 4 byte: "fact"
	wav_fact_chunk fact_chunk;// optional
	// 4 byte: "LIST"
	wav_list_chunk list_chunk;// optional
	// 4 byte: "INFO"
	// 4 byte: "data"
	wav_data_chunk data_chunk;
}wav_tag_header;


#define WAV_INFO_ID_ARTIST		"IART" // The artist of the original subject of the file
#define WAV_INFO_ID_TITLE			"INAM" // Title of the subject of the file (name)
#define WAV_INFO_ID_ALBUM			"IPRD" // Name of the title the subject was originally intended for
#define WAV_INFO_ID_DATE			"ICRD" // The date the subject of the file was created (creation date)
#define WAV_INFO_ID_GENRE			"IGNR" // The genre of the subject

#define WAV_ID_DATA						"data"
#define WAV_ID_FACT						"fact"
#define WAV_ID_LIST						"LIST"
#define WAV_ID_END_OF_LIST		"INFO"

void wav_read_header(char* file_name);
void wav_play_music(I2S_HandleTypeDef* hi2s,char* file_name);
#endif