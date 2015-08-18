/* nick_ledrainbow.c
 * Plain nick animation that shows a moving rainbow on the rad1o's antenna
 * (needs the ws2812 leds populated)
 *
 * By Nervengift
 * rad1o@nerven.gift
 *
 * Known bugs:
 * - openingg the main menu does not always work at first try
 */
#include <r0ketlib/display.h>
#include <r0ketlib/config.h>
#include <r0ketlib/print.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/itoa.h>
#include <rad1olib/pins.h>

#include <rad1olib/light_ws2812_cortex.h>
#include <rad1olib/setup.h>
#include <r0ketlib/display.h>

#include <stdio.h>
#include <string.h>
#include "usetable.h"

// #define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))

typedef struct {
	uint8_t r, g, b;
} Color;

void ram(){
	uint8_t pattern[24];
	int offset = 0;
	int point = 0;
	int dx=0;
	int dy=0;
	Color col;
	col.r = 255;
	col.g = 0;
	col.b = 0;

	uint8_t dims[] = {0.5, 0.3, 0.2, 0.1};
	// int dimsSize = ARRAY_SIZE(dims);
	int dimsSize = 4;
	// convert to char
	char *dimsSizeStr;
	sprintf(dimsSizeStr, "%d", dimsSize);

	SETUPgout(RGB_LED);

	setExtFont(GLOBAL(nickfont));
	setTextColor(GLOBAL(nickbg),GLOBAL(nickfg));
	// dx=DoString(0,0,GLOBAL(nickname));
	dx=DoString(0,0,dimsSizeStr);
	lcdFill(GLOBAL(nickbg));
	dx=(RESX-dx)/2;
	if(dx<0)
		dx=0;
	dy=(RESY-getFontHeight())/2;
	lcdSetCrsr(dx,dy);
	// lcdPrint(GLOBAL(nickname));
	lcdPrint(dimsSizeStr);
	lcdDisplay();

	pattern[0] = 0;
	pattern[1] = 0;
	pattern[2] = 0;
	pattern[3] = 0;
	pattern[4] = 0;
	pattern[5] = 0;

	while (1) {
		for (int i = 0; i < 6; i++) {
			float dim = dims[(point + offset) % dimsSize];
			pattern[3*i+1+6] = col.r * dim;
			pattern[3*i+0+6] = col.g * dim;
			pattern[3*i+2+6] = col.b * dim;
			point++;
		}
		ws2812_sendarray(pattern, sizeof(pattern));
		offset++;
		if(getInput() != BTN_NONE) return;
		delayms_queue_plus(50, 0);
	}
	/*
	while (1) {
		for (int i = 0; i < 6; i++) {
			col=wheel((-offset+30 * i)%255);
			pattern[3*i+0+6] = col.g * DIM;
			pattern[3*i+1+6] = col.r * DIM;
			pattern[3*i+2+6] = col.b * DIM;
		}
		ws2812_sendarray(pattern, sizeof(pattern));
		offset+=3;
		if(getInput() != BTN_NONE) return;
		delayms_queue_plus(50, 0);
	}
	*/
	return;
};