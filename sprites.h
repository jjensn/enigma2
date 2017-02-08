/*
tabrix mini-hack for Half-Life
Copyright (C) 2004 Niall FitzGibbon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef SPRITES_H
#define SPRITES_H

#define MAX_SPRITES				1024

typedef struct
{
	char szName[256];
	char szSprite[256];
	HSPRITE hspr;
	wrect_t rc;
	int iRes;
} spriteinfo_t;

typedef struct
{
	int iSpritesLoaded;
	spriteinfo_t spriteinfo[MAX_SPRITES];
} spritedata_t;

extern spritedata_t g_spritedata;

void InitSprites(void);
client_sprite_t* hook_pfnSPR_GetList(char *psz, int *piCount);
void LoadSprites(client_sprite_t *pList, int iCount);
int GetSpriteIndex(char *szName);

bool DrawAddSpriteByIndex(int index, int frame, int x, int y, int red, int green, int blue);
bool DrawAddSpriteByName(char *szName, int frame, int x, int y, int red, int green, int blue);

#endif