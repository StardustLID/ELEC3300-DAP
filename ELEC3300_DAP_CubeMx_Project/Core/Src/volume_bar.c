#include "volume_bar.h"

// TODO: make macros
void VOL_CreateVolBar() {
	uint16_t x0 = volumeBar.pos.x;
	uint16_t x1 = x0 + volumeBar.width - 1;
	uint16_t y0 = volumeBar.pos.y;
	uint16_t y1 = y0 + volumeBar.height - 1;

	LCD_DrawLine(x0, y0, x1, y0, volumeBar.usColor);
	LCD_DrawLine(x0, y0+1, x0, y1-1, volumeBar.usColor);
	LCD_DrawLine(x1, y0+1, x1, y1-1, volumeBar.usColor);
	LCD_DrawLine(x0, y1, x1, y1, volumeBar.usColor);
}

void VOL_UpdateVolBar(uint8_t initVol, bool isIncrease) {
	uint16_t x0 = volumeBar.pos.x + 1;
	uint16_t x1 = volumeBar.pos.x + volumeBar.width - 2;
	uint16_t barBottomPage = volumeBar.pos.y + volumeBar.height - 1;
	if (isIncrease) {
		if (initVol >= 100) return;
		for (uint8_t i = 0; i < LEVEL_HEIGHT; i++) {
			const uint16_t vol_y = barBottomPage - LEVEL_HEIGHT*initVol - i - 1;
			LCD_DrawLine(x0, vol_y, x1, vol_y, volumeBar.usColor);
			HAL_Delay(5);
		}
	} else {
		if (initVol <= 0) return;
		for (uint8_t i = 0; i < LEVEL_HEIGHT; i++) {
			const uint16_t vol_y = barBottomPage - LEVEL_HEIGHT*initVol + i;
			LCD_DrawLine(x0, vol_y, x1, vol_y, DARK);
			HAL_Delay(5);
		}
	}
}