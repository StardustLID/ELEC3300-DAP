#ifndef _CODEC_H
#define _CODEC_H

#include "wm8918_define.h"
#define AUDIO_BUFFER_SIZE 8192 // 16384// 1024 * 8
#define AUDIO_BUFFER_SIZE_DOUBLE 16384// 32768
#define AUDIO_HALF_BUFFER_SIZE 4096// 8192
void codec_init(I2C_HandleTypeDef *hi2c, I2S_HandleTypeDef* hi2s3, DMA_HandleTypeDef* hdma_spi3_tx);
void coded_i2s_set_up(I2S_HandleTypeDef *hi2s, I2C_HandleTypeDef *hi2c, uint32_t sample_freq, uint8_t bit_pre_sample);
void codec_play_music(I2S_HandleTypeDef* hi2s, I2C_HandleTypeDef *hi2c, char* file_name);


void i2s_DMA_cplt_callback(DMA_HandleTypeDef *hdma);
void i2s_DMA_half_cplt_callback(DMA_HandleTypeDef *hdma);
void i2s_DMA_error_callback(DMA_HandleTypeDef *hdma);


#endif