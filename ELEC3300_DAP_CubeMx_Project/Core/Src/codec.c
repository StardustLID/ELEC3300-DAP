#include "codec.h"
#include <string.h>
#include "fatfs.h"
#include "file_sys_func.h"
#include "lcd.h"
#include "main.h"
#include "wav_decoder.h"
#include "mp3_decoder.h"

uint16_t codec_out_buffer[AUDIO_BUFFER_SIZE];


uint32_t size;
uint32_t file_size;
uint8_t file_read_flag = 0;

void codec_init(I2C_HandleTypeDef *hi2c, I2S_HandleTypeDef *hi2s3, DMA_HandleTypeDef *hdma_spi3_tx) {
	uint8_t buf[2] = {0};  // 0: device address 1:device address 2:reg_address 3:reg_address
	// uint8_t readbuf[4] = {0};
	//char string[50] = {0};

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

	//sprintf(string, "data: %x, %x", buf[0], buf[1]);
	//LCD_DrawString(0, 300, string);

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

void coded_i2s_set_up(I2S_HandleTypeDef *hi2s, I2C_HandleTypeDef *hi2c, uint32_t sample_freq, uint8_t bit_pre_sample) {
	
	HAL_I2S_DeInit(hi2s);
	hi2s->Instance = SPI3;
	hi2s->Init.Mode = I2S_MODE_MASTER_TX;
	hi2s->Init.Standard = I2S_STANDARD_PHILIPS;
	hi2s->Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	hi2s->Init.AudioFreq = sample_freq;
	hi2s->Init.CPOL = I2S_CPOL_LOW;
	hi2s->Init.ClockSource = I2S_CLOCK_PLL;
	hi2s->Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
	switch (bit_pre_sample) {
		case 16:
			hi2s->Init.DataFormat = I2S_DATAFORMAT_16B;
			break;
		case 24:
			hi2s->Init.DataFormat = I2S_DATAFORMAT_24B;
			break;
	}
	
	if (HAL_I2S_Init(hi2s) != HAL_OK) {
		Error_Handler();
	}
	
	uint8_t buf[2] = {0x0C, 0x00};

	switch (sample_freq) {
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
		default:
			break;
	}
	/*
	codec_i2s_update(hi2s,hi2c,sample_freq, bit_pre_sample);
  */
	// char string[25];
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

	/*
	HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, hi2s->hdmatx->XferCpltCallback);
	HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_HALFCPLT_CB_ID, hi2s->hdmatx->XferHalfCpltCallback);
	HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, hi2s->hdmatx->XferCpltCallback);
	*/
	
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void codec_volume_update(uint16_t volume, I2C_HandleTypeDef *hi2c) {
	/* dont know why the volume regsister for L/R Headphone work...*/
	/* only dac volume regsister is working... */
	/* for dac volume only 00h to C0h can be used, */
	/*   00 is mute, 01h: -71.625dB, C0h: 0db, 0.375db/step */
	uint8_t buf[2] = {0};

	if (volume > 0xC0) volume = 0xC0;

	buf[0] = 0x00;
	buf[1] = volume;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DAC_DIGITAL_VOLUME_LEFT, 1, buf, 2, 50);
	HAL_Delay(1);

	buf[0] = 0x01;
	buf[1] = volume;
	HAL_I2C_Mem_Write(hi2c, WM8918_DEVICE_ID, WM8918_DAC_DIGITAL_VOLUME_RIGHT, 1, buf, 2, 50);
	HAL_Delay(1);
}

void codec_i2s_update(I2S_HandleTypeDef *hi2s, I2C_HandleTypeDef *hi2c, uint32_t audio_freq, uint32_t bit_per_sample) {
	uint32_t i2sdiv;
	uint32_t i2sodd;
	uint32_t packetlength;
	uint32_t tmp;
	uint32_t i2sclk;

	hi2s->Init.AudioFreq = audio_freq;
	hi2s->Init.DataFormat = bit_per_sample;
	if (hi2s->Init.AudioFreq != I2S_AUDIOFREQ_DEFAULT) {
		/* Check the frame length (For the Prescaler computing) ********************/
		if (hi2s->Init.DataFormat == I2S_DATAFORMAT_16B) {
			/* Packet length is 16 bits */
			packetlength = 16U;
		} else {
			/* Packet length is 32 bits */
			packetlength = 32U;
		}

#if defined(I2S_APB1_APB2_FEATURE)
		if (IS_I2S_APB1_INSTANCE(hi2s->Instance)) {
			i2sclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2S_APB1);
		} else {
			i2sclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2S_APB2);
		}
#else
		i2sclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2S);
#endif /* I2S_APB1_APB2_FEATURE */

		if (hi2s->Init.MCLKOutput == I2S_MCLKOUTPUT_ENABLE) {
			/* MCLK output is enabled */
			if (hi2s->Init.DataFormat != I2S_DATAFORMAT_16B) {
				tmp = (uint32_t)(((((i2sclk / (packetlength * 4U)) * 10U) / hi2s->Init.AudioFreq)) + 5U);
			} else {
				tmp = (uint32_t)(((((i2sclk / (packetlength * 8U)) * 10U) / hi2s->Init.AudioFreq)) + 5U);
			}
		} else {
			/* MCLK output is disabled */
			tmp = (uint32_t)(((((i2sclk / packetlength) * 10U) / hi2s->Init.AudioFreq)) + 5U);
		}
		/* Remove the flatting point */
		tmp = tmp / 10U;

		/* Check the parity of the divider */
		i2sodd = (uint32_t)(tmp & (uint32_t)1U);

		/* Compute the i2sdiv prescaler */
		i2sdiv = (uint32_t)((tmp - i2sodd) / 2U);

		/* Get the Mask for the Odd bit (SPI_I2SPR[8]) register */
		i2sodd = (uint32_t)(i2sodd << 8U);
	}

	/* Write to SPIx I2SPR register the computed value */
	hi2s->Instance->I2SPR = (uint32_t)((uint32_t)i2sdiv | (uint32_t)(i2sodd | (uint32_t)hi2s->Init.MCLKOutput));
	MODIFY_REG(hi2s->Instance->I2SCFGR,
			   (SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CKPOL | SPI_I2SCFGR_I2SSTD | SPI_I2SCFGR_PCMSYNC |
				SPI_I2SCFGR_I2SCFG | SPI_I2SCFGR_I2SE | SPI_I2SCFGR_I2SMOD),
			   (SPI_I2SCFGR_I2SMOD | hi2s->Init.Mode | hi2s->Init.Standard | hi2s->Init.DataFormat | hi2s->Init.CPOL));
#if defined(SPI_I2SCFGR_ASTRTEN)
	if ((hi2s->Init.Standard == I2S_STANDARD_PCM_SHORT) || ((hi2s->Init.Standard == I2S_STANDARD_PCM_LONG))) {
		/* Write to SPIx I2SCFGR */
		SET_BIT(hi2s->Instance->I2SCFGR, SPI_I2SCFGR_ASTRTEN);
	}
#endif /* SPI_I2SCFGR_ASTRTEN */
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
	if(wav_get_play_flag()){
		wav_buf_pos_update(AUDIO_HALF_BUFFER_SIZE);
	}
	else if(mp3_get_play_flag()){
		mp3_buf_pos_update(MP3_HALF_OUT_BUF_SIZE);
	}
	
	file_read_flag = 1;
	// size += AUDIO_BUFFER_SIZE;
	/*
	if(file_read_for_wav((buffer + wav_buf_pos), AUDIO_BUFFER_SIZE, &size, file_size)){
		HAL_I2S_DMAStop(hi2s);
	}
	*/
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
	if(wav_get_play_flag()){
		wav_buf_pos_update(0);
	}
	else if(mp3_get_play_flag()){
		mp3_buf_pos_update(0);
	}
	file_read_flag = 1;
	/*
	if(file_read_for_wav((buffer + wav_buf_pos), AUDIO_BUFFER_SIZE, &size, file_size)){
		HAL_I2S_DMAStop(hi2s);
	}
	*/
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
}

void i2s_DMA_error_callback(DMA_HandleTypeDef *hdma){
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0); 
}
