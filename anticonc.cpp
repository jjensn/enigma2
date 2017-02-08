#include "client_headers.h"
#include "anticonc.h"

CAnticonc gAnticonc;

DECLARE_COMMAND_2(gAnticonc, Anticonc);

bool CAnticonc::Pre_HUD_Init(void)
{
	HOOK_COMMAND_2("e_anticonc",Anticonc);
	return true;
}

void CAnticonc::Cmd_Anticonc(void)
{
	if(gEngine.Cmd_Argc() < 2)
	{
		econsoleprint("valid e_anticonc commands: noconc xhair off",true);
		return;
	}

	if(!strcmp(gEngine.Cmd_Argv(1), "noconc"))
	{
		gAnticonc.bNoConc = gAnticonc.bNoConc ? 0 : 1;
		gAnticonc.bXHair = 0;

		if(gAnticonc.bNoConc)
			econsoleprint("disabled the conc effect",true);
		else
			econsoleprint("enabled the conc effect",true);
	}
	if(!strcmp(gEngine.Cmd_Argv(1), "xhair"))
	{
		gAnticonc.bXHair = gAnticonc.bXHair ? 0 : 1;
		gAnticonc.bNoConc = 0;

		if(gAnticonc.bXHair)
			econsoleprint("concussion xhair enabled",true);
		else
			econsoleprint("concussion xhair disabled",true);
	}
}

void CAnticonc::Pre_HUD_Redraw(float flTime,int intermission)
{
	if(!(gAnticonc.bInConc) || !(gAnticonc.bXHair)) return;

	// we can assume that we are in a concussion state and we are using xhair mode

	cl_entity_s *pMe;

	pMe = pEngine->GetLocalPlayer();

	if(!pMe || !pMe->curstate.solid || !pMe->angles)
		return;

		vec3_t vecEnd, up, right, forward, EntViewOrg;

		VectorCopy(pMe->origin,EntViewOrg);
			
		if (pMe->curstate.usehull == 0)
			EntViewOrg[2] += 28;
		else if (pMe->curstate.usehull == 1)
			EntViewOrg[2] += 14;
			
		vec3_t myAngles;
		pEngine->GetViewAngles((float*)myAngles);
			
		pEngine->pfnAngleVectors (myAngles, forward, right, up);
		vec3_t vecBegin(EntViewOrg);
			
		vecEnd = vecBegin + forward * 5000;
		float ang = myAngles[2];
		vecEnd[2] = (vecEnd[2] + sinf(ang*10)*5000);

		Vector vScreenPos;

		if(CalcScreen(vecEnd,vScreenPos))
		{
            vScreenPos.x = (int)vScreenPos.x;
			vScreenPos.y = (int)vScreenPos.y;

			float r;//= 240;
			float g;// = 179;
			int b = 0;
			int a = 255;

			r = gAnticonc.iConcAmt / 100.0;

			r = r * 255;

			g = 255 - r;

			gEngine.pfnFillRGBA(vScreenPos.x - 14, vScreenPos.y, 9, 1,r,g,b,a);
			gEngine.pfnFillRGBA(vScreenPos.x +5, vScreenPos.y, 9, 1,r,g,b,a);
			gEngine.pfnFillRGBA(vScreenPos.x, vScreenPos.y - 14, 1, 9,r,g,b,a);
			gEngine.pfnFillRGBA(vScreenPos.x, vScreenPos.y + 5, 1, 9,r,g,b,a);
			gEngine.pfnFillRGBA(vScreenPos.x, vScreenPos.y , 1, 1,r,g,b,a);
		}

		/*pmtrace_t *vis;

		vis = pEngine->PM_TraceLine(vecBegin, (float*)vecEnd, PM_TRACELINE_ANYVISIBLE, 2, -1);

		pEngine->pEfxAPI->R_SparkShower(vis->endpos);*/
}
