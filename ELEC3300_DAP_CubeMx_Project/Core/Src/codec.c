#include "main.h"
#include "wav_decoder.h"
#include "codec.h"
#include "file_sys_func.h"
#include "lcd.h"
#include "fatfs.h"
#include <string.h>

static uint16_t buffer[AUDIO_BUFFER_SIZE];

uint16_t buf_pos = 0;
uint32_t size = 0;
uint32_t file_size = 0;
uint8_t file_read_flag = 0;

void codec_init(I2C_HandleTypeDef *hi2c, I2S_HandleTypeDef* hi2s3, DMA_HandleTypeDef* hdma_spi3_tx){
	uint8_t buf[2] = {0};// 0: device address 1:device address 2:reg_address 3:reg_address
	//uint8_t readbuf[4] = {0};
	char string[50] = {0};

	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, 0x00, 1, buf, 2, 50); 
	
	buf[0] = 0x00;
	buf[1] = 0x06;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CLOCK_RATE_2, 1, buf, 2, 50);
	HAL_Delay(1);
	
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
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_LINEOUT_ENABLE_ADDRESS, 1, buf, 2, 50);
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_MICINPUT_ENABLE_ADDRESS, 1, buf, 2, 50);
	
	buf[0] = 0x00;	
	buf[1] = 0x77; 
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_LINEOUT_0, 1, buf, 2, 50);
	
	buf[0] = 0x00;	
	buf[1] = 0x00; 
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_LINEOUT_0, 1, buf, 2, 50);
	
	buf[0] = 0x00;	
	buf[1] = 0x0C; 
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DC_SERVO_0, 1, buf, 2, 50);
	
	buf[0] = 0x84;
	buf[1] = 0x5E;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CLOCK_RATE_0, 1, buf, 2, 50);
	HAL_Delay(1);
	/*
	buf[0] = 0x00;
	buf[1] = 0x00;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_OUT12_ZC, 1, buf, 2, 50);
	HAL_Delay(1);
	*/
	
	buf[0] = 0x00;
	buf[1] = 0x30;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_OUT_1_LEFT, 1, buf, 2, 50);
	HAL_Delay(1);
	
	buf[0] = 0x00;
	buf[1] = 0xB0;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_OUT_1_RIGHT, 1, buf, 2, 50);
	HAL_Delay(1);
	
	buf[0] = 0x00;
	buf[1] = 0x00;
	HAL_I2C_Mem_Read(hi2c, WM8918_DEVICE_ID, WM8918_ANALOGUE_OUT_2_LEFT, 1, buf, 2, 50);
	HAL_Delay(1);

	sprintf(string, "data: %x, %x", buf[0], buf[1]);
	LCD_DrawString(0,300,string);

	buf[0] = 0x00;
	buf[1] = 0x40;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DAC_DIGITAL_VOLUME_LEFT, 1, buf, 2, 50);
	HAL_Delay(1);
	
	buf[0] = 0x01;
	buf[1] = 0x40;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DAC_DIGITAL_VOLUME_RIGHT, 1, buf, 2, 50);
	HAL_Delay(1);
	
	buf[0] = 0x00;
	buf[1] = 0x00;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DAC_DIGITAL_1, 1, buf, 2, 50);
	HAL_Delay(1);
	
	
	/*
	buf[0] = 0x00;
	buf[1] = 0x01;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CP_ENABLE, 1, buf, 2, 50);
	HAL_Delay(1);
	*/
	
	buf[0] = 0x00;
	buf[1] = 0x05;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CP_DYN_PWR, 1, buf, 2, 50);
	HAL_Delay(1);

	buf[0] = 0x00;
	buf[1] = 0x02;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_AUDIO_INTERFACE_1, 1, buf, 2, 50);
	HAL_Delay(1);

}


void coded_i2s_set_up(I2S_HandleTypeDef *hi2s, I2C_HandleTypeDef *hi2c, uint32_t sample_freq, uint8_t bit_pre_sample){
	HAL_I2S_DeInit(hi2s);
	
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
	
	uint8_t buf[2] = {0x0C,0x00};
	
	switch(sample_freq){
		case I2S_AUDIOFREQ_8K: 
			break;
		case I2S_AUDIOFREQ_11K: 
		case 12000:
			buf[1] = 1;
			break;
		case I2S_AUDIOFREQ_16K: 
			buf[1] = 2;
			break;
		case I2S_AUDIOFREQ_22K: 
		case 24000: 
			buf[1] = 3;
			break;
		case I2S_AUDIOFREQ_32K: 
			buf[1] = 4;
			break;
		case I2S_AUDIOFREQ_44K: 
		case I2S_AUDIOFREQ_48K: 
			buf[1] = 5;
			break;
		case I2S_AUDIOFREQ_96K: 
			buf[0] = 0x04; 
			buf[1] = 5;
			break;
		/* to set 96kHz, from datasheet p.96
			SAMPLE_RATE = 101
			CLK_SYS_RATE = 0001 (SYSCLK/fs = 128)
			BCLK_DIV = 00010
			LRCLK_RATE = 040h
			DAC_OSR128 = 0
			EQ_ENA = 0
		*/
		case I2S_AUDIOFREQ_192K:
		default: break;
	}
	
	char string[25];
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_CLOCK_RATE_1, 1, buf, 2, 50);
	HAL_Delay(1);
	/*
	sprintf(string, "data: %x, %x", buf[0], buf[1]);
	LCD_DrawString(0,300,string);
	*/
	
	/*
	hi2s->hdmatx->XferCpltCallback = i2s_DMA_cplt_callback;
	hi2s->hdmatx->XferHalfCpltCallback = i2s_DMA_half_cplt_callback;
	hi2s->hdmatx->XferErrorCallback = i2s_DMA_error_callback;
	*/
	
	HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, hi2s->hdmatx->XferCpltCallback);
	HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_HALFCPLT_CB_ID, hi2s->hdmatx->XferHalfCpltCallback);
	HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, hi2s->hdmatx->XferCpltCallback);
	
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	
}

void codec_play_music(I2S_HandleTypeDef* hi2s, I2C_HandleTypeDef *hi2c, char* file_name){
	
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	char string[128] = {0};
	
	strcat(path, "0:/MUSIC/");
	strcat(path, file_name);

	char ReadBuffer[2] = {0};
	
	uint16_t data = 0;
	
	size = wav_get_data_offest();
	file_size = wav_get_file_size();
	
	f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);
	
	f_lseek(&myFILE, wav_get_data_offest()); // jump to the music data
	f_read(&myFILE, buffer, AUDIO_BUFFER_SIZE, &fnum);
	f_read(&myFILE, buffer + AUDIO_HALF_BUFFER_SIZE, AUDIO_BUFFER_SIZE, &fnum);
	
	
	coded_i2s_set_up(hi2s, hi2c, wav_get_sample_rate(), wav_get_bit_per_sample());
	HAL_I2S_Transmit_DMA(hi2s, buffer, AUDIO_BUFFER_SIZE);
	
	while(size < file_size){	
		if(file_read_flag){
			//HAL_I2S_Transmit_DMA(hi2s, &(buffer[AUDIO_HALF_BUFFER_SIZE - buf_pos]), AUDIO_BUFFER_SIZE);
			f_read(&myFILE, buffer + buf_pos, AUDIO_BUFFER_SIZE, &fnum); // TODO: put it DMA cplt callback, but not working for now
			size += AUDIO_BUFFER_SIZE;
			//HAL_I2S_Transmit(hi2s, buffer[buffer_flag], AUDIO_BUFFER_SIZE, 1000000);
			file_read_flag = 0;
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
		}
	}
	HAL_I2S_DMAStop(hi2s);
}

void codec_volume_update(uint16_t volume, I2C_HandleTypeDef *hi2c){
	/* dont know why the volume regsister for L/R Headphone work...*/
	/* only dac volume regsister is working... */
	/* for dac volume only 00h to C0h can be used, */
	/*   00 is mute, 01h: -71.625dB, C0h: 0db, 0.375db/step */
	uint8_t buf[2] = {0};
	
	if(volume > 0xC0) volume = 0xC0;
	
	
	buf[0] = 0x00;
	buf[1] = volume;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DAC_DIGITAL_VOLUME_LEFT, 1, buf, 2, 50);
	HAL_Delay(1);
	
	buf[0] = 0x01;
	buf[1] = volume;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DAC_DIGITAL_VOLUME_RIGHT, 1, buf, 2, 50);
	HAL_Delay(1);
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s){
	buf_pos = AUDIO_HALF_BUFFER_SIZE;
	file_read_flag = 1;
	//size += AUDIO_BUFFER_SIZE;
	/*
	if(file_read_for_wav((buffer + buf_pos), AUDIO_BUFFER_SIZE, &size, file_size)){
		HAL_I2S_DMAStop(hi2s);
	}
	*/
	HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,0);
	
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s){
	buf_pos = 0;
	file_read_flag = 1;
	/*
	if(file_read_for_wav((buffer + buf_pos), AUDIO_BUFFER_SIZE, &size, file_size)){
		HAL_I2S_DMAStop(hi2s);
	}
	*/
	HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,1);
}

void i2s_DMA_error_callback(DMA_HandleTypeDef *hdma){
		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,0);
}