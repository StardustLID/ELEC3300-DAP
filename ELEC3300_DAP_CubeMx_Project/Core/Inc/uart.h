#ifndef _UART_H
#define _UART_H

#define UART_RX_BUF_SIZE	64

extern uint8_t uart1_rx_buffer[UART_RX_BUF_SIZE];
extern uint8_t uart1_rx_byte;

#define UART_VOL_CMD 	"vol"
#define UART_MUTE_CMD "mute"
#define UART_NEXT_SONG_CMD	"nsong"
#define UART_PREVIOUS_SONG_CMD "psong"
#define UART_PLAY_SONG_CMD "play"
#define UART_REPLAY_CMD "replay"
#define UART_PAUSE_SONG_CMD "pause"


#define UART_OK "OK\n"

#endif