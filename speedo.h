#ifndef SPEEDO_H
#define SPEEDO_H


class CSpeedo
{
	public:

	virtual bool Pre_HUD_Init			(void);
	//virtual bool Pre_HUD_PlayerMove		(struct playermove_s *ppmove, int server);
	//virtual void Pre_CL_CreateMove		(float frametime, struct usercmd_s *cmd, int active);
	virtual void Pre_HUD_Redraw (float flTime, int intermission);

	virtual void Cmd_ToggleSpeedo(void);

	bool graphic;
	bool text;

};

extern CSpeedo gSpeedo;


#endif