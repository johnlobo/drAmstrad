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
#include <stdio.h> //because sprintf
#include "../defines.h"
#include "board.h"
#include "../game.h"
#include "../util/util.h"
#include "../text/text.h"
#include "../sprites/hit.h"
#include "../sprites/downPills.h"
#include "../sprites/viruses-big.h"
#include "match.h"
#include "../game.h"
#include "audio/arkosPlayer2.h"

u8 *const spritesBigVirus[9] = {sp_viruses_big_0, sp_viruses_big_1, sp_viruses_big_2};

u8 *const hitSprite[3] = {sp_hit_0, sp_hit_1, sp_hit_2};
u8 bigVirusOnScreen[3];

TMatch match;

u16 const pointsPerKill[9] = {0, 100, 220, 360, 540, 700, 900, 1120, 1360};

u8 partialCount;
TVirusList *virusListPtr;
TAnimatedCellsList *animateCellsPtr;

// Prototype of clearMatches function to be used by addViruses procedure
u8 clearMatches(TBoard *b);
// Prototype of drawCell to be used by animateCells
void drawCell(TBoard *b, u8 x, u8 y);

///////////////////////////////////////////////////////////////////////////////////
// Virus section
///////////////////////////////////////////////////////////////////////////////////

// ********************************************************************************
// initvirusList
// 		Initialize a list of Virus
// Input:      A list of Virus
// Returns:    void.
// ********************************************************************************
void initvirusList(TVirusList *virlist)
{
	cpct_memset(virlist, 0, sizeof(TVirusList));
	virlist->lastUpdate = i_time;
}

// ********************************************************************************
// addVirus
// 		adds a Virus to a Virus list
// Input:      A list of Virus, virria position, color and type
// Returns:    Index of the virus just added.
// ********************************************************************************
u8 addVirusToList(TVirusList *virlist, u8 x, u8 y, u8 color)
{
	u8 iter = 0;
	if (virlist->count < MAX_VIR_LIST)
	{
		do
		{
			if (virlist->virusList[iter].type != 0)
			{
				iter++;
			}
			else
			{
				break;
			}
		} while (iter < MAX_VIR_LIST);

		virlist->virusList[iter].color = color;
		virlist->virusList[iter].type = 6;
		virlist->virusList[iter].x = x;
		virlist->virusList[iter].y = y;
		virlist->count++;
		virlist->colorCount[color]++;
	}
	return iter;
}

// ********************************************************************************
// deleteVirus
// 		deletes a Virus from a Virus list
// Input:      A list of Virus and the index of the Virus to remove
// Returns:    void.
// ********************************************************************************
void deleteVirus(TVirusList *virlist, u8 x, u8 y)
{
	u8 i;

	i = 0;
	while (i < MAX_VIR_LIST)
	{
		if ((virlist->virusList[i].x == x) && (virlist->virusList[i].y == y))
		{
			break;
		}
		else
		{
			i++;
		}
	}
	if (i < MAX_VIR_LIST)
	{
		virlist->virusList[i].x = 255;
		virlist->virusList[i].y = 255;
		virlist->virusList[i].type = 0;
		virlist->colorCount[virlist->virusList[i].color]--;
		virlist->virusList[i].color = 255;
		virlist->count--;
	}
}

// ********************************************************************************
// drawOneVirus
// 		Prints one Virus in the board
// Returns: void
// ********************************************************************************
void drawOneVirus(TBoard *b, u8 i)
{
	u8 *pvmem;
	TVirus *vir;
	u8 step;

	vir = &b->virList.virusList[i];
	step = b->virList.step;
	// Calculate screen adrees for the Virus
	pvmem = cpct_getScreenPtr(CPCT_VMEM_START,
							  b->originX + (vir->x * CELL_WIDTH),
							  b->originY + (vir->y * CELL_HEIGHT));
	// Print Virus
	cpct_drawSprite(
		sprites[vir->color][vir->type + step],
		pvmem,
		CELL_WIDTH,
		CELL_HEIGHT);
}

// ********************************************************************************
// drawVirusList
// 		Draws all the Virus in a board
// Input:      A list of Virus and the board
// Returns:    void.
// ********************************************************************************
void drawVirusList(TBoard *b)
{
	u8 i;
	u8 rep;

	virusListPtr = &b->virList;

	rep = NUM_ANIMATED_VIRUS;
	i = virusListPtr->animateIndex;
	while (rep)
	{
		if (virusListPtr->virusList[i].type)
		{
			drawOneVirus(b, i);
			rep--;
		}
		i++;
		if (i >= MAX_VIR_LIST)
		{
			i = 0;
			virusListPtr->step = (virusListPtr->step + 1) % 3;
		}
	}
	virusListPtr->animateIndex = i;
}

// ********************************************************************************
// createtVirus
// Set the Virus in the board depending on the level
// Input:      Level
// Returns:    the index of the virus in the virus list.
// ********************************************************************************
u8 createVirus(TBoard *b, u8 randomXY, u8 x, u8 y)
{
	u8 color;

	if (randomXY)
	{
		do
		{
			x = (cpct_rand8() % BOARD_WIDTH);
			y = (cpct_rand8() % (BOARD_HEIGHT - levels[level].maxRow)) + levels[level].maxRow;
		} while (b->content[y][x] != 0);
	}

	color = (cpct_rand8() % 3);						 // creates a random color
	b->content[y][x] = 6;							 // 6 is Virus order in the content array;
	b->color[y][x] = color;							 // Assign a random color
	return addVirusToList(&b->virList, x, y, color); // add Virus to the list
}

// ********************************************************************************
// createInitialSetOfVirus
// Set the Virus in the board depending on the level
// Input:      Level
// Returns:    void.
// ********************************************************************************
void createInitialSetOfVirus(TBoard *b, u8 l)
{
	u8 count = 0;
	u8 virus = levels[l].numberOfVirus;

	while (count < virus)
	{
		createVirus(b, YES, 0, 0); // Create a virus in random position
		count++;
	}
	b->virList.animateIndex = 0;
}

// ********************************************************************************
// resetBigVirus
// Input: void
// Returns: void
// ********************************************************************************
void resetBigVirus()
{
	cpct_memset(&bigVirusOnScreen, 0, 3);
}

// ********************************************************************************
// printBigVirus
// Input: void
// Returns: void
// ********************************************************************************
void printBigVirus(TBoard *b)
{
	u8 n;
	u8 *pvmem;

	for (n = 0; n < 3; n++)
	{
		if ((u8)(b->virList.colorCount[n] > 0) != bigVirusOnScreen[n])
		{
			pvmem = cpct_getScreenPtr(SCR_VMEM, 5 + (SP_VIRUSES_BIG_1_W * (n == 1)), 100 + (SP_VIRUSES_BIG_1_H * n));
			//cpct_drawSprite( (u8 *)spritesBigVirus[n], pvmem, SP_VIRUSES_BIG_1_W, SP_VIRUSES_BIG_1_H);
			cpct_drawSpriteBlended(pvmem, SP_VIRUSES_BIG_1_H, SP_VIRUSES_BIG_1_W, (u8 *)spritesBigVirus[n]);

			bigVirusOnScreen[n] = (b->virList.colorCount[n] > 0);
		}
		// Print number
		pvmem = cpct_getScreenPtr(SCR_VMEM, 15 - (14 * (n == 1)) + (SP_VIRUSES_BIG_1_W * (n == 1)), 111 + (SP_VIRUSES_BIG_1_H * n));
		cpct_drawSolidBox(pvmem, 0, 4, 8);
		if (b->virList.colorCount[n] > 0)
		{
			sprintf(auxTxt, "%d", b->virList.colorCount[n]);
			drawText(auxTxt, 15 - (14 * (n == 1)) + (SP_VIRUSES_BIG_1_W * (n == 1)), 111 + (SP_VIRUSES_BIG_1_H * n), COLORTXT_WHITE, NORMALHEIGHT);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Queue section
///////////////////////////////////////////////////////////////////////////////////

// ********************************************************************************
// initPillQueue
// Returns:    
//		void.
// ********************************************************************************
void initPillQueue()
{
	u8 i;

	for (i = 0; i < 128; i++)
	{
		pillQueue[i].color[0] = (cpct_rand8() % 3);
		pillQueue[i].color[1] = (cpct_rand8() % 3);
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Animate Cell section
///////////////////////////////////////////////////////////////////////////////////

// ********************************************************************************
// initAnimatedCellList
// Returns:
// 		Void
// ********************************************************************************
void initAnimatedCellList(TAnimatedCellsList *l)
{
	cpct_memset(l, 0, sizeof(TAnimatedCellsList));
}

// ********************************************************************************
// addAnimatedCell
// Returns:
// 		void
// ********************************************************************************
void addAnimatedCell(TAnimatedCellsList *l, u8 x, u8 y, u8 createVirus)
{
	u8 i = 0;
	// search for a free slot
	if (l->count < MAX_ANIM_CELLS)
	{
		while (i < MAX_ANIM_CELLS)
		{
			if (l->cells[i].status) //the cell status is set => busy
			{
				i++;
			}
			else
			{
				break;
			}
		}
		// if a free slot was found we store it
		if (i < MAX_ANIM_CELLS)
		{
			l->cells[i].status = 1;
			l->cells[i].x = x;
			l->cells[i].y = y;
			l->cells[i].index = 0;
			l->cells[i].createVirus = createVirus;
			l->count = l->count + 1;
		}
	}
}

// ********************************************************************************
// animateCells
// Returns:
// 		void
// ********************************************************************************
void animateCells(TBoard *b, u8 type)
{
	u8 i, virusIndex;
	//u8 *pvmem;

	animateCellsPtr = &b->animatedCells;

	// Iteration over the animaMatchList to print next step on every match
	for (i = 0; i < MAX_ANIM_CELLS; i++)
	{
		// Check if the cell has to be animated
		if (animateCellsPtr->cells[i].status)
		{
			// Depending on the step of the animation print a new frame or init the match
			if (animateCellsPtr->cells[i].index < 3)
			{
				drawHitSpriteXY(b->originX + (animateCellsPtr->cells[i].x * CELL_WIDTH),
								b->originY + (animateCellsPtr->cells[i].y * CELL_HEIGHT), animateCellsPtr->cells[i].index);
				animateCellsPtr->cells[i].index++;
			}
			else
			{
				//We are finished with the animation, so init match and decrease animateCells count
				animateCellsPtr->cells[i].status = 0;
				animateCellsPtr->count--;
				if (animateCellsPtr->cells[i].createVirus)
				{
					virusIndex = createVirus(b, NO, animateCellsPtr->cells[i].x, animateCellsPtr->cells[i].y); // add Virus to the list of viruses
					drawOneVirus(b, virusIndex);
					clearMatches(b);
				}
				else
				{
					drawCell(b, animateCellsPtr->cells[i].x, animateCellsPtr->cells[i].y);
				}
				drawSingleVirusCount(b);
				if (type == PLAYER1)
					printBigVirus(b);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Board section
///////////////////////////////////////////////////////////////////////////////////

// ********************************************************************************
// initBoard
// 		initializes the board
// Returns: 
//		void
// ********************************************************************************
void initBoard(TBoard *b, u8 p, u8 x, u8 y, u8 scX, u8 scY, u8 viX, u8 viY)
{
	u8 i, j;

	b->player = p;
	b->playerLapse = PLAYER_SPEED;
	b->originX = x;
	b->originY = y;
	b->scoreX = scX;
	b->scoreY = scY;
	b->virusX = viX;
	b->virusY = viY;
	for (j = 0; j < BOARD_HEIGHT; j++)
	{
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			b->color[j][i] = 255;
			b->content[j][i] = 0;
		}
	}
	b->capsules = 0;
	b->currentDelay = 0;
	b->applyingGravity = NO;
	b->throwing = NO;
	initvirusList(&b->virList);
	initAnimatedCellList(&b->animatedCells);
	b->pillQueueIndex = 0;
}

// ********************************************************************************
// drawBoardCells
// 		prints the content of the cells of the board on the screen
// Returns: 
// 		void
// ********************************************************************************
void drawBoardCells(TBoard *b)
{
	u8 i, j;
	u8 *pvmem;

	for (j = 1; j < BOARD_HEIGHT; j++)
	{
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			pvmem = cpct_getScreenPtr(CPCT_VMEM_START, b->originX + (i * CELL_WIDTH), b->originY + (j * CELL_HEIGHT));
			if (b->content[j][i] != 0)
			{
				cpct_drawSprite(
					sprites[b->color[j][i]][b->content[j][i]],
					pvmem,
					CELL_WIDTH,
					CELL_HEIGHT);
			}
			else
				cpct_drawSprite(emptyCell, pvmem, CELL_WIDTH, CELL_HEIGHT);
		}
	}
}

// ********************************************************************************
// drawBoard
// 		prints the board on the screen
// Returns: void
// ********************************************************************************
void drawBoard(TBoard *b)
{
	// Clear board background
	drawWindow(b->originX - 1, b->originY - 5 + 8, 28, 119);
	drawBottleNeck(b->originX - 1 + 4, b->originY - 5 - 29 + 8, 18, 32, 15, BG_COLOR);
	drawBoardCells(b);
}

// ********************************************************************************
// clearGameArea
// Output: void
// ********************************************************************************
void clearGameArea(TBoard *b)
{
	u8 *pvmem;
	pvmem = cpct_getScreenPtr(SCR_VMEM, b->originX - SP_DOWNPILLS_0_W, b->originY - SP_DOWNPILLS_0_H);
	cpct_drawSolidBox(pvmem, cpct_px2byteM0(BG_COLOR, BG_COLOR), 10 * SP_DOWNPILLS_0_W, 10 * (SP_DOWNPILLS_0_H + 1));
	pvmem = cpct_getScreenPtr(SCR_VMEM, b->originX - SP_DOWNPILLS_0_W, b->originY + 9 * (SP_DOWNPILLS_0_H + 1));
	cpct_drawSolidBox(pvmem, cpct_px2byteM0(BG_COLOR, BG_COLOR), 10 * SP_DOWNPILLS_0_W, 8 * (SP_DOWNPILLS_0_H + 1));
}

// ********************************************************************************
// drawSingleScore
// Output: void
// ********************************************************************************
void drawSingleScore(TBoard *b)
{
	u8 *pvmem;

	sprintf(auxTxt, "%06d", b->score);
	pvmem = cpct_getScreenPtr(CPCT_VMEM_START, b->scoreX, b->scoreY);
	cpct_drawSolidBox(pvmem, cpct_px2byteM0(14, 14), strLength(auxTxt) * 2, 9);
	drawText(auxTxt, b->scoreX, b->scoreY, COLORTXT_WHITE, NORMALHEIGHT);
}

// ********************************************************************************
// drawScoreBoard1
// Output:
// ********************************************************************************
void drawScoreBoard1(TBoard *b)
{
	drawWindow(1, 3, 30, 29);
	//Top
	drawText("TOP", 3, 9, COLORTXT_RED, NORMALHEIGHT);
	sprintf(auxTxt, "%06d", hallOfFameSingle.topScore);
	drawText(auxTxt, 16, 9, COLORTXT_WHITE, NORMALHEIGHT);
	//Score
	drawText("SCORE", 3, 19, COLORTXT_RED, NORMALHEIGHT);
	drawSingleScore(b);
}

// ********************************************************************************
// drawSingleVirusCount
// Output:
// ********************************************************************************
void drawSingleVirusCount(TBoard *b)
{
	u8 *pvmem;

	sprintf(auxTxt, "%02d", b->virList.count);
	pvmem = cpct_getScreenPtr(CPCT_VMEM_START, b->virusX, b->virusY);
	cpct_drawSolidBox(pvmem, cpct_px2byteM0(BG_COLOR, BG_COLOR), strLength(auxTxt) * 2, 9);
	drawText(auxTxt, b->virusX, b->virusY, COLORTXT_WHITE, NORMALHEIGHT);
}

// ********************************************************************************
// drawScoreBoard2
// Output:
// ********************************************************************************
void drawScoreBoard2(TBoard *b)
{
#define X_SB2 58
#define Y_SB2 162

	drawWindow(X_SB2, Y_SB2, 24, 31);
	drawText("LEVEL", (u8)(X_SB2 + 3), (u8)(Y_SB2 + 7), COLORTXT_RED, NORMALHEIGHT);
	sprintf(auxTxt, "%2d", level);
	drawText(auxTxt, (u8)(X_SB2 + 15), (u8)(Y_SB2 + 7), COLORTXT_WHITE, NORMALHEIGHT);
	drawText("VIRUS", (u8)(X_SB2 + 3), (u8)(Y_SB2 + 17), COLORTXT_RED, NORMALHEIGHT);
	drawSingleVirusCount(b);
}

// ********************************************************************************
// drawHitSpriteXY
// Input: board and match to remove form the screen
// Output: void
// ********************************************************************************
void drawHitSpriteXY(u8 x, u8 y, u8 step)
{
	u8 *pvmem;
	pvmem = cpct_getScreenPtr(CPCT_VMEM_START, x, y);
	cpct_drawSprite(hitSprite[step], pvmem, SP_HIT_0_W, SP_HIT_0_H);

}

// ********************************************************************************
// drawCell
// Input:
// Output: void
// ********************************************************************************
void drawCell(TBoard *b, u8 x, u8 y)
{
	u8 *pvmem;
	u8 *sprite;

	pvmem = cpct_getScreenPtr(CPCT_VMEM_START, b->originX + (x * CELL_WIDTH), b->originY + (y * CELL_HEIGHT));

	//Select the sprite to draw depending on the content
	if (b->content[y][x] == 0){
		sprite = emptyCell;
	} else {
		sprite = sprites[b->color[y][x]][b->content[y][x]];
	}

	cpct_drawSprite(
		(u8*) sprite,
		pvmem,
		CELL_WIDTH,
		CELL_HEIGHT
		);
}

// ********************************************************************************
// deleteCell
// Input:
// Output: void
// ********************************************************************************
void deleteCell(TBoard *b, u8 x, u8 y)
{
	u8 *pvmem;

	pvmem = cpct_getScreenPtr(CPCT_VMEM_START, b->originX + (x * CELL_WIDTH), b->originY + (y * CELL_HEIGHT));
	cpct_drawSprite(
		emptyCell,
		pvmem,
		CELL_WIDTH,
		CELL_HEIGHT);
}

// ********************************************************************************
// updateCell
// Input:
// Output: void
// ********************************************************************************
void updateCell(TBoard *b, u8 x, u8 y)
{
	deleteCell(b, x, y);
	drawCell(b, x, y);
}

// ********************************************************************************
// deleteMatch
// Input: board and match to remove form the screen
// Output: void
// ********************************************************************************
void deleteMatch(TBoard *b, TMatch *m)
{
	u8 i;
	u8 x, y;

	for (i = 0; i < m->count; i++)
	{
		x = m->x + (i * (!m->direction));
		y = m->y + (i * m->direction);

		deleteCell(b, x, y);
	}
}

// ********************************************************************************
// removeMatch
// Input: board and match to remove form the board
// Output: void
// ********************************************************************************
void removeMatch(TBoard *b, TMatch *m)
{
	u8 i;
	u8 x0, y0, c0, d0;
	u8 x, y;
	u8 virusCount = 0;

	x0 = m->x;
	y0 = m->y;
	d0 = m->direction;
	c0 = m->count;

	//erase match form logic board
	//check vertical pill doing the match case
	if ((y0 > 0) && (d0 == VERTICAL) && (b->content[y0][x0] == 2))
	{
		b->content[y0 - 1][x0] = 5;
		updateCell(b, x0, y0 - 1);
	}

	// match loop
	for (i = 0; i < c0; i++)
	{
		y = y0 + (d0 * i);
		x = x0 + ((!d0) * i);
		// erase match from board
		// Change the half of the cell erased
		if (d0 == VERTICAL)
		{
			if ((b->content[y][x] == 3) && (x < BOARD_WIDTH) && (b->content[y][x + 1] == 4))
			{
				b->content[y][x + 1] = 5;
				updateCell(b, x + 1, y);
			}
			if ((b->content[y][x] == 4) && (x > 0) && (b->content[y][x - 1] == 3))
			{
				b->content[y][x - 1] = 5;
				updateCell(b, x - 1, y);
			}
		}
		else
		{
			if ((b->content[y][x] == 1) && (y < BOARD_HEIGHT) && (b->content[y + 1][x] == 2))
			{
				b->content[y + 1][x] = 5;
				updateCell(b, x, y + 1);
			}
			if ((b->content[y][x] == 2) && (y > 0) && (b->content[y - 1][x] == 1))
			{
				b->content[y - 1][x] = 5;
				updateCell(b, x, y - 1);
			}
		}
		if (b->content[y][x] == 6)
		{
			virusCount++; // One virus found
			deleteVirus(&b->virList, x, y);
			drawSingleVirusCount(b);
		}
		b->content[y][x] = 0;
		b->color[y][x] = 255;
	}
	// Add score for killing a virus
	if (virusCount)
	{
		b->score += pointsPerKill[virusCount];
		drawSingleScore(b);
	}
	//Marked the found virus in the match for further treatment
	b->virusMatched = b->virusMatched + virusCount;

	// Set animation
	for (i = 0; i < m->count; i++)
	{
		x = m->x + (i * (!m->direction));
		y = m->y + (i * m->direction);

		addAnimatedCell(&b->animatedCells, x, y, NO);
	}

	initMatch(m);
}

// ********************************************************************************
// applyGravity
// Input:
// Output:
// ********************************************************************************
void applyGravity(TBoard *b)
{
	u8 i, j, k;
	u8 *pvmem;

	for (j = (BOARD_HEIGHT - 2); j > 0; j--)
	{
		for (i = 0; i < BOARD_WIDTH; i++)
		{
			if (
				// is not a virus and not empty
				(b->content[j][i] > 0) && (b->content[j][i] < 6) &&
				(b->content[j + 1][i] == 0) && // there is free space underneath
				!(							   //None of these conditions is met
					//is a complete piece laying on something on it's right side
					((i < 7) && (j < 16) && (b->content[j][i] == 3) && (b->content[j][i + 1] == 4) && (b->content[j + 1][i + 1] != 0)) ||
					///is a complete piece laying on something on it's left side
					((i > 0) && (j < 16) && (b->content[j][i - 1] == 3) && (b->content[j][i] == 4) && (b->content[j + 1][i - 1] != 0))))
			{
				k = j + 1;
				deleteCell(b, i, k - 1);
				b->content[k][i] = b->content[k - 1][i];
				b->color[k][i] = b->color[k - 1][i];
				b->content[k - 1][i] = 0;
				b->color[k - 1][i] = 255;
				pvmem = cpct_getScreenPtr(CPCT_VMEM_START, b->originX + (i * CELL_WIDTH), b->originY + (k * CELL_HEIGHT));
				cpct_drawSprite(
					sprites[b->color[k][i]][b->content[k][i]],
					pvmem,
					CELL_WIDTH,
					CELL_HEIGHT);
				// Sound if hit
				if ((k == 16) || (b->content[k + 1][i] != 0))
				{
					PLY_AKG_PLAYSOUNDEFFECT(SOUND_HIT, CHANNEL_B, 0);
				}
				//Return after moving a line down
				return;
			}
		}
	}
	//If no gravity is applied deactivate gravity flag
	b->applyingGravity = clearMatches(b);
}

// ********************************************************************************
// clearMatches
// Input:
// Output:
// ********************************************************************************
u8 clearMatches(TBoard *b)
{
	u8 row, col;
	u8 i, j, k, l;
	u8 result;

	result = NO;
	b->virusMatched = 0;

	// Find matches in rows
	for (row = 0; row < BOARD_HEIGHT; row++)
	{
		i = 0;
		while (i < BOARD_WIDTH)
		{
			if (b->color[row][i] != 255)
			{
				j = i + 1;
				partialCount = 1;
				while ((j < BOARD_WIDTH) && (b->color[row][i] == b->color[row][j]))
				{
					partialCount++;
					j++;
				}
				if (partialCount > 3)
				{
					setMatch(&match, i, row, HORIZONTAL, partialCount, 0);
					removeMatch(b, &match);
					result = YES;
					PLY_AKG_PLAYSOUNDEFFECT(SOUND_LINE, CHANNEL_B, 0);
				}
				i = j;
			}
			else
			{
				i++;
			}
		}
	}
	// Clear matches in cols
	for (col = 0; col < BOARD_WIDTH; col++)
	{
		k = 0;
		while (k < BOARD_HEIGHT)
		{
			if (b->color[k][col] != 255)
			{
				l = k + 1;
				partialCount = 1;
				while ((l < BOARD_HEIGHT) && (b->color[k][col] == b->color[l][col]))
				{
					partialCount++;
					l++;
				}
				if (partialCount > 3)
				{
					setMatch(&match, col, k, VERTICAL, partialCount, 0);
					removeMatch(b, &match);
					result = YES;
					PLY_AKG_PLAYSOUNDEFFECT(SOUND_LINE, CHANNEL_B, 0);
				}
				k = l;
			}
			else
			{
				k++;
			}
		}
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////////
// Vs section
///////////////////////////////////////////////////////////////////////////////////

// ********************************************************************************
// drawScoreBoardVs
// Input:
// Output:
// ********************************************************************************
void drawScoreBoardVs(TBoard *b1, TBoard *b2)
{
	u8 i;
	u8 *pvmem;

	drawWindow(11, 3, 62, 30);
	//Top
	drawText("TOP", 32, 9, COLORTXT_RED, NORMALHEIGHT);
	sprintf(auxTxt, "%06d", hallOfFameVs.topScore);
	drawText(auxTxt, 39, 9, COLORTXT_WHITE, NORMALHEIGHT);
	//Score
	drawText("PLAYER2", 14, 19, COLORTXT_RED, NORMALHEIGHT);
	drawSingleScore(b2);
	drawText("PLAYER1", 42, 19, COLORTXT_RED, NORMALHEIGHT);
	drawSingleScore(b1);

	drawWindow(32, 46, 19, 20);
	drawText("LEVEL", 34, 52, COLORTXT_RED, NORMALHEIGHT);
	sprintf(auxTxt, "%2d", level);
	drawText(auxTxt, 44, 52, COLORTXT_WHITE, NORMALHEIGHT);

	//Player indicators
	drawText("P2", 1, 47, COLORTXT_YELLOW, NORMALHEIGHT);
	drawText("P1", 75, 47, COLORTXT_YELLOW, NORMALHEIGHT);

	//Wins panel
	pvmem = cpct_getScreenPtr(SCR_VMEM, 31, 80);
	cpct_drawSolidBox(pvmem, 255, 19, 65);

	for (i = 0; i < 3; i++)
	{
		pvmem = cpct_getScreenPtr(SCR_VMEM, 32, 82 + (i * 21));
		cpct_drawSolidBox(pvmem, cpct_px2byteM0(0, 0), 8, 19);
		pvmem += 9;
		cpct_drawSolidBox(pvmem, cpct_px2byteM0(0, 0), 8, 19);
	}

	// Virus Panels
	drawText("VIRUS", 36, 179, COLORTXT_RED, NORMALHEIGHT);
	drawWindow(26, 172, 10, 18);
	drawWindow(46, 172, 10, 18);
	drawSingleVirusCount(b1);
	drawSingleVirusCount(b2);
}