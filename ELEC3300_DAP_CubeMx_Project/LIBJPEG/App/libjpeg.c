/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : libjpeg.c
  * Description        : This file provides code for the configuration
  *                      of the libjpeg instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "libjpeg.h"

/* USER CODE BEGIN 0 */
#include <string.h>
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
#include "file_sys_func.h"
/* USER CODE END 1 */

/* Global variables ---------------------------------------------------------*/

/* USER CODE BEGIN 2 */
/* USER CODE END 2 */

/* LIBJPEG init function */
void MX_LIBJPEG_Init(void)
{
/***************************************/
   /**
  */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

}

/* USER CODE BEGIN 4 */
void decode_jpeg(char* file_name){
	
	char path[sizeof("0:/MUSIC/") + _MAX_LFN] = {0};
	//char string[1024] = {0};
	
	strcat(path, "0:/MUSIC/");
	strcat(path, file_name);
	
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	res = f_open(&myFILE, path, FA_READ |FA_OPEN_EXISTING);
	
	if(res != FR_OK){
		return;
	}
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo,&myFILE);
	jpeg_read_header(&cinfo,TRUE);
	jpeg_start_decompress(&cinfo);
	
	uint32_t width = cinfo.output_width;
	uint32_t height = cinfo.output_height;
	uint16_t depth = cinfo.output_components;
	
	unsigned char *src_buff;
	src_buff = (unsigned char *)malloc(width * height * depth);
	if(src_buff == 0){
		return;
	}
	memset(src_buff, 0, sizeof(unsigned char) * width * height * depth);
	
	JSAMPARRAY buffer;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, width*depth, 1);
	
	unsigned char *point = src_buff;
	while(cinfo.output_scanline < height){
		jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(point, *buffer, width*depth);
		point += width * depth;
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(src_buff);
	
	f_close(&myFILE);
}
/* USER CODE END 4 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
