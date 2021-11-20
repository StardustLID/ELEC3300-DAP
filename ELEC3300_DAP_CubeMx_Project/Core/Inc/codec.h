#ifndef _CODEC_H
#define _CODEC_H

#include "wm8918_define.h"

void codec_init(I2C_HandleTypeDef *hi2c);
void coded_i2s_set_up(I2S_HandleTypeDef *hi2s, uint32_t sample_freq, uint8_t bit_pre_sample);

#endif
