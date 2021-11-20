#include "main.h"
#include "wav_decoder.h"
#include "codec.h"
#include "lcdtp.h"

void codec_init(I2C_HandleTypeDef *hi2c){
	uint8_t buf[2] = {0};// 0: device address 1:device address 2:reg_address 3:reg_address
	//uint8_t readbuf[4] = {0};
	//char string[50] = {0};

	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, 0x00, 1, buf, 2, 50); 
	
	buf[0] = 0x00;
	buf[1] = 0x06;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CLOCK_RATE_2, 1, buf, 2, 50);
	
	buf[0] = 0x01;
	buf[1] = 0x00;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_WRITE_SEQUENCER_0, 1, buf, 2, 50);
	HAL_Delay(300);

	buf[0] = 0x01;
	buf[1] = 0x00;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_WRITE_SEQUENCER_3, 1, buf, 2, 50);
	HAL_Delay(300);
	
	buf[0] = 0x00;
	buf[1] = 0x00;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CLOCK_RATE_0, 1, buf, 2, 50);
	
	buf[0] = 0x00;
	buf[1] = 0x33;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_OUT_1_LEFT, 1, buf, 2, 50);
	
	buf[0] = 0x00;
	buf[1] = 0xB3;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_OUT_1_RIGHT, 1, buf, 2, 50);
	
	buf[0] = 0x00;
	buf[1] = 0x00;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DAC_DIGITAL_1, 1, buf, 2, 50);
	
	buf[0] = 0x00;
	buf[1] = 0x01;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CP_DYN_PWR, 1, buf, 2, 50);
	
	
	/*
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, 0x00, 2, buf, 2, 50); // software reset
	
	buf[1] = 1; // enable analog func
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_BIAS_CONTROL_0, 2, buf, 2, 50);
	
	buf[1] = 0x22;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_VMID_CONTROL_0, 2, buf, 2, 50);
	
	buf[1] = 0x03; // enable L/R (bit1 , bit0)
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_HEADPHONE_OUTPUT_ENABLE_ADDRESS, 2, buf, 2, 50);
	buf[1] = 0x00; // disable L/R (bit1 , bit0)
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_LINEOUT_ENABLE_ADDRESS, 2, buf, 2, 50);
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_MICINPUT_ENABLE_ADDRESS, 2, buf, 2, 50);
	
	
	buf[1] = 0x02; // enable DSP clock source
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CLOCK_RATE_2, 2, buf, 2, 50);
	
	buf[1] = 0x0C; // enable L/R DAC
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_POWER_MANAGEMENT_06, 2, buf, 2, 50);
	
	buf[1] = 0x01; // enable pump digits
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CP_ENABLE, 2, buf, 2, 50);
	
	buf[1] = 0x11; // enable L/R HPOUT
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_HP_0, 2, buf, 2, 50);
	
	buf[1] = 0x22; // enable L/R HPOUT intermediate stage
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_HP_0, 2, buf, 2, 50);
	
	buf[1] = 0x03; // enable L/R HPOUT
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DC_SERVO_0, 2, buf, 2, 50);
	
	buf[1] = 0x18; // set trig start up
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DC_SERVO_1, 2, buf, 2, 50);
	
	buf[1] = 0x44; // enable L/R HPOUT output stage
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_HP_0, 2, buf, 2, 50);
	
	buf[1] = 0x88; // enable L/R HPOUT remove short
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_HP_0, 2, buf, 2, 50);
	*/
}


void coded_i2s_set_up(I2S_HandleTypeDef *hi2s, uint32_t sample_freq, uint8_t bit_pre_sample){
	hi2s->Instance = SPI3;
  hi2s->Init.Mode = I2S_MODE_MASTER_TX;
  hi2s->Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s->Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s->Init.AudioFreq = sample_freq;
  hi2s->Init.CPOL = I2S_CPOL_LOW;
  hi2s->Init.ClockSource = I2S_CLOCK_PLL;
  hi2s->Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
	
	switch(bit_pre_sample){
		case 16: hi2s->Init.DataFormat = I2S_DATAFORMAT_16B; break;
		case 24: hi2s->Init.DataFormat = I2S_DATAFORMAT_24B; break;
	}
	
  if (HAL_I2S_Init(hi2s) != HAL_OK)
  {
    Error_Handler();
  }
}
