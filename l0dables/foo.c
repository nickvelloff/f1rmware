#include <r0ketlib/display.h>
#include <r0ketlib/config.h>
#include <r0ketlib/print.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/itoa.h>
#include <rad1olib/pins.h>

#include <rad1olib/light_ws2812_cortex.h>
#include <rad1olib/setup.h>
#include <r0ketlib/display.h>

#include "usetable.h"

#define DIM 0.1

typedef struct {
	uint8_t r, g, b;
} Color;

Color wheel(uint8_t pos);

// 0, 0, 1, .7, .4, .1       0, 1, 2, 3, 4, 5
// 0, 1, .7, .4, .1. 0       1, 2, 3, 4, 5, 0

void ram(){
	float dim[] {0, 0, 1, 0.7, 0.4, 0.1};
	uint8_t pattern[24];
	int offset = 0;
	int dx=0;
	int dy=0;

	uint8_t color[] = {0, 0, 0};
	Color col;
	SETUPgout(RGB_LED);

	setExtFont(GLOBAL(nickfont));
	setTextColor(GLOBAL(nickbg),GLOBAL(nickfg));
	dx=DoString(0,0,GLOBAL(nickname));
	lcdFill(GLOBAL(nickbg));
	dx=(RESX-dx)/2;
	if(dx<0)
		dx=0;
	dy=(RESY-getFontHeight())/2;
	lcdSetCrsr(dx,dy);
	lcdPrint(GLOBAL(nickname));
	lcdDisplay();

	offset=0;
	// first 2 leds:
	// led 1:
	pattern[0] = 0 * DIM;
	pattern[1] = 0 * DIM;
	pattern[2] = 0 * DIM;
	// led 2:
	pattern[3] = 0 * DIM;
	pattern[4] = 0 * DIM;
	pattern[5] = 0 * DIM;
	int cdim = 0;
	int ndim = sizeof(dim);
	int start = 0;
	// default color
	col.g = 0;
	col.r = 255;
	col.b = 0;
	while (1) {
		// update all leds with new dims
		// for each one of the 6 leds on the antenna
		cdim = 0;
		for (int i = 0; i < 6; i++) {
			//col=wheel((-offset+30 * i)%255);

			pattern[3*i+0+6] = col.g * dim[start + (cdim % ndim)];
			pattern[3*i+1+6] = col.r * dim[start + (cdim % ndim)];
			pattern[3*i+2+6] = col.b * dim[start + (cdim % ndim)];
			cdim++;
		}
		// update the dim
		start += start / ndim;
		ws2812_sendarray(pattern, sizeof(pattern));
		//offset+=3;
		if(getInput() != BTN_NONE) return;
		delayms_queue_plus(50, 0);
	}
	return;
};

Color wheel(uint8_t pos) {
	Color c;
	if (pos < 85) {
		c = (Color) {pos * 3, 255 - pos*3, 0};
	} else if (pos < 170) {
		pos -= 85;
		c = (Color) {255 - pos*3, 0, pos * 3};
	} else {
		pos -= 170;
		c = (Color) {0, pos * 3, 255 - pos*3};
	}
	return c;
}
