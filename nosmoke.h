#ifndef NOSMOKE_H
#define NOSMOKE_H


class CNoSmoke
{
	public:

	virtual bool Pre_HUD_Init			(void);
	//virtual bool Pre_HUD_PlayerMove		(struct playermove_s *ppmove, int server);
	//virtual void Pre_CL_CreateMove		(float frametime, struct usercmd_s *cmd, int active);
	//virtual void Pre_HUD_Redraw (float flTime, int intermission);

	virtual int  Post_HUD_AddEntity (int type, struct cl_entity_s *ent, const char* modelname);

	virtual void Cmd_ToggleSmoke(void);

	bool IsActive;

};

extern CNoSmoke gNoSmoke;


#endif