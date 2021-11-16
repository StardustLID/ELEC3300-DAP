#include "main.h"
#include "wav_decoder.h"
#include "codec.h"

void codec_init(I2C_HandleTypeDef *hi2c){
	uint8_t buf[2] = {0};
	
	buf[1] = 1; // enable analog func
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_BIAS_CONTROL_0, 2, buf, 2, 50);

	
	
	buf[1] = 0x03; // enable L/R (bit1 , bit0)
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_HEADPHONE_OUTPUT_ENABLE_ADDRESS, 2, buf, 2, 50);
	buf[1] = 0x00; // disable L/R (bit1 , bit0)
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_LINEOUT_ENABLE_ADDRESS, 2, buf, 2, 50);
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_INPUT_ENABLE_ADDRESS, 2, buf, 2, 50);
	buf[1] = 0x06; // enable DAC L/R (bit4, bit3)
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_INPUT_ENABLE_ADDRESS, 2, buf, 2, 50);
	
	
	
}