//-----------------------------LICENSE NOTICE------------------------------------
//
//  /$$$$$$$                /$$$$$$$            /$$                           /$$
// | $$__  $$              | $$__  $$          | $$                          | $$
// | $$  \ $$  /$$$$$$     | $$  \ $$  /$$$$$$ | $$  /$$$$$$  /$$$$$$$   /$$$$$$$
// | $$  | $$ /$$__  $$    | $$$$$$$/ /$$__  $$| $$ |____  $$| $$__  $$ /$$__  $$
// | $$  | $$| $$  \__/    | $$__  $$| $$  \ $$| $$  /$$$$$$$| $$  \ $$| $$  | $$
// | $$  | $$| $$          | $$  \ $$| $$  | $$| $$ /$$__  $$| $$  | $$| $$  | $$
// | $$$$$$$/| $$       /$$| $$  | $$|  $$$$$$/| $$|  $$$$$$$| $$  | $$|  $$$$$$$
// |_______/ |__/      |__/|__/  |__/ \______/ |__/ \_______/|__/  |__/ \_______/
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include <cpctelera.h>
#include <stdio.h>
#include "../defines.h"
#include "util.h"
#include "../keyboard/keyboard.h"
#include "../text/text.h"

//////////////////////////////////////////////////////////////////
// clearScreen
// Returns: 
//		void
//////////////////////////////////////////////////////////////////

void clearScreen(u8 bgColor)
{
	// Clear Screen
	cpct_memset(CPCT_VMEM_START, cpct_px2byteM0(bgColor, bgColor), 0x4000);
}

//////////////////////////////////////////////////////////////////
// drawWindow
// Returns: 
//		void
//////////////////////////////////////////////////////////////////
void drawWindow(u8 x, u8 y, u8 width, u8 height)
{
	u8 *pvideo;
	u8 fgColor = 15;
	u8 bgColor = 0;

	// top and bottom fgColor horizontal lines
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, fgColor), width - 4, 2);
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y + height);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, fgColor), width - 4, 2);
	// top and bottom BG_COLOR horizontal lines
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y + 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(BG_COLOR, BG_COLOR), width - 4, 2);
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y + height - 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(BG_COLOR, BG_COLOR), width - 4, 2);
	// Internal box
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y + 4);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, bgColor), width - 4, height - 6);

	// top left corner
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y + 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, 2);

	// left vertical line
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y + 4);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, BG_COLOR), 1, height - 6);

	//bottom left corner
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y + height - 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, 2);
	// top right corner
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - 3, y + 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, BG_COLOR), 1, 2);
	// right vertical line
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - 3, y + 4);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, height - 6);
	// bottom right corner
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - 3, y + height - 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, BG_COLOR), 1, 2);
}

/////////////////////////////////////////////////////////////////
// drawBottleNeck
// Returns: 
//		void
/////////////////////////////////////////////////////////////////
void drawBottleNeck(u8 x, u8 y, u8 width, u8 height, u8 fgColor, u8 bgColor)
{
	u8 *pvideo;

	// Internal box
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y + 4);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, bgColor), width - 1, (height / 2) - 1);
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1 + (width / 4), y + 3 + (height / 2));
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, bgColor), (width / 2) - 1, (height / 2) - 4);

	// top and bottom fgColor horizontal lines
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, fgColor), width - 2, 2);

	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y + (height / 2) + 3);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, fgColor), (width / 4), 2);
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - (width / 4) - 1, y + (height / 2) + 3);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, fgColor), (width / 4), 2);

	// top and bottom BG_COLOR horizontal lines
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y + 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, bgColor), width - 1, 2);

	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1, y + (height / 2) + 5);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, bgColor), (width / 4), 2);
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - (width / 4) - 1, y + (height / 2) + 5);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, bgColor), (width / 4), 2);

	// top left connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y + 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, fgColor), 1, 2);

	// left vertical line
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y + 4);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, BG_COLOR), 1, (height / 2) - 2);

	// bottom left connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y + (height / 2) + 1);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, fgColor), 1, 2);

	// top right connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - 1, y + 2);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, bgColor), 1, 2);

	// right vertical line
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - 1, y + 4);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(bgColor, fgColor), 1, (height / 2) - 2);

	// bottom right connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - 1, y + (height / 2) + 1);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, bgColor), 1, 2);

	// Second half of bottleneck

	// top left connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1 + (width / 4), y + 5 + (height / 2));
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, BG_COLOR), 1, 2);

	// left vertical line
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1 + (width / 4), y + 7 + (height / 2));
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, (height / 2) - 12);

	// bottom left connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + 1 + (width / 4), y + height - 5);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, BG_COLOR), 1, 2);

	// top right connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - (width / 4) - 2, y + 5 + (height / 2));
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, 2);

	// right vertical line
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - (width / 4) - 2, y + 7 + (height / 2));
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(fgColor, BG_COLOR), 1, (height / 2) - 12);

	// bottom right connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x + width - (width / 4) - 2, y + height - 5);
	cpct_drawSolidBox(pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, 2);
}

/////////////////////////////////////////////////////////////////
// updateNumber
// Returns: 
//		void
/////////////////////////////////////////////////////////////////
void updateNumber(u8 number, u8 y)
{
	u8 *pvmem;
	u8 text[3];

	pvmem = cpct_getScreenPtr(SCR_VMEM, 58, y);
	cpct_drawSolidBox(pvmem, cpct_px2byteM0(14, 14), 4, 14);
	sprintf(text, "%02d", number);
	drawText(text, 58, y, COLORTXT_YELLOW, DOUBLEHEIGHT);
}

/////////////////////////////////////////////////////////////////
// resultNumber
// Returns: 
//		void
/////////////////////////////////////////////////////////////////

u8 resultNumber(u8 y, u8 start, u8 end)
{
	u8 selection;
	u8 changed;

	selection = start;
	drawText("UP/DOWN:CHANGE LEVEL", 16, y + 20, COLORTXT_MAUVE, NORMALHEIGHT);
	drawText("FIRE: CONFIRM", 16, y + 32, COLORTXT_MAUVE, NORMALHEIGHT);
	updateNumber(selection, y);
	while (1)
	{
		changed = NO;
		cpct_waitHalts(20);
		if ((cpct_isKeyPressed(keys1.up)) || (cpct_isKeyPressed(keys1.j_up)) || (cpct_isKeyPressed(Key_CursorUp)))
		{
			if (selection < end)
				selection++;
			else
				selection = start;
			changed = YES;
		}
		else if ((cpct_isKeyPressed(keys1.down)) || (cpct_isKeyPressed(keys1.j_down)) || (cpct_isKeyPressed(Key_CursorDown)))
		{
			if (selection > start)
				selection--;
			else 
				selection = end;
			changed = YES;
		}
		if (changed)
		{
			updateNumber(selection, y);
			changed = NO;
		}
		if ((cpct_isKeyPressed(keys1.fire1)) || (cpct_isKeyPressed(keys1.j_fire1)) || (cpct_isKeyPressed(keys1.j_fire2))
			|| (cpct_isKeyPressed(Key_Space)))
		{
			return selection;
		}
	}
}

/////////////////////////////////////////////////////////////////
// drawMessage
// Returns: 
//		void
/////////////////////////////////////////////////////////////////
void drawMessage(u8 *message, u8 type, u8 **address, u8 *xx, u8 *yy, u8 *width, u8 *height)
{
	u8 messageLength;
	u8 defaultMax;
	u8 x;
	u8 y; 
	u8 w; 
	u8 h;
	u8 *pvmem;
	u8 fgColor;
	u8 bgColor;

	// Window colors
	fgColor = 15; //White
	bgColor = 14; //Blue

	if (type == NUMBER)
		defaultMax = 56;
	else if (type == TEMPORAL)
	{
		defaultMax = 4;
		// Yellow background color for info
		fgColor = 15; // Light Green
		bgColor = 8;  // Green
	}
	else
		defaultMax = 32;

	messageLength = strLength(message);
	w = max(((messageLength * 2) + 7), defaultMax);
	// If it's a TEMPORAL Message reduce the height of the window
	h = 60 - ((type == TEMPORAL) * 20);
	x = (79 - w) / 2;

	// If don't have a predefined Y coord, set it to the center of the screen
	if (*yy == 0xff){
		y = (199 - h) / 2;
	} 
	else
	{
		y = *yy;
	}
	

	//Capture the portion of screen that will overwrite the message
	pvmem = cpct_getScreenPtr(CPCT_VMEM_START, x, y);

	cpct_getScreenToSprite(pvmem, (u8 *)&screenBuffer0, w, h);
	
	//Draw Message
	drawWindow(x, y, w, h - 2);
	drawText(message, x + 3, y + 12, COLORTXT_WHITE, DOUBLEHEIGHT);

	//Return data to restore screen
	*address = pvmem;
	*xx = x;
	*yy = y;
	*width = w;
	*height = h;
}

/////////////////////////////////////////////////////////////////
// showMessage
// Returns: 
//		void
/////////////////////////////////////////////////////////////////
u8 showMessage(u8 *message, u8 type)
{
	u8 result;
	u8 *address;
	u8 x,y,w,h;

	//Messages don0t have predefined y coord
	y = 0xff;

	// Draw window for message
	drawMessage(message, type, &address, &x, &y, &w, &h);

	// If it's a question I'll wait Y/N... otherwise any key
	if (type == YESNO)
	{
		drawText("(YES/NO)", x + ((w - 16) / 2), y + 38, COLORTXT_YELLOW, NORMALHEIGHT);

		while (1)
		{
			if ((cpct_isKeyPressed(Key_Y)) || (cpct_isKeyPressed(Key_N)))
			{
				result = (cpct_isKeyPressed(Key_Y));
				break;
			}
		}
	}
	else if (type == TEMPORAL)
	{
		cpct_waitHalts(100);
		result = YES;
	}
	else
	{
		drawText(" PRESS ANY KEY ", x + ((w - 30) / 2), y + 38, COLORTXT_YELLOW, NORMALHEIGHT);
		result = YES;
		wait4OneKey();
	}

	//Restore the portion of screen overwrited the message
	cpct_drawSprite((u8 *)0xb000, address, w, h);

	return result;
}

/////////////////////////////////////////////////////////////////
// getNumber
// Returns: 
//		void
/////////////////////////////////////////////////////////////////
u8 getNumber(u8 *message, u8 yy, u8 start, u8 end){
	u8 result;
	u8 *address;
	u8 x,y,w,h;

	y = yy;
	// Draw window for message
	drawMessage(message, NUMBER, &address, &x, &y, &w, &h);

	result = resultNumber(y+12, start, end);

	//Restore the portion of screen overwrited the message
	cpct_drawSprite((u8 *)0xb000, address, w, h);

	return result;
}

/////////////////////////////////////////////////////////////////
// drawCompressToScreen
// Returns: 
//		void
/////////////////////////////////////////////////////////////////
void drawCompressToScreen(u8 x, u8 y, u8 w, u8 h, u16 size, u8 *comp_end)
{
	u8 *pvmem;

	pvmem = cpct_getScreenPtr(SCR_VMEM, x, y);
	cpct_zx7b_decrunch_s((u8 *)&screenBuffer0 + size - 1, comp_end);
	cpct_drawSprite(&screenBuffer0, pvmem, w, h);
}