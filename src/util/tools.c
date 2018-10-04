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
#include "../defines.h"
#include "util.h"
#include "../sprites/border.h"
#include "../keyboard/keyboard.h"


//////////////////////////////////////////////////////////////////
// delay
//
//    Wait for an specific number of cycles
//
//
// Returns:
//    void
//
void delay(u32 cycles)
{
    u32 i;
    for (i = 0; i < cycles; i++)
    {
        __asm
        halt
        __endasm;
    }
}

//////////////////////////////////////////////////////////////////
// Name
//
//    Descriptio
//
//
// Returns:
//    

/*u8 hasReachedTarget(TEntity *e, u8 x, u8 y, i16 stepX, i16 stepY)
{
    u8 posX0, posY0;
    u8 posX1, posY1;

    posX0 = e->x[0] / SCALE;
    posY0 = e->y[0] / SCALE;
    posX1 = stepX / SCALE;
    posY1 = stepY / SCALE;

    //return ((((posX0 <= x) && (x <= posX1)) || ((posX0 >= x) && (x >= posX1))) &&
    //        (((posY0 <= y) && (y <= posY1)) || ((posY0 >= y) && (y >= posY1))));
    //return distance(posX0,posY0,posX1, posY1)<2;
    return (fast_abs(posX0 - x) <= fast_abs(posX1)) && (fast_abs(posY0 - y) <= fast_abs(posY1));
}
*/

//////////////////////////////////////////////////////////////////
// distance
//
//    Descriptio
//
//
// Returns:
//    

u16 distance (u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 x = fast_abs( x2 - x1 );  // x = valor absoluto de x2 – x1, es decir, la distancia entre las x
    u16 y = fast_abs( y2 - y1 );  // y = valor absoluto de y2 – y1, es decir, la distancia entre las y

    u16 min = x < y ? x : y; // Si x < y min = x, si no, vale y. Es decir, el menor de los 2

    return ( x + y - (min >> 1) - (min >> 2) + (min >> 4) );
}
//////////////////////////////////////////////////////////////////
// sign
//
//    Descriptio
//
//
// Returns:
//    

i16 sign(i16 x)
{
    return  (x > 0) - (x < 0);
}


//////////////////////////////////////////////////////////////////
// abs
//
//    Descriptio
//
//
// Returns:
//    
i16 abs(i16 j)
{
    return (j < 0) ? -j : j;
}

//////////////////////////////////////////////////////////////////
// clearScreen
//
//
//
// Returns:
//    void
//

void clearScreen(u8 bgColor) {
    // Clear Screen
    cpct_memset(CPCT_VMEM_START, cpct_px2byteM0(bgColor,bgColor), 0x4000);
}


/////////////////////////////////////////////////////////////////
// drawWindow
//
//
//
// Returns:
//    void
//
void drawWindow(u8 x, u8 y, u8 width, u8 height, u8 fgColor, u8 bgColor){
    u8 *pvideo;
    
    //cpct_waitVSYNC ();

	// top and bottom fgColor horizontal lines
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,fgColor), width-4, 2);
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y+height);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,fgColor), width-4, 2);
    // top and bottom BG_COLOR horizontal lines
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y+2);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR,BG_COLOR), width-4, 2);
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y+height-2);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR,BG_COLOR), width-4, 2);
    // Internal box
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y+4);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(bgColor,bgColor), width-4, height-6);
          
    // top left corner
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y+2);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR,fgColor), 1, 2);
	
    // left vertical line
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y+4);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,BG_COLOR), 1, height-6);
	
    //bottom left corner
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y+height-2);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR,fgColor), 1, 2);
    // top right corner
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+width-3, y+2);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,BG_COLOR), 1, 2);
    // right vertical line
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+width-3, y+4);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR,fgColor), 1, height-6);
    // bottom right corner
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+width-3, y+height-2);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,BG_COLOR), 1, 2);
}

void drawBottleNeck(u8 x, u8 y, u8 width, u8 height, u8 fgColor, u8 bgColor){
    u8 *pvideo;
	
	// Internal box
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y+4);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(bgColor,bgColor), width, (height/2)-1);
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1+(width/4), y+4+(height/2));
	cpct_drawSolidBox (pvideo, cpct_px2byteM0(bgColor,bgColor), (width/2), (height/2)-1);
    
	// top and bottom fgColor horizontal lines
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,fgColor), width-1, 2);
	
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y+(height/2)+3);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,fgColor), (width/4), 2);
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+width-(width/4), y+(height/2)+3);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,fgColor), (width/4), 2);

    // top and bottom BG_COLOR horizontal lines
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y+2);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR,BG_COLOR), width-1, 2);
	
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1, y+(height/2)+5);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR,BG_COLOR), (width/4), 2);
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+width-(width/4), y+(height/2)+5);
cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR,BG_COLOR), (width/4), 2);

	
	 // top left connector
//	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y+2);
//    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, 2);
//	
//	// left vertical line
//    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y+4);
//    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,BG_COLOR), 1, (height/2)-3);
//	
//	 // bottom left connector
//	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x, y+(height/2));
//    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, 2);
//	
	 // top right connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+width, y+2);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,BG_COLOR), 1, 2);
	
	// right vertical line
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+width, y+4);
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, (height/2)-3);
	
	 // bottom right connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+width, y+(height/2));
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,BG_COLOR), 1, 2);
	
	// Second half of bottleneck
	
	 // top left connector
	pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1+(width/4), y+4+(height/2));
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,BG_COLOR), 1, 2);
	
	// left vertical line
    pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1+(width/4), y+6+(height/2));
    cpct_drawSolidBox (pvideo, cpct_px2byteM0(BG_COLOR, fgColor), 1, (height/2)-7);
	
//// bottom left connector
//pvideo = cpct_getScreenPtr(CPCT_VMEM_START, x+1+(width/4), y+height-3);
//   cpct_drawSolidBox (pvideo, cpct_px2byteM0(fgColor,BG_COLOR), 1, 2);
	
	

}

//
// Function borrowed from Baba's Palace
// This function waits until raster pass the area where I want to draw a sprite.
// Much more faster than waitVSync
void waitRaster(u8 ty) {
	//*** SYNK WITH RASTER
	if(ty<3) while(g_nInterrupt<4);
	else {
		if(ty>=3 && ty<=5) while(g_nInterrupt<5);
		else while(g_nInterrupt>0);
	}
}


