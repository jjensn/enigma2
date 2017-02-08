#include "client_headers.h"
#include "nosmoke.h"

CNoSmoke gNoSmoke;

DECLARE_COMMAND_2(gNoSmoke, ToggleSmoke);

bool CNoSmoke::Pre_HUD_Init	(void)
{
    HOOK_COMMAND_2("e_smoke",ToggleSmoke);
	return true;
}

void CNoSmoke::Cmd_ToggleSmoke(void)
{
	if(gNoSmoke.IsActive == 0)
	{
		// smoke off!!!
		gNoSmoke.IsActive = 1;
		econsoleprint("smoke/explosions off",true);
		return;
	}

	if(gNoSmoke.IsActive == 1)
	{
		// smoke on :(
		gNoSmoke.IsActive = 0;
		econsoleprint("smoke/explosions on",true);
		return;
	}
}

int CNoSmoke::Post_HUD_AddEntity(int type, cl_entity_s *ent,const char *modelname)
{

	if(!gNoSmoke.IsActive) return 0;

	if(g_iModID == MODID_CS)
	{
		if ((strstr(modelname, "_smoke")||strstr(modelname, "puff")||strstr(modelname,"explo")))
		{
			ent->curstate.rendermode = kRenderTransTexture;
			ent->curstate.renderamt = 5;
		}

		pEngine->pfnHookEvent("events/createsmoke.sc",nop_smoke);
	}

	if(g_iModID == MODID_TFC)
	{

		if ((strstr(modelname, "explode")))
		{
			ent->curstate.rendermode = kRenderTransTexture;
			ent->curstate.renderamt = 5;
		}

	}

	return 0;

}