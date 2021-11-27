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
	EEPROM_VOLUME,
	EEPROM_EQ_ENA,
	EEPROM_EQ1,
	EEPROM_EQ2,
	EEPROM_EQ3,
	EEPROM_EQ4,
	EEPROM_EQ5
}eeprom_data;

typedef struct eeprom_struct{
	uint8_t ver_date[DATE_LENGTH];
	uint16_t date_address;
	uint8_t ver_time[TIME_LENGTH];
	uint16_t time_address;
	uint8_t volume;
	uint16_t volume_address;
	uint8_t eq_ena;
	uint16_t eq_ena_address;
	uint8_t eq1;
	uint16_t eq1_address;
	uint8_t eq2;
	uint16_t eq2_address;
	uint8_t eq3;
	uint16_t eq3_address;
	uint8_t eq4;
	uint16_t eq4_address;
	uint8_t eq5;
	uint16_t eq5_address;
}eeprom_struct;

void eeprom_init(I2C_HandleTypeDef *hi2c);
void eeprom_read(I2C_HandleTypeDef *hi2c,eeprom_data data);
void eeprom_write(I2C_HandleTypeDef *hi2c,eeprom_data data, uint8_t* data_buf);
void eeprom_reset_eq_value();

uint8_t get_eeprom_volume(void);
int8_t get_eeprom_eq1(void);
int8_t get_eeprom_eq2(void);
int8_t get_eeprom_eq3(void);
int8_t get_eeprom_eq4(void);
int8_t get_eeprom_eq5(void);

#endif