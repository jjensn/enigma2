#include "client_headers.h"
#include "playerinfo.h"

/*Global*/
CPlayerinfo gPlayerinfo;

bool CPlayerinfo::Pre_HUD_PlayerMove (struct playermove_s *ppmove, int server)
{
	/*
	// Using memcpy to cache the whole structure every frame cut FPS in half :(
	// So this just copies the entire structure once and then updates individual elements as needed
	if(!bCached)
	{
		memcpy(&ppmove_cache, ppmove, sizeof(playermove_t));
		bCached = 1;
	}
	else
	{
	int playerclass = gEngfuncs.GetLocalPlayer()->curstate.playerclass;
	ppmove_cache.velocity = ppmove->velocity;
	ppmove_cache.flags = ppmove->flags;
	ppmove_cache.waterlevel = ppmove->waterlevel;
	ppmove_cache.movetype = ppmove->movetype;
	ppmove_cache.maxspeed = ppmove->maxspeed;
	ppmove_cache.origin = ppmove->origin;
	}
	*/
	return true;
}

void CPlayerinfo::Post_HUD_PlayerMove (struct playermove_s *ppmove, int server)
{
	// Using memcpy to cache the whole structure every frame cut FPS in half :(
	// So this just copies the entire structure once and then updates individual elements as needed
	if(!gPlayerinfo.bCached)
	{
		memcpy(&ppmove_cache, ppmove, sizeof(playermove_t));
		gPlayerinfo.bCached = 1;
	}
	else
	{
		ppmove_cache.velocity = ppmove->velocity;
		ppmove_cache.flags = ppmove->flags;
		ppmove_cache.waterlevel = ppmove->waterlevel;
		ppmove_cache.movetype = ppmove->movetype;
		ppmove_cache.maxspeed = ppmove->maxspeed;
		ppmove_cache.origin = ppmove->origin;
	}
}


CPlayerinfo::CPlayerinfo(void)
{
	bCached = 0;
}