#ifndef __VOLUME_BAR_H
#define	__VOLUME_BAR_H

#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "lcdtp.h"
#include "point.h"

#define LEVEL_HEIGHT 2

typedef struct {
	const Point pos;
    const uint16_t height;
    const uint16_t width;
	const uint16_t usColor;
	uint8_t volume;
} VolumeBar;

static VolumeBar volumeBar = {
	.pos = {7, 59},
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
 * render the change in volume text
 */
// void VOL_UpdateVolText(void);

#endif
