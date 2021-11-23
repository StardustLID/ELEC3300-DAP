#ifndef _EEPROM_H
#define _EEPROM_H

#include <stdint.h>
#include "main.h"

#define EEPROM_DEVICE_ADDRESS 0xA0 // 0b1010 000 << 1

#define DATE_LENGTH 11//ex: 'Nov 22 2021' pad space if month less than 10
#define TIME_LENGTH 5//ex: '12:56:53'

#define EEPROM_EACH_TX_BYTE		8

typedef enum eeprom_data{
	EEPROM_DATE,
	EEPROM_TIME,
	EEPROM_VOLUME
}eeprom_data;

typedef struct eeprom_struct{
	uint8_t ver_date[DATE_LENGTH];
	uint16_t date_address;
	uint8_t ver_time[TIME_LENGTH];
	uint16_t time_address;
	uint8_t volume;
	uint16_t volume_address;
}eeprom_struct;

void eeprom_init(I2C_HandleTypeDef *hi2c);
void eeprom_read(I2C_HandleTypeDef *hi2c,eeprom_data data);
void eeprom_write(I2C_HandleTypeDef *hi2c,eeprom_data data, uint8_t* data_buf);

#endif