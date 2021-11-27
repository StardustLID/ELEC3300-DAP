#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "uart.h"
#include "codec.h"
#include "eeprom.h"

uint8_t uart1_rx_buffer[UART_RX_BUF_SIZE];
uint8_t uart1_rx_byte;
uint8_t uart1_rx_cnt = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	UNUSED(huart);
	
	uart1_rx_buffer[uart1_rx_cnt++] =  uart1_rx_byte;
	if(uart1_rx_buffer[uart1_rx_cnt - 1] == 0x0A && uart1_rx_buffer[uart1_rx_cnt - 2] == 0x0D){
		if(strncmp(UART_VOL_CMD, uart1_rx_buffer, 3) == 0){
			if(uart1_rx_buffer[3] == '?'){
				char str[4];
				sprintf(str,"%d",get_eeprom_volume() - 100);
				HAL_UART_Transmit(huart, str, strlen(str), 20);
				HAL_UART_Transmit(huart, "\n", 1, 20);
			}
			else{
				uint8_t num = atoi((uart1_rx_buffer + 4)) + 100;
				codec_volume_update(&hi2c1, num);
				HAL_UART_Transmit(huart, UART_OK, 3, 20);
			}
		}
		else if(strncmp(UART_MUTE_CMD, uart1_rx_buffer, 4) == 0){
		
		}
		else if(strncmp(UART_NEXT_SONG_CMD, uart1_rx_buffer, 5) == 0){
		
		}
		else if(strncmp(UART_PREVIOUS_SONG_CMD, uart1_rx_buffer, 5) == 0){
		
		}
		else if(strncmp(UART_PLAY_SONG_CMD, uart1_rx_buffer, 4) == 0){
			codec_play_song();
			HAL_UART_Transmit(huart, UART_OK, 3, 20);
		}
		else if(strncmp(UART_REPLAY_CMD, uart1_rx_buffer, 6) == 0){
		
		}
		else if(strncmp(UART_PAUSE_SONG_CMD, uart1_rx_buffer, 5) == 0){
			codec_pause_song();
			HAL_UART_Transmit(huart, UART_OK, 3, 20);
		}
		else if(strncmp(UART_EQ_ENA_CMD, uart1_rx_buffer, 2) == 0){
			if(uart1_rx_buffer[2] == '?'){
				char str[4];
				sprintf(str,"%d",get_eeprom_eq_ena());
				HAL_UART_Transmit(huart, str, strlen(str), 20);
				HAL_UART_Transmit(huart, "\n", 1, 20);
			}
			else{
				uint8_t num = atoi((uart1_rx_buffer + 3));
				codec_enable_eq();
				HAL_UART_Transmit(huart, UART_OK, 3, 20);
			}
		}
		else if(strncmp(UART_EQ1_CMD, uart1_rx_buffer, 3) == 0){
			if(uart1_rx_buffer[3] == '?'){
				char str[4];
				sprintf(str,"%d",get_eeprom_eq1());
				HAL_UART_Transmit(huart, str, strlen(str), 20);
				HAL_UART_Transmit(huart, "\n", 1, 20);
			}
			else{
				uint8_t num = atoi((uart1_rx_buffer + 4));
				codec_eq(num, get_eeprom_eq2(), get_eeprom_eq3(), get_eeprom_eq4(), get_eeprom_eq5());
				HAL_UART_Transmit(huart, UART_OK, 3, 20);
			}
		}
		else if(strncmp(UART_EQ2_CMD, uart1_rx_buffer, 3) == 0){
			if(uart1_rx_buffer[3] == '?'){
				char str[4];
				sprintf(str,"%d",get_eeprom_eq2());
				HAL_UART_Transmit(huart, str, strlen(str), 20);
				HAL_UART_Transmit(huart, "\n", 1, 20);
			}
			else{
				uint8_t num = atoi((uart1_rx_buffer + 4));
				codec_eq(get_eeprom_eq1(), num, get_eeprom_eq3(), get_eeprom_eq4(), get_eeprom_eq5());
				HAL_UART_Transmit(huart, UART_OK, 3, 20);
			}
		}
		else if(strncmp(UART_EQ3_CMD, uart1_rx_buffer, 3) == 0){
			if(uart1_rx_buffer[3] == '?'){
				char str[4];
				sprintf(str,"%d",get_eeprom_eq3());
				HAL_UART_Transmit(huart, str, strlen(str), 20);
				HAL_UART_Transmit(huart, "\n", 1, 20);
			}
			else{
				uint8_t num = atoi((uart1_rx_buffer + 4));
				codec_eq(get_eeprom_eq1(), get_eeprom_eq2(), num, get_eeprom_eq4(), get_eeprom_eq5());
				HAL_UART_Transmit(huart, UART_OK, 3, 20);
			}
		}
		else if(strncmp(UART_EQ4_CMD, uart1_rx_buffer, 3) == 0){
			if(uart1_rx_buffer[3] == '?'){
				char str[4];
				sprintf(str,"%d",get_eeprom_eq4());
				HAL_UART_Transmit(huart, str, strlen(str), 20);
				HAL_UART_Transmit(huart, "\n", 1, 20);
			}
			else{
				uint8_t num = atoi((uart1_rx_buffer + 4));
				codec_eq(get_eeprom_eq1(), get_eeprom_eq2(), get_eeprom_eq3(), num, get_eeprom_eq5());
				HAL_UART_Transmit(huart, UART_OK, 3, 20);
			}
		}
		else if(strncmp(UART_EQ5_CMD, uart1_rx_buffer, 3) == 0){
			if(uart1_rx_buffer[3] == '?'){
				char str[4];
				sprintf(str,"%d",get_eeprom_eq5());
				HAL_UART_Transmit(huart, str, strlen(str), 20);
				HAL_UART_Transmit(huart, "\n", 1, 20);
			}
			else{
				uint8_t num = atoi((uart1_rx_buffer + 4));
				codec_eq(get_eeprom_eq1(), get_eeprom_eq2(), get_eeprom_eq3(), get_eeprom_eq4(), num);
				HAL_UART_Transmit(huart, UART_OK, 3, 20);
			}
		}
		else if(strncmp(UART_EQ_REST, uart1_rx_buffer, 3) == 0){
			if(uart1_rx_buffer[4] == '?'){
				char str[4];
				sprintf(str,"%d",get_eeprom_eq5());
				HAL_UART_Transmit(huart, str, strlen(str), 20);
				HAL_UART_Transmit(huart, "\n", 1, 20);
			}
			else{
				codec_eq(12, 12, 12, 12, 12);
				HAL_UART_Transmit(huart, UART_OK, 3, 20);
			}
		}
		uart1_rx_cnt = 0;
	}
	HAL_UART_Receive_IT(&huart1, &uart1_rx_byte, 1);
}
