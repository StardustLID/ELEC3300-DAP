#include <string.h>

#include "volume_bar.h"

void VOL_CreateVolBar() {
	uint16_t x1 = VOL_BAR_X0 + volumeBar.width - 1;
	uint16_t y1 = VOL_BAR_Y0 + volumeBar.height - 1;

	LCD_DrawString_Color(0, VOL_BAR_Y0 - HEIGHT_EN_CHAR, "Vol", DARK, CYAN);
	
	LCD_DrawLine(VOL_BAR_X0, VOL_BAR_Y0, x1, VOL_BAR_Y0, volumeBar.usColor);
	LCD_DrawLine(VOL_BAR_X0, VOL_BAR_Y0+1, VOL_BAR_X0, y1-1, volumeBar.usColor);
	LCD_DrawLine(x1, VOL_BAR_Y0+1, x1, y1-1, volumeBar.usColor);
	LCD_DrawLine(VOL_BAR_X0, y1, x1, y1, volumeBar.usColor);

	_updateVolText(0);
}

void VOL_UpdateVolBar(uint8_t initVol, bool isIncrease) {
	uint16_t barBottomPage = volumeBar.pos.y + volumeBar.height - 1;
	if (isIncrease) {
		if (initVol >= 100) return;
		for (uint8_t i = 0; i < LEVEL_HEIGHT; i++) {
			const uint16_t vol_y = barBottomPage - LEVEL_HEIGHT*initVol - i - 1;
			LCD_DrawLine(VOL_BAR_UPDATE_X0, vol_y, VOL_BAR_UPDATE_X1, vol_y, volumeBar.usColor);
			HAL_Delay(UPDATE_VOL_BAR_DELAY);
		}
		_updateVolText(initVol + 1);
	} else {
		if (initVol <= 0) return;
		for (uint8_t i = 0; i < LEVEL_HEIGHT; i++) {
			const uint16_t vol_y = barBottomPage - LEVEL_HEIGHT*initVol + i;
			LCD_DrawLine(VOL_BAR_UPDATE_X0, vol_y, VOL_BAR_UPDATE_X1, vol_y, DARK);
			HAL_Delay(UPDATE_VOL_BAR_DELAY);
		}
		_updateVolText(initVol - 1);
	}
	
}

static void _updateVolText(uint8_t newVol) {
	char vol_str[3];
	sprintf(vol_str, "%03d", newVol);
	LCD_DrawString_Color(0, VOL_BAR_Y0 + volumeBar.height, vol_str, DARK, CYAN);
}
