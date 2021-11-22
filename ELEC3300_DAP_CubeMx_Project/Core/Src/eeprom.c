#include "eeprom.h"
#include "main.h"
#include "lcd.h"
#include <string.h>

eeprom_struct eeprom;

char string[15];

void eeprom_init(I2C_HandleTypeDef *hi2c){
	eeprom.date_address = 0;
	eeprom.time_address = sizeof(eeprom.ver_date)/sizeof(eeprom.ver_date[0]);
	eeprom.volume_address = sizeof(eeprom.ver_time)/sizeof(eeprom.ver_time[0]) + eeprom.time_address;
	
	uint8_t* data = __DATE__;
	eeprom_write(hi2c,EEPROM_DATE, data);
	HAL_Delay(20);
	eeprom_read(hi2c, EEPROM_DATE);
	HAL_Delay(20);
	
	data = __TIME__;
	eeprom_write(hi2c,EEPROM_TIME, data);
	HAL_Delay(20);
	eeprom_read(hi2c, EEPROM_TIME);
	HAL_Delay(20);
	
	
	sprintf(string, "date: %s", eeprom.ver_date);
	LCD_DrawString(0,260,string);
	
	sprintf(string, "time: %s", eeprom.ver_time);
	LCD_DrawString(0,280,string);
}

void eeprom_read(I2C_HandleTypeDef *hi2c,eeprom_data data){
	uint16_t memaddress = 0;
	uint16_t memsize = 0;
	uint8_t* data_ptr;
	switch(data){
		case EEPROM_DATE:
			memaddress = eeprom.date_address;
			memsize = sizeof(eeprom.ver_date)/sizeof(eeprom.ver_date[0]);
			data_ptr = eeprom.ver_date;
			break;
		case EEPROM_TIME:
			memaddress = eeprom.time_address;
			memsize = sizeof(eeprom.ver_time)/sizeof(eeprom.ver_time[0]);
			data_ptr = eeprom.ver_time;	
			break;
		case EEPROM_VOLUME:
			memaddress = eeprom.volume_address;
			memsize = sizeof(eeprom.volume_address);
			data_ptr = &eeprom.volume;
			break;
		default:
			return;
	}
	/*
	for(int i = 0; i < memsize; i++){
		HAL_I2C_Mem_Read(hi2c,EEPROM_DEVICE_ADDRESS,memaddress,1, data_ptr + i, 1, 50);
		HAL_Delay(10);
	}
	*/
	
	if(memsize > EEPROM_EACH_TX_BYTE){
		uint16_t temp_size = memsize;
		for(int i = 0; i < memsize/EEPROM_EACH_TX_BYTE; i++){
			HAL_I2C_Mem_Read(hi2c,EEPROM_DEVICE_ADDRESS,memaddress + i*EEPROM_EACH_TX_BYTE,1, data_ptr + i*EEPROM_EACH_TX_BYTE, EEPROM_EACH_TX_BYTE, 50);
			temp_size -= EEPROM_EACH_TX_BYTE;
			HAL_Delay(10);
		}
		if(temp_size > 0){
			HAL_I2C_Mem_Read(hi2c,EEPROM_DEVICE_ADDRESS,memaddress + (memsize/EEPROM_EACH_TX_BYTE)*EEPROM_EACH_TX_BYTE,1, data_ptr + (memsize/EEPROM_EACH_TX_BYTE)*EEPROM_EACH_TX_BYTE, temp_size, 50);
		}
	}
	else{
		HAL_I2C_Mem_Read(hi2c,EEPROM_DEVICE_ADDRESS,memaddress,1, data_ptr, memsize, 50);
	}
	
}


void eeprom_write(I2C_HandleTypeDef *hi2c,eeprom_data data, uint8_t* data_buf){
	uint16_t memaddress = 0;
	uint16_t memsize = 0;
	
	switch(data){
		case EEPROM_DATE:
			memaddress = eeprom.date_address;
			memsize = sizeof(eeprom.ver_date)/sizeof(eeprom.ver_date[0]);
			break;
		case EEPROM_TIME:
			memaddress = eeprom.time_address;
			memsize = sizeof(eeprom.ver_time)/sizeof(eeprom.ver_time[0]);
			break;
		case EEPROM_VOLUME:
			memaddress = eeprom.volume_address;
			memsize = sizeof(eeprom.volume_address);
			break;
		default:
			return;
	}
	
	if(memsize > EEPROM_EACH_TX_BYTE){
		uint16_t temp_size = memsize;
		for(int i = 0; i < memsize/EEPROM_EACH_TX_BYTE; i++){
			HAL_I2C_Mem_Write(hi2c,EEPROM_DEVICE_ADDRESS,memaddress + i*EEPROM_EACH_TX_BYTE, 1, data_buf + i*EEPROM_EACH_TX_BYTE, EEPROM_EACH_TX_BYTE, 50);
			temp_size -= EEPROM_EACH_TX_BYTE;
			HAL_Delay(10);
		}
		if(temp_size > 0){
			HAL_I2C_Mem_Write(hi2c,EEPROM_DEVICE_ADDRESS,memaddress + (memsize/EEPROM_EACH_TX_BYTE)*EEPROM_EACH_TX_BYTE, 1, data_buf + (memsize/EEPROM_EACH_TX_BYTE) * EEPROM_EACH_TX_BYTE, temp_size, 50);
		}
	}
	else{
		HAL_I2C_Mem_Write(hi2c,EEPROM_DEVICE_ADDRESS,memaddress,1, data_buf, memsize, 50);
	}
}

