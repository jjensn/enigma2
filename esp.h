#ifndef ESP_H
#define ESP_H

#include "opengl.h"

#define MAX_PLAYERS				32
#define MAX_PLAYER_INFO_STRING	256

#define DOT_AIMING				0.995f
#define DOT_CLOSE				0.98f
#define DOT_FACINGHEMISPHERE	0.0f

typedef struct
{
	bool bCurrent;
	int iTeam;		// 0 = terrists, 1 = counter
	bool bGotHead;
	Vector vPlayerHead;
	int iAimWeighting;
	Vector vOrigin;
	Vector vVelocity;
	float fLastUpdateTime;
	char szName[MAX_PLAYER_INFO_STRING];
	char szModel[MAX_PLAYER_INFO_STRING];
	char szWeaponModel[MAX_PLAYER_INFO_STRING];
	bool bHasBomb;
	bool bHasDefuse;
	bool bHasShield;
	bool bIsReloading;
	bool bTarget;
	Vector vColor1;
	Vector vColor2;
	Vector vColor3;
} playerdata_t;

class CESP
{
	public:

	virtual bool Pre_HUD_Init (void);
	
	virtual void Cmd_ESP (void);

	void Pre_HUD_Redraw (float flTime, int intermission);
	
	//virtual int  Post_HUD_AddEntity (int type, struct cl_entity_s *ent, const char* modelname);

	virtual void Pre_CL_CreateMove		(void);

	virtual bool PlayerIsCurrent(cl_entity_t *pEnt, cl_entity_t *pLocal);

	virtual void CalculatePlayerTeam(int index);

	virtual void GetColorsForTeam(int iTeam, int myTeam, float *vColor1, float *vColor2, float *vColor3);

	virtual char *GetSpriteNameFromModelName(const char *szModelName);

	bool bESP;

	playerdata_t g_playerdata[MAX_PLAYERS]; 
};

extern CESP gESP;

#endif