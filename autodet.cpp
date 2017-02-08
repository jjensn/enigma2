#include "client_headers.h"
#include "autodet.h"

CAutodet gAutodet;

DECLARE_COMMAND_2(gAutodet, Autodet);
DECLARE_COMMAND_2(gAutodet, Autodetdebug);

bool CAutodet::Pre_HUD_Init(void)
{
	gEngine.pfnRegisterVariable("e2_det_sens", "3", FCVAR_ARCHIVE);

	HOOK_COMMAND_2("e_autodet",Autodet);
	HOOK_COMMAND_2("e_debug",Autodetdebug);
	return true;
}


void CAutodet::Cmd_Autodetdebug(void)
{
	int i;
	for(i = 0; i < 8; i++)
	{
		//add_log("x: %f, y: %f, z: %f",pipes[i].X,pipes[i].Y,pipes[i].Z);
	}

	//add_log("det sens: %f",gEngine.pfnGetCvarFloat("e2_det_sens"));
	//add_log("---------------------");
}
void CAutodet::Cmd_Autodet(void)
{

	if(gEngine.Cmd_Argc() < 2)
	{
		econsoleprint("valid e_autodet commands: safe stupid off",true);
		return;
	}

    if(!strcmp(gEngine.Cmd_Argv(1), "safe"))
	{
		gAutodet.bSafe = gAutodet.bSafe ? 0 : 1;
		gAutodet.bStupid = 0;

		if(gAutodet.bSafe)
			econsoleprint("safe autodet enabled",true);
		else
			econsoleprint("safe autodet disabled",true);
	}

	if(!strcmp(gEngine.Cmd_Argv(1), "stupid"))
	{
		gAutodet.bStupid = gAutodet.bStupid ? 0 : 1;
		gAutodet.bSafe = 0;

		if(gAutodet.bStupid)
			econsoleprint("stupid autodet enabled",true);
		else
			econsoleprint("stupid autodet disabled",true);
	}

	if(!strcmp(gEngine.Cmd_Argv(1), "off"))
	{
		gAutodet.bStupid = 0;
		gAutodet.bSafe = 0;


		econsoleprint("autodet disabled",true);
	}	
}

int CAutodet::Post_HUD_AddEntity(int type, struct cl_entity_s *ent, const char* modelname)
{
	if((!gAutodet.bSafe) && (!gAutodet.bStupid)) return 0;

	if((!strcmp(modelname, "models/pipebomb.mdl")) && ent->curstate.solid)
	{
		hud_player_info_t OwnersPlayerInfo;

		cl_entity_s *pLocal;
		pLocal = gEngine.GetLocalPlayer();

		gEngine.pfnGetPlayerInfo(ent->curstate.owner,&OwnersPlayerInfo);
		if(pLocal->index == ent->curstate.owner && ent->curstate.skin==0)
		{
			// this is mine!
			////add_log("x: %f, y: %f, z: %f",ent->curstate.origin.x,ent->curstate.origin.y,ent->curstate.origin.z);
			////add_log("pipe: %i index %i",gAutodet.iCurrPipe,ent->index);

			gAutodet.pipes[gAutodet.iCurrPipe].X = ent->curstate.origin.x;
			gAutodet.pipes[gAutodet.iCurrPipe].Y = ent->curstate.origin.y;
			gAutodet.pipes[gAutodet.iCurrPipe].Z = ent->curstate.origin.z;
			gAutodet.pipes[gAutodet.iCurrPipe].index = ent->index;


            gAutodet.iCurrPipe += 1;

			if(gAutodet.iCurrPipe ==  8)
				gAutodet.iCurrPipe = 0;
		}
	}

	return 0;    
}

void CAutodet::Pre_HUD_Redraw (float flTime, int intermission)
{

	if (g_iModID != MODID_TFC) return;
	
	int i;

	if((!gAutodet.bSafe) && (!gAutodet.bStupid)) return;

	cl_entity_s *pLocal;
	pLocal = gEngine.GetLocalPlayer();

	gAutodet.UpdatePipes();

	for(i = 0; i < 32; i++)
	{
		if(gESP.g_playerdata[i].bCurrent)
		{
            cl_entity_s *pEnt;		
			pEnt = pEngine->GetEntityByIndex(i + 1);


			if(pEnt && pEnt->player && pEnt->index != pLocal->index && pEnt->curstate.team != pLocal->curstate.team &&
				pEnt->curstate.solid && abs(pEnt->curstate.messagenum - pLocal->curstate.messagenum) < 6)
			{
				// its a player, and not me!

				float pX, pY, pZ; 
				int k;
				bool bNear, bVisible;


				pX = gESP.g_playerdata[i].vOrigin.x;
				pY = gESP.g_playerdata[i].vOrigin.y;
				pZ = gESP.g_playerdata[i].vOrigin.z;

				gAutodet.sens = gEngine.pfnGetCvarFloat("e2_det_sens");
			
				for(k = 0; k < 8; k++)
				{
					if(gAutodet.bSafe)
					{
						bNear = bIsNear(pX,pipes[k].X,pY,pipes[k].Y,pZ,pipes[k].Z);
						bVisible = bIsVisible(pLocal->curstate.usehull,pEnt->index);
						if(bNear && bVisible) {
							pEngine->pfnClientCmd("+attack2;-attack2");
							pEngine->pfnCenterPrint("- boom boom -"); }
					}

					if(gAutodet.bStupid)
					{
						bNear = bIsNear(pX,pipes[k].X,pY,pipes[k].Y,pZ,pipes[k].Z);
						if(bNear) { pEngine->pfnClientCmd("+attack2;-attack2"); 
						pEngine->pfnCenterPrint("- boom boom -"); }
					}
				}
			}
		}
	}
}

void CAutodet::UpdatePipes (void)
{
	int i;
	cl_entity_s *p_ent;

	for(i = 0; i < 8; i++)
	{
		p_ent = gEngine.GetEntityByIndex(pipes[i].index);

		if(p_ent && p_ent->curstate.solid && p_ent->model->name && strstr(p_ent->model->name, "pipebomb"))
		{
			cl_entity_s *pMe;

			pMe = gEngine.GetLocalPlayer();

			int a;

			a = abs(pMe->curstate.messagenum - p_ent->curstate.messagenum);

			if (a > 20)
			{
				gAutodet.pipes[i].index = 0;
				gAutodet.pipes[i].X = -1;
				gAutodet.pipes[i].Y = -1;
				gAutodet.pipes[i].Z = -1;
			}

		}
	}
}

bool CAutodet::bIsNear (float x1, float x2, float y1, float y2, float z1, float z2)
{
	float diffx, diffy, diffz;
	int formSens;

	diffx = x2 - x1;
	diffy = y2 - y1;
	diffz = z2 - z1;

	formSens = (gAutodet.sens) * 25;
	float fRange = formSens; // problem here

	if(sqrt(diffx*diffx + diffy*diffy + diffz*diffz) <= fRange)
	{
		return true;
	}

return false;
}

bool CAutodet::bIsVisible(bool ducking, int entindex)
{
	vec3_t playerPosition;
	pmtrace_t *visible;

	cl_entity_s *p_det, *pMe;

	p_det = gEngine.GetEntityByIndex(entindex);
	pMe = gEngine.GetLocalPlayer();

	playerPosition = p_det->origin;

	if ( ducking )
		playerPosition = playerPosition + Vector(0,0,12);
	else
		playerPosition = playerPosition + Vector(0,0,17);

	visible = gEngine.PM_TraceLine(pMe->origin, playerPosition, 0, 2, -1);

	return ( visible->fraction >= .5 ) ? true : false;	
}
