#include "mp3common.h"
#include "fatfs.h"
#include "mp3_decoder.h"
#include <string.h>
#include "file_sys_func.h"
#include "codec.h"

mp3_tag_header mp3_tag = {0};
uint8_t mp3_read_buf[MP3_READ_BUF_SIZE];
uint16_t mp3_out_buf[MP3_OUT_BUF_SIZE*2];
uint8_t mp3_play_flag = 0;
uint16_t mp3_buf_pos = 0;

static HMP3Decoder mp3decoder;
static MP3FrameInfo mp3frameinfo;
static mp3_type mp3_player;



void mp3_read_header(char* file_name){
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	char string[1024] = {0};
	
	strcat(path, "0:/MUSIC/");
	strcat(path, file_name);
	
	char ReadBuffer[256] = {0};
	f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);
	
	f_lseek(&myFILE, 3); // skip "ID3"
	f_read(&myFILE, ReadBuffer, 1, &fnum);
	mp3_tag.ID3_ver = ReadBuffer[0];
	f_read(&myFILE, ReadBuffer, 2, &fnum);
	mp3_tag.flag = ReadBuffer[0];
	f_read(&myFILE, ReadBuffer, 2, &fnum);
	mp3_tag.total_tag_size = (ReadBuffer[0] & 0x7F) << 21 | (ReadBuffer[1] & 0x7F) << 14  | (ReadBuffer[2] & 0x7F) << 7 | (ReadBuffer[3] & 0x7F);
}

void mp3_play_music(I2S_HandleTypeDef* hi2s, I2C_HandleTypeDef *hi2c,const uint8_t* file_name){
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	strcat(path, "0:/MUSIC/");

	strcat(path, file_name);
	uint8_t *read_ptr=mp3_read_buf;
	uint32_t frames=0;
	int err=0, i=0, outputSamps=0;	
	int	read_offset = 0;
	int	bytes_left = 0;	

	mp3_play_flag = 1;
	
	res = f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);
	if(res != FR_OK){
		// cannot open file
		f_close(&myFILE);		
		return;
	}
	
	mp3decoder = MP3InitDecoder();
	if(mp3decoder==0){
		return;
	}

	coded_i2s_set_up(hi2s, hi2c,44100, 16);

	res = f_read(&myFILE, mp3_read_buf, MP3_READ_BUF_SIZE, &fnum);
	if(res != FR_OK){
		MP3FreeDecoder(mp3decoder);
		return;
	}

	read_ptr = mp3_read_buf;
	bytes_left = fnum;

	/*play music*/
	while(file_read_flag){
		read_offset = MP3FindSyncWord(read_ptr, bytes_left);
		if(read_offset < 0){
			res = f_read(&myFILE, mp3_read_buf, MP3_READ_BUF_SIZE, &fnum);
			if(res != FR_OK){
				// cannot open file
				break;
			}
			read_ptr = mp3_read_buf;
			bytes_left = fnum;
		}
		read_ptr += read_offset;
		bytes_left -= read_offset;
		if(bytes_left < 1024){
			i=(uint32_t)(bytes_left)&3;
			if(i) i = 4 - i;
			memcpy(mp3_read_buf + i, read_ptr, bytes_left);
			read_ptr = mp3_read_buf + i;
			res = f_read(&myFILE, mp3_read_buf + bytes_left + i, MP3_READ_BUF_SIZE - bytes_left - i, &fnum);//补充数据
			if(res != FR_OK)
			{
				break;
			}
			bytes_left += fnum;	
		}
		err = MP3Decode(mp3decoder, &read_ptr, &bytes_left, &(codec_out_buffer[mp3_buf_pos]), 0); 
		frames++;
		if(err != ERR_MP3_NONE){
			switch (err)
			{
				case ERR_MP3_INDATA_UNDERFLOW:
					res = f_read(&myFILE, mp3_read_buf, MP3_READ_BUF_SIZE, &fnum);
					read_ptr = mp3_read_buf;
					bytes_left = fnum;
					break;		
				case ERR_MP3_MAINDATA_UNDERFLOW:
					/* do nothing - next call to decode will provide more mainData */
					break;		
				default:
					if (bytes_left > 0)
					{
						bytes_left --;
						read_ptr ++;
					}	
					break;
			}
		}
		else{
			MP3GetLastFrameInfo(mp3decoder, &mp3frameinfo);
			outputSamps = mp3frameinfo.outputSamps;
			if(outputSamps > 0){
				if(mp3frameinfo.nChans == 1){
					for (i = outputSamps - 1; i >= 0; i--)
					{
						codec_out_buffer[mp3_buf_pos + i * 2] = codec_out_buffer[mp3_buf_pos + i];
						codec_out_buffer[mp3_buf_pos + i * 2 + 1] = codec_out_buffer[mp3_buf_pos + i];
					}
					outputSamps *= 2;
				}
			}
			mp3_player.output_buf_size = outputSamps;
			if(mp3frameinfo.samprate != mp3_player.sample_rate){
				mp3_player.sample_rate = mp3frameinfo.samprate;
				if(mp3_player.sample_rate >= I2S_AUDIOFREQ_DEFAULT){
					codec_i2s_update(hi2s,hi2c, mp3_player.sample_rate, I2S_DATAFORMAT_16B);
					HAL_I2S_Transmit_DMA(hi2s, codec_out_buffer, outputSamps);
				}
			}
		}
		if(f_eof(&myFILE)){
			break;
		}
	}
	HAL_I2S_DMAStop(hi2s);
	MP3FreeDecoder(mp3decoder);
	f_close(&myFILE);
	mp3_play_flag = 0;
}

inline uint32_t mp3_get_size(){
	return mp3_tag.total_tag_size + 10;
}

uint8_t mp3_get_play_flag(){
	return mp3_play_flag;
}

void mp3_buf_pos_update(uint16_t pos){
	mp3_buf_pos = pos;
}