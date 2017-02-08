#ifndef BHOP_H
#define BHOP_H


class CBunnyhop
{
	public:

	virtual bool Pre_HUD_Init			(void);
	//virtual bool Pre_HUD_PlayerMove		(struct playermove_s *ppmove, int server);
	virtual void Pre_CL_CreateMove		(float frametime, struct usercmd_s *cmd, int active);

	virtual void Cmd_BHop(void);

	int IsActive;

};

extern CBunnyhop gBunnyhop;


#endif