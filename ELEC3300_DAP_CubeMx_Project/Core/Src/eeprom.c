#include "eeprom.h"
#include "main.h"
#include "lcdtp.h"
#include <string.h>
#include "codec.h"

eeprom_struct eeprom = {0};

char string[15];

void eeprom_init(I2C_HandleTypeDef *hi2c){
	//eeprom_earse_all();
	
	eeprom.date_address = 0;
	eeprom.time_address = sizeof(eeprom.ver_date)/sizeof(eeprom.ver_date[0]);
	eeprom.volume_address = sizeof(eeprom.ver_time)/sizeof(eeprom.ver_time[0]) + eeprom.time_address;
	eeprom.eq_ena_address = sizeof(eeprom.volume) + eeprom.volume_address;
	eeprom.eq1_address = sizeof(eeprom.eq1) + eeprom.eq_ena_address;
	eeprom.eq2_address = sizeof(eeprom.eq2) + eeprom.eq1_address;
	eeprom.eq3_address = sizeof(eeprom.eq3) + eeprom.eq2_address;
	eeprom.eq4_address = sizeof(eeprom.eq4) + eeprom.eq3_address;
	eeprom.eq5_address = sizeof(eeprom.eq4) + eeprom.eq4_address;
	eeprom.num_of_playlist_address = sizeof(eeprom.num_of_playlist) + eeprom.eq5_address;

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
	eeprom_read(&hi2c2,EEPROM_NUM_OF_PLAYLIST);
	HAL_Delay(5);
	
	sprintf(string, "Num: %d", eeprom.num_of_playlist);
	LCD_DrawString(0,260, string);
	
	sprintf(string, "Date: %s", eeprom.ver_date);
	LCD_DrawString(0,280, string);
	
	sprintf(string, "TIme: %s", eeprom.ver_time);
	LCD_DrawString(0,300, string);
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
		case EEPROM_NUM_OF_PLAYLIST:
			memaddress = eeprom.num_of_playlist_address;
			memsize = sizeof(eeprom.num_of_playlist);
			data_ptr = &eeprom.num_of_playlist;
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
			break;
		case EEPROM_NUM_OF_PLAYLIST:
			memaddress = eeprom.num_of_playlist_address;
			memsize = sizeof(eeprom.num_of_playlist_address);
			eeprom.num_of_playlist = *data_buf;
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

uint8_t get_eeprom_volume(){
	return eeprom.volume;
}


void eeprom_earse_all(){
	uint8_t buf = 0;
	for(int i = 0; i < 256; i++){
		HAL_I2C_Mem_Write(&hi2c2, EEPROM_DEVICE_ADDRESS, i, 1, &buf,1,50);
		HAL_Delay(5);
	}
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

uint8_t get_eeprom_num_of_playlist(){
	return eeprom.num_of_playlist;
}

uint16_t get_eeprom_num_of_playlist_address(){
	return eeprom.num_of_playlist_address;
}
