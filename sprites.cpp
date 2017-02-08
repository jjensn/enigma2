#include "client_headers.h"

spritedata_t g_spritedata;

void InitSprites(void)
{
	pEngine->pfnSPR_GetList = &hook_pfnSPR_GetList;
}

client_sprite_t* hook_pfnSPR_GetList(char *psz, int *piCount)
{
	client_sprite_t *ret;

	ret = gEngine.pfnSPR_GetList(psz, piCount);

	if(psz && psz[0] /*&& strstr(psz, "sprites/weapon_")*/)
	{
		int iCount;
		client_sprite_t *pList;

		//add_log("hook_pfnSPR_GetList called for %s, taking opportunity to load sprite data...", psz);

		pList = gEngine.pfnSPR_GetList("sprites/hud.txt", &iCount);
		if(pList)
			LoadSprites(pList, iCount);
	}

	return ret;
}

void LoadSprites(client_sprite_t *pList, int iCount)
{
	int i;
	client_sprite_t *p;
	char buf[512];

	g_spritedata.iSpritesLoaded = 0;

	if(!pList)
		return;

	p = pList;

	i = iCount;

	while(i-- && p->szName[0] != '\0' && g_spritedata.iSpritesLoaded < MAX_SPRITES)
	{
		strcpy(g_spritedata.spriteinfo[g_spritedata.iSpritesLoaded].szName, p->szName);
		strcpy(g_spritedata.spriteinfo[g_spritedata.iSpritesLoaded].szSprite, p->szSprite);

		sprintf(buf, "sprites/%s.spr", p->szSprite);
		g_spritedata.spriteinfo[g_spritedata.iSpritesLoaded].hspr = pEngine->pfnSPR_Load(buf);

		memcpy(&(g_spritedata.spriteinfo[g_spritedata.iSpritesLoaded].rc), &(p->rc), sizeof(p->rc));
		g_spritedata.spriteinfo[g_spritedata.iSpritesLoaded].iRes = p->iRes;

		p++;
		g_spritedata.iSpritesLoaded++;
	}
}

int GetSpriteIndex(char *szName)
{
	int i;

	if(szName[0] == '\0')
		return -1;

	for(i = 0; i < g_spritedata.iSpritesLoaded; i++)
	{
		if(g_spritedata.spriteinfo[i].szName[0] == szName[0] && g_spritedata.spriteinfo[i].iRes == 640)
		{
			if(!strcmp(g_spritedata.spriteinfo[i].szName, szName))
			{
				return i;
			}
		}
	}

	return -1;
}

bool DrawAddSpriteByIndex(int index, int frame, int x, int y, int red, int green, int blue)
{
	if(index == -1)
		return false;

//	if(!CheckAgainstViewport(x, y))
	//	return false;

	pEngine->pfnSPR_Set(g_spritedata.spriteinfo[index].hspr, red, green, blue);
	pEngine->pfnSPR_DrawAdditive(frame, x, y, &(g_spritedata.spriteinfo[index].rc));

	return true;
}

bool DrawAddSpriteByName(char *szName, int frame, int x, int y, int red, int green, int blue)
{
	int index;

	if(!szName || !szName[0])
		return false;

	index = GetSpriteIndex(szName);

	return DrawAddSpriteByIndex(index, frame, x, y, red, green, blue);
}