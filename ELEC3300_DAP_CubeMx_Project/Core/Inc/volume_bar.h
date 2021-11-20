#ifndef __VOLUME_BAR_H
#define	__VOLUME_BAR_H

#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "lcdtp.h"
#include "point.h"

#define VOL_BAR_X0 7
#define VOL_BAR_Y0 59
#define VOL_BAR_UPDATE_X0 8 // VOL_BAR_X0 + 1
#define VOL_BAR_UPDATE_X1 15 // VOL_BAR_X0 + WIDTH_EN_CHAR - 3
#define LEVEL_HEIGHT 2
#define UPDATE_VOL_BAR_DELAY 7

typedef struct {
	const Point pos;
    const uint16_t height;
    const uint16_t width;
	const uint16_t usColor;
	uint8_t volume;
} VolumeBar;

static const VolumeBar volumeBar = {
	.pos = {VOL_BAR_X0, VOL_BAR_Y0},
    .height = 202, // 2 px per interval, 1 px for box
    .width = WIDTH_EN_CHAR + 2,
	.usColor = CYAN
};

/**
 * create a volume bar with zero volume
 */
void VOL_CreateVolBar(void);

/**
 * render the change in volume bar
 */
void VOL_UpdateVolBar(uint8_t initVol, bool isIncrease);

/**
 * private: render the change in volume text
 */
static void _updateVolText(uint8_t newVol);

#endif
