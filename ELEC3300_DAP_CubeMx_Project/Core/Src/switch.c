#include <stdio.h>
#include <string.h>

#include "switch.h"

volatile SwitchState switches[3] = {0};

void updateSwitch(uint8_t id) {
	// the button has been just pressed
	if (switches[id].buttonState == GPIO_PIN_RESET) {
		switches[id].startPressed = HAL_GetTick();
		switches[id].idleTime = switches[id].startPressed - switches[id].endPressed;

		// if (idleTime >= 500 && idleTime < 1000) {
		// 	Serial.println("Button was idle for half a second");
		// }

		// if (idleTime >= 1000) {
		// 	Serial.println("Button was idle for one second or more"); 
		// }

	// the button has been just released
	} else {
		switches[id].endPressed = HAL_GetTick();
		switches[id].holdTime = switches[id].endPressed - switches[id].startPressed;

		// if (holdTime >= 500 && holdTime < 1000) {
		// 	Serial.println("Button was held for half a second"); 
		// }

		// if (holdTime >= 1000) {
		// 	Serial.println("Button was held for one second or more"); 
		// }
	}
}