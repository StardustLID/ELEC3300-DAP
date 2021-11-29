#ifndef __SWITCH_H
#define	__SWITCH_H

#include "stm32f4xx_hal.h"

#define LONG_PRESS_DELAY 300

typedef struct {
	GPIO_PinState buttonState;     // current state of the button
	GPIO_PinState lastButtonState; // previous state of the button
	uint32_t startPressed;    // the moment the button was pressed
	uint32_t endPressed;      // the moment the button was released
	uint32_t holdTime;        // how long the button was hold
	uint32_t idleTime;        // how long the button was idle
} SwitchState;

extern volatile SwitchState switches[3];

void updateSwitch(uint8_t id);

#endif
