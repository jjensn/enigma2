#ifndef PLAYERINFO_H
#define PLAYERINFO_H

class CPlayerinfo
{
	public:
	
	virtual bool Pre_HUD_PlayerMove (struct playermove_s *ppmove, int server);
	virtual void Post_HUD_PlayerMove (struct playermove_s *ppmove, int server);

	CPlayerinfo(void);

	playermove_t ppmove_cache;
	bool bCached;
	float fMaxSpeeds[10];
};

extern CPlayerinfo gPlayerinfo;

#endif