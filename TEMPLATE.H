#ifndef SPEEDO_H
#define SPEEDO_H


class CSpeedo
{
	public:

	virtual bool Pre_HUD_Init			(void);
	virtual bool Pre_HUD_PlayerMove		(struct playermove_s *ppmove, int server);
	virtual void Pre_CL_CreateMove		(float frametime, struct usercmd_s *cmd, int active);

	virtual void Cmd_ToggleSpeedo(void);



};

extern CSpeedo gSpeedo;


#endif