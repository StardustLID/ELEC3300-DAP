#include "eeprom.h"
#include "main.h"
#include "lcdtp.h"
#include <string.h>
#include "codec.h"

eeprom_struct eeprom;

char string[15];

void eeprom_init(I2C_HandleTypeDef *hi2c){
	eeprom.date_address = 0;
	eeprom.time_address = sizeof(eeprom.ver_date)/sizeof(eeprom.ver_date[0]);
	eeprom.volume_address = sizeof(eeprom.ver_time)/sizeof(eeprom.ver_time[0]) + eeprom.time_address;
	eeprom.eq_ena_address = sizeof(eeprom.volume) + eeprom.volume_address;
	eeprom.eq1_address = sizeof(eeprom.eq1) + eeprom.eq_ena_address;
	eeprom.eq2_address = sizeof(eeprom.eq2) + eeprom.eq1_address;
	eeprom.eq3_address = sizeof(eeprom.eq3) + eeprom.eq2_address;
	eeprom.eq4_address = sizeof(eeprom.eq4) + eeprom.eq3_address;
	eeprom.eq5_address = sizeof(eeprom.eq4) + eeprom.eq4_address;

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

	eeprom_read(hi2c,EEPROM_VOLUME);
	HAL_Delay(20);
<<<<<<< HEAD
	
	codec_volume_update(&hi2c1,eeprom.volume);
	HAL_Delay(5);
	eeprom_read(&hi2c2,EEPROM_EQ_ENA);
	HAL_Delay(5);
	eeprom_read(&hi2c2,EEPROM_EQ1);
	HAL_Delay(5);
	eeprom_read(&hi2c2,EEPROM_EQ2);
	HAL_Delay(5);
	eeprom_read(&hi2c2,EEPROM_EQ3);
	HAL_Delay(5);
	eeprom_read(&hi2c2,EEPROM_EQ4);
	HAL_Delay(5);
	eeprom_read(&hi2c2,EEPROM_EQ5);
	HAL_Delay(5);
	
	//uint8_t buf = 0;
	//HAL_I2C_Mem_Read(hi2c,EEPROM_DEVICE_ADDRESS,eeprom.eq_ena_address,1, &buf, 1, 50);
	
	sprintf(string, "eq_ena: %d", eeprom.eq_ena);
	LCD_DrawString(0,140,string);
	
	sprintf(string, "band1: %d", eeprom.eq1);
	LCD_DrawString(0,160,string);
	
	sprintf(string, "band2: %d", eeprom.eq2);
	LCD_DrawString(0,180,string);
	
	sprintf(string, "band3: %d", eeprom.eq3);
	LCD_DrawString(0,200,string);
	
	sprintf(string, "band4: %d", eeprom.eq4);
	LCD_DrawString(0,220,string);
	
	sprintf(string, "band5: %d", eeprom.eq5);
	LCD_DrawString(0,240,string);
=======
	
	codec_volume_update(&hi2c1,eeprom.volume);
>>>>>>> 2b88573e3b7dfbb2e2266b9512a21764ad2f75f9
	
	sprintf(string, "date: %s", eeprom.ver_date);
	LCD_DrawString(0,260,string);
	
	sprintf(string, "time: %s", eeprom.ver_time);
	LCD_DrawString(0,280,string);

	sprintf(string, "vol: %d", eeprom.volume);
	LCD_DrawString(0,300,string);
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
			memsize = sizeof(eeprom.volume);
			data_ptr = &eeprom.volume;
			break;
		case EEPROM_EQ_ENA:
			memaddress = eeprom.eq_ena_address;
			memsize = sizeof(eeprom.eq_ena);
			data_ptr = &eeprom.eq_ena;
			break;
		case EEPROM_EQ1:
			memaddress = eeprom.eq1_address;
			memsize = sizeof(eeprom.eq1);
			data_ptr = &eeprom.eq1;
			break;
		case EEPROM_EQ2:
			memaddress = eeprom.eq2_address;
			memsize = sizeof(eeprom.eq2);
			data_ptr = &eeprom.eq2;
			break;
		case EEPROM_EQ3:
			memaddress = eeprom.eq3_address;
			memsize = sizeof(eeprom.eq3);
			data_ptr = &eeprom.eq3;
			break;
		case EEPROM_EQ4:
			memaddress = eeprom.eq4_address;
			memsize = sizeof(eeprom.eq4);
			data_ptr = &eeprom.eq4;
			break;
		case EEPROM_EQ5:
			memaddress = eeprom.eq5_address;
			memsize = sizeof(eeprom.eq5);
			data_ptr = &eeprom.eq5;
			break;
		default:
			return;
	}
	
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
<<<<<<< HEAD
			memsize = sizeof(eeprom.volume);
			eeprom.volume = *data_buf;
			break;
		case EEPROM_EQ_ENA:
			memaddress = eeprom.eq_ena_address;
			memsize = sizeof(eeprom.eq_ena);
			eeprom.eq_ena = *data_buf;
			break;
		case EEPROM_EQ1:
			memaddress = eeprom.eq1_address;
			memsize = sizeof(eeprom.eq1);
			eeprom.eq1 = *data_buf;
			break;
		case EEPROM_EQ2:
			memaddress = eeprom.eq2_address;
			memsize = sizeof(eeprom.eq2);
			eeprom.eq2 = *data_buf;
			break;
		case EEPROM_EQ3:
			memaddress = eeprom.eq3_address;
			memsize = sizeof(eeprom.eq3);
			eeprom.eq3 = *data_buf;
			break;
		case EEPROM_EQ4:
			memaddress = eeprom.eq4_address;
			memsize = sizeof(eeprom.eq4);
			eeprom.eq4 = *data_buf;
			break;
		case EEPROM_EQ5:
			memaddress = eeprom.eq5_address;
			memsize = sizeof(eeprom.eq5);
			eeprom.eq5 = *data_buf;
=======
			memsize = sizeof(eeprom.volume_address);
			eeprom.volume = *data_buf;
>>>>>>> 2b88573e3b7dfbb2e2266b9512a21764ad2f75f9
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

<<<<<<< HEAD
uint8_t get_eeprom_volume(){
	return eeprom.volume;
}

uint8_t get_eeprom_eq_ena(){
	return eeprom.eq_ena;
}

int8_t get_eeprom_eq1(){
	return eeprom.eq1;
}

int8_t get_eeprom_eq2(){
	return eeprom.eq2;
}

int8_t get_eeprom_eq3(){
	return eeprom.eq3;
}

int8_t get_eeprom_eq4(){
	return eeprom.eq4;
}

int8_t get_eeprom_eq5(){
	return eeprom.eq5;
}
=======
/*
void update_eerprom_data(eeprom_data data, uint32_t value){
	switch(data){
		case EEPROM_DATE:	break;
		case EEPROM_TIME: break;
		case EEPROM_VOLUME: eeprom.volume = (uint8_t)value; break;
		default: break
	}
}
*/

uint8_t get_eeprom_volume(){
	return eeprom.volume;
}
>>>>>>> 2b88573e3b7dfbb2e2266b9512a21764ad2f75f9
