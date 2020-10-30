//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of Throne Legacy
//  Copyright (C) 2020 Arnaud Bouche (@Arnaud6128)
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
//-------------------------------------------------------------------------------

#ifndef SOUND_FX_H
#define SOUND_FX_H

/*************** Arkos2 includes **************/
#include "arkosPlayer2.h"

enum
{
    CHANNEL_A,
    CHANNEL_B,
    CHANNEL_C
};

enum
{
    SOUND_DOOR = 1,
    SOUND_FLAME,
    SOUND_FOOT_STEP,
    SOUND_IMPACT,
    SOUND_MISSILE,
    SOUND_SELECT
};

void InitSound();
void PlaySound();

void InitMusic(void* music);
void ActivateMusic();

#define NO_FX 0

void InitSFX(void* sfx);
void PlaySFX(u8 fx);
void ActivateSFX();

#endif
