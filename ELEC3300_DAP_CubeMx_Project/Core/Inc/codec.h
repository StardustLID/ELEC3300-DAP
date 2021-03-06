#ifndef _CODEC_H
#define _CODEC_H

#include "wm8918_define.h"
#include <stdint.h>
#include "main.h"

#define AUDIO_BUFFER_SIZE 4096//8192 // 16384// 1024 * 8
#define AUDIO_BUFFER_SIZE_DOUBLE 8192//16384// 32768
#define AUDIO_HALF_BUFFER_SIZE 2048//4096// 8192
extern uint16_t codec_out_buffer[AUDIO_BUFFER_SIZE];
extern uint32_t size;
extern uint32_t file_size;
extern uint8_t file_read_flag;


void codec_init(I2C_HandleTypeDef *hi2c, I2S_HandleTypeDef* hi2s3, DMA_HandleTypeDef* hdma_spi3_tx);
void coded_i2s_set_up(I2S_HandleTypeDef *hi2s, I2C_HandleTypeDef *hi2c, uint32_t sample_freq, uint8_t bit_pre_sample);
void codec_volume_update(I2C_HandleTypeDef *hi2c, uint16_t volume);
void update_play_flag(uint8_t flag);

void codec_load_setting();
void codec_eq_enable(uint8_t enable_flag);
void codec_eq(uint8_t band1, uint8_t band2, uint8_t band3, uint8_t band4, uint8_t band5);


void i2s_DMA_cplt_callback(DMA_HandleTypeDef *hdma);
void i2s_DMA_half_cplt_callback(DMA_HandleTypeDef *hdma);
void i2s_DMA_error_callback(DMA_HandleTypeDef *hdma);


#endif
