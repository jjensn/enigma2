#include "client_headers.h"
#include "bunnyhop.h"

CBunnyhop gBunnyhop;

DECLARE_COMMAND_2(gBunnyhop, BHop);

bool CBunnyhop::Pre_HUD_Init	(void)
{
    HOOK_COMMAND_2("e_bhop",BHop);
	return true;
}

void CBunnyhop::Cmd_BHop		(void)
{
	if(gBunnyhop.IsActive == 0)
	{
		// bhop on!
		gBunnyhop.IsActive = 1;
		econsoleprint("bhop enabled",true);
		return;
	}

	if(gBunnyhop.IsActive == 1)
	{
		// bhop off!
		gBunnyhop.IsActive = 0;
		econsoleprint("bhop disabled",true);
		return;
	}
}

void CBunnyhop::Pre_CL_CreateMove(float frametime, usercmd_s *cmd,int active)
{
	if (!gBunnyhop.IsActive) return;

	if ((cmd->buttons & IN_JUMP) && !(gPlayerinfo.ppmove_cache.flags & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;
}