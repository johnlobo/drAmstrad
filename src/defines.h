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
#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <cpctelera.h>
#include "entities/board.h"
#include "keyboard/keyboard.h"
#include "game.h"

#define SCR_VMEM (u8 *)0xC000

#define YES 1
#define NO 0

#define TRUE 1
#define FALSE 0

#define HORIZONTAL 0
#define VERTICAL 1

#define PREVIOUS 0
#define CURRENT 1
#define NEXT 2

//Font Size
#define FONT_H 5
#define FONT_W 2

#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define max(X, Y) (X > Y ? X : Y)

// FOR TEXT
#define COLORTXT_WHITE 0
#define COLORTXT_YELLOW 1
#define COLORTXT_ORANGE 2
#define COLORTXT_BLUE 3
#define COLORTXT_RED 4
#define COLORTXT_MAUVE 5

#define TRANSPARENT 1
#define OPAQUE 0

#define NORMALHEIGHT 1
#define DOUBLEHEIGHT 2

#define VIEW_X 14
#define VIEW_Y 17

//Speed
#define PLAYER_SPEED 17
#define ANIM_SPEED 20

#define BG_COLOR 0

#define SWITCH_SCREENS 300

#define PLAYER1 0
#define PLAYER2 1
#define PLAYER1_VS 2
#define PLAYER2_VS 3

#define SINGLE 0
#define VS 1

#define MAX_LEVEL 20

#define FIRE_COOL_TIME 1

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define CELL_WIDTH 3
#define CELL_HEIGHT 7

//Board
#define MAX_ROWS 17
#define MAX_COLS 8

// showMessage types
#define MESSAGE 0
#define YESNO 1
#define NUMBER 2
#define TEMPORAL 3

// board states
#define RUNNING 0
#define ANIMATE_MATCH 1
#define ANIMATE_GRAVITY 2
#define CHECKING_ROWS 3
#define CHECKING_COLS 3

// cursor states
#define CURSOR_ANIM 2

//Virus
#define NUM_ANIMATED_VIRUS 4
#define VIRUS_LEVEL_FACTOR 4
#define VIRUS_ANIM_SPEED 200

//FX
#define SOUND_VIRUS 4
#define SOUND_LINE 9
#define SOUND_HIT 14
#define SOUND_TURN 15
#define SOUND_HIHAT 16

//MUSIC
#define LODISKA_SONG 0
#define LESGRACES_SONG 1
#define LANATIVE_SONG 2
#define FARNHAM_SONG 3
#define WIN_SONG 4
#define LOSE_SONG 5
#define SILENCE 6
#define FEVER_MIX 7

typedef struct
{
	u8 name[10];
	u32 score;
	u8 level;
} THallOfFameEntry;

typedef struct
{
	THallOfFameEntry entries[3];
	u32 topScore;
} THallOfFame;

extern const u8 sp_palette0[16]; // Regular palette
extern u8 g_nInterrupt;			 // Manage Interrupt and locate raytrace
extern u32 i_time;
extern u8 music;
extern u8 startingLevel;
extern const u8 passwords[21][6];

// Relocated variables
extern __at(0xa800) TBoard board1;				  //size: 0x291
extern __at(0xaa91) TBoard board2;				  //size: 0x291
extern __at(0xad22) TPill pillQueue[128];		  //size: 0x100
extern __at(0xae22) THallOfFame hallOfFameSingle; //size: 0x4f
extern __at(0xae71) THallOfFame hallOfFameVs;	  //size: 0x4f
extern __at(0xaec0) TKeys keys1;				  //size: 0x1f
extern __at(0xaedf) TKeys keys2;				  //size: 0x1f
extern __at(0xb000) u8 *screenBuffer0;			  //size: 0xe10

// Spare space in Video Memory
extern __at(0xc7d0) u8 *screenSpareBuffer01; //size: 0x2f
extern __at(0xcfd0) u8 emptyCell[21];		 //size: 0x2f used: 0x15
extern __at(0xd7d0) u8 auxTxt[40];			 //size: 0x2f used: 0x28
extern __at(0xdfd0) u8 *screenSpareBuffer04; //size: 0x2f
extern __at(0xe7d0) u8 *screenSpareBuffer05; //size: 0x2f
extern __at(0xefd0) u8 *screenSpareBuffer06; //size: 0x2f
extern __at(0xf7d0) u8 *screenSpareBuffer07; //size: 0x2f
extern __at(0xffd0) u8 *screenSpareBuffer08; //size: 0x2f

extern u8 current_song;
void activateMusic(u8 random);
void deActivateMusic();

// Sound, music and fx
extern void *DRROLANDSOUNDTRACK_START;
extern void *FX_SOUNDEFFECTS;
enum
{
	CHANNEL_A,
	CHANNEL_B,
	CHANNEL_C
};

#endif