#ifndef FLAGSTATUS_H
#define FLAGSTATUS_H

class CFlagStatus
{
	public:

	virtual bool Pre_HUD_Init			(void);
	//virtual bool Pre_HUD_PlayerMove		(struct playermove_s *ppmove, int server);
	//virtual void Pre_CL_CreateMove		(float frametime, struct usercmd_s *cmd, int active);
	virtual void Pre_HUD_Redraw (float flTime, int intermission);
	//virtual int  Post_HUD_AddEntity (int type, struct cl_entity_s *ent, const char* modelname);
	virtual void Cmd_ToggleFlag(void);
	virtual void Post_Hook_HudText ( const char *pszString );
	virtual void Post_Hook_TxtMsg ( const char *pszString );
	virtual void Pre_Hook_ConPrint ( const char *pszString);

	bool IsActive;
	bool bEnemyT;
	bool bMyT;

	char myflag[128];
	char enemyflag[128];

	//char bluebuff[128];
	//char redbuff[128];

	float fEnemyTimer;
	float fMyTimer;

	int x1;
	int x2;

	//int redflag;
	//int blueflag;

	//int team;

};

extern CFlagStatus gFlagStatus;


#endif