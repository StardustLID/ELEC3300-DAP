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
void codec_i2s_update(I2S_HandleTypeDef *hi2s, I2C_HandleTypeDef *hi2c, uint32_t audio_freq, uint32_t bit_per_sample);

void i2s_DMA_cplt_callback(DMA_HandleTypeDef *hdma);
void i2s_DMA_half_cplt_callback(DMA_HandleTypeDef *hdma);
void i2s_DMA_error_callback(DMA_HandleTypeDef *hdma);


#endif
