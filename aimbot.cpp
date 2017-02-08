#include "client_headers.h"

CAimbot gAimbot;

int iOld = -1;
bool bRun = false;

DECLARE_COMMAND_2(gAimbot,Aimbot);
bool bOldKeyState[1024];

void CAimbot::DoAimbot(usercmd_t *cmd)
{
	static Vector vOldPlayerAngles = Vector(0, 0, 0);
	static bool bInAutomation = false;

	int iTarget;
	cl_entity_t *pLocal, *pTarget, *pOld;
	float fBlock = 0.0f;
	Vector vAim, vAimAngles, vViewOffset, vSpread, vTemp;
	bool bGotTargetThisFrame = false;
	bool bInAutomationLastFrame = false;

	if(!gAimbot.bAimbot)
		return;

	pLocal = gEngine.GetLocalPlayer();

	if(!pLocal)
		return;

	if(pLocal->curstate.solid == SOLID_NOT || pLocal->curstate.movetype != MOVETYPE_WALK)
		return;

	gEngine.pEventAPI->EV_LocalPlayerViewheight(vViewOffset);
	
	iTarget = ChooseTarget(cmd);

	if(!bIsVisible(pLocal->curstate.usehull,iTarget + 1))
		iTarget = -1;

	if(iTarget != -1)
	{
		if(iTarget != iOld)
		{
			if(!bRun)
			{
				bRun = true;
				iOld = iTarget;
			}
			else
			{
				// target changed. check to see if they are still alive and visible.
				// if so, keep the old target to prevent the aimbot look.

				pOld = gEngine.GetEntityByIndex(iOld + 1);

				if(pOld->curstate.solid == SOLID_NOT || pOld->curstate.movetype != MOVETYPE_WALK)
				{
					// old player is dead. okay to switch.
					iOld = iTarget;
				}
				else
				{
					fBlock = GetBlockingDistance(pLocal->origin + vViewOffset, pOld->origin);

					if(fBlock < 20.0f && bIsVisible(pLocal->curstate.usehull,iOld + 1))
					{
						// player is still visibile, do not switch the target
						iTarget = iOld;
					}
					else
					{
						// player is not visible, switch.
						iOld = iTarget;
					}			

				}
			}
		}
	}

	hud_player_info_t enemy;
	char buff[1028];

	if(iTarget != -1)
	{
		//bInAutomationLastFrame = bInAutomation;

		pTarget = gEngine.GetEntityByIndex(iTarget + 1);

		pEngine->pfnGetPlayerInfo(iTarget + 1, &enemy);

		//sprintf(buff,"target: %s",enemy.name);

		//gEngine.pfnCenterPrint(buff);

		gESP.g_playerdata[iTarget].bTarget = true;

		if(GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if(pTarget && gESP.g_playerdata[iTarget].bCurrent && pLocal->curstate.solid != SOLID_NOT && pLocal->curstate.movetype != MOVETYPE_TOSS )
			{

				vAim = pTarget->origin - (pLocal->origin + vViewOffset);
				vAim.z *= -1;
				VectorAngles(vAim, vAimAngles);
				
				if(vAimAngles.x >= 180) vAimAngles.x -= 360;

				Vector vMyVecs;

				float fRate = gEngine.pfnGetCvarFloat("e2_aim_rate"); 

				VectorCopy(cmd->viewangles,vMyVecs);

//				char cTemp[256];

//				sprintf(cTemp,"X1: %f, Y1: %f, Z1: %f, X2: %f, Y2: %f, Z2: %f",vMyVecs.x,vMyVecs.y,vMyVecs.z,vAimAngles.x,vAimAngles.y,vAimAngles.z);

//				pEngine->pfnConsolePrint(cTemp);
//				add_log(cTemp);
//				pEngine->pfnDrawConsoleString(0,0,cTemp);
//				pEngine->pfnDrawConsoleStringLen(cTemp,10,10);
//				pEngine->pfnDrawConsoleStringLen(cTemp,20,20);

				while((vMyVecs.x != vAimAngles.x) && (vMyVecs.y != vAimAngles.y))
				{

					if((abs(vMyVecs.x - vAimAngles.x))  < fRate) {
						vMyVecs.x = vAimAngles.x; }
					else
					{
						if(vMyVecs.x > vAimAngles.x){
							vMyVecs.x = vMyVecs.x - fRate;
						}
						else if(vMyVecs.x < vAimAngles.x){
							vMyVecs.x = vMyVecs.x + fRate; }
					}

					if((abs(vMyVecs.y - vAimAngles.y))  < fRate) {
						vMyVecs.y = vAimAngles.y; }
					else
					{
						if(vMyVecs.y > vAimAngles.y){
							vMyVecs.y = vMyVecs.y - fRate;
						}
						else if(vMyVecs.y < vAimAngles.y){
							vMyVecs.y = vMyVecs.y + fRate; }
					}

					gEngine.SetViewAngles(vMyVecs); 
				}
				//gEngine.SetViewAngles(vAimAngles); 
		}
	}
	else
		SetOldKeyState(VK_SPACE, false);
	}
}

//int ChooseTarget(void)
int CAimbot::ChooseTarget (usercmd_t *cmd)
{
	cl_entity_t *pLocal, *pTarget;
	Vector vViewOffset;
	float fBlockingDistance = 0;
	float fEnemyAimDotProduct = 0;
	int iHighestWeighting = 0;
	int iBestTarget = -1;
	int i;
	bool bIsVisible = 0;

	pLocal = gEngine.GetLocalPlayer();
	if(!pLocal)
		return -1;

	gEngine.pEventAPI->EV_LocalPlayerViewheight(vViewOffset);

	for(i = 0; i < 32; i++)
	{
		pTarget = gEngine.GetEntityByIndex(i + 1);
		if(pTarget && gESP.g_playerdata[i].bCurrent && gESP.g_playerdata[i].iTeam != gESP.g_playerdata[pLocal->index - 1].iTeam && pTarget->index != pLocal->index && pTarget->curstate.solid != SOLID_NOT && pTarget->curstate.movetype != MOVETYPE_TOSS)
		{
			fEnemyAimDotProduct = GetEnemyAimDotProduct(pTarget);
			
			fBlockingDistance = GetBlockingDistance(pLocal->origin + vViewOffset, pTarget->origin);
			//if(fBlockingDistance < 48.0f)
			if(fBlockingDistance < 20.0f)
			{
				gESP.g_playerdata[i].iAimWeighting += (8192 - (pTarget->origin - (pLocal->origin + vViewOffset)).Length())/200;
				//if(fBlockingDistance < 8.0f) gESP.g_playerdata[i].iAimWeighting += 2000;
				//if(fEnemyAimDotProduct > DOT_FACINGHEMISPHERE) gESP.g_playerdata[i].iAimWeighting += fEnemyAimDotProduct*250;
				//if(fEnemyAimDotProduct > DOT_AIMING) gESP.g_playerdata[i].iAimWeighting += 2000;
				//gESP.g_playerdata[i].iAimWeighting = 0;
			}
			else
				gESP.g_playerdata[i].iAimWeighting = 0;

		}

		Vector vScreen;

		if(gESP.g_playerdata[i].iAimWeighting > iHighestWeighting && gESP.g_playerdata[i].iAimWeighting > 0 && CalcScreen(pTarget->origin,vScreen))
		{

			Vector vPOS;

			CalcScreen(pTarget->origin,vPOS);

			float x1 = g_screeninfo.iWidth / 2;
			float y1 = g_screeninfo.iHeight / 2;
			float x2 = vPOS.x;
			float y2 = vPOS.y;

			float dist = sqrt(pow(x2-x1,2) + pow(y2-y1,2));
			float fSens = gEngine.pfnGetCvarFloat("e2_aim_sens"); 

			//add_log("%f",dist);

			if(dist < fSens)
			{
				iBestTarget = i;
				iHighestWeighting = gESP.g_playerdata[i].iAimWeighting;
			}

		}
	}
	
	return iBestTarget;
}

// calculates the amount of space blocked between two positions, used for autowall
float CAimbot::GetBlockingDistance(float *vec1, float *vec2)
{
	cl_entity_t *pLocal;

	pLocal = gEngine.GetLocalPlayer();

	if(!pLocal)
		return 0.0f;
	// old PM_ method
	/*
	pmtrace_t *tr;
	Vector vEnd1, vEnd2;
	float f1, f2;

	tr = gEngine.PM_TraceLine(vec1, vec2, PM_TRACELINE_PHYSENTSONLY, 2, -1);
	vEnd1 = tr->endpos;
	f1 = tr->fraction;
	tr = gEngine.PM_TraceLine(vec2, vec1, PM_TRACELINE_PHYSENTSONLY, 2, -1);
	vEnd2 = tr->endpos;
	f2 = tr->fraction;
	*/

	// new EV_ method
	pmtrace_t *tr;
	Vector vEnd1, vEnd2;
	float f1, f2;

	tr = gEngine.PM_TraceLine(vec1, vec2, PM_TRACELINE_PHYSENTSONLY, 2, pLocal->index);
	//gEngine.pEventAPI->EV_PlayerTrace(vec1, vec2, PM_STUDIO_IGNORE, pLocal->index, &tr);
//	vEnd1 = tr.endpos;
//	f1 = tr.fraction;
	vEnd1 = tr->endpos;
	f1 = tr->fraction;

	tr = gEngine.PM_TraceLine(vec2, vec1, PM_TRACELINE_PHYSENTSONLY, 2, pLocal->index);
	//gEngine.pEventAPI->EV_PlayerTrace(vec2, vec1, PM_STUDIO_IGNORE, pLocal->index, &tr);
//	vEnd2 = tr.endpos;
//	f2 = tr.fraction;
	vEnd2 = tr->endpos;
	f2 = tr->fraction;


	if(f1 == 1.0f || f2 == 1.0f)
	{
		////add_log("traceline indicates unblocked path");
		return 0.0f;
	}

	//gEngine.pEfxAPI->R_SparkShower(vEnd1);
	//gEngine.pEfxAPI->R_SparkShower(vEnd2);

	////add_log("traceline indicates path blocked between (%f, %f, %f) and (%f, %f, %f) distance %f", vEnd1.x, vEnd1.y, vEnd1.z, vEnd2.x, vEnd2.y, vEnd2.z, (vEnd1 - vEnd2).Length());
	return (vEnd1 - vEnd2).Length();
}

float CAimbot::GetEnemyAimDotProduct(cl_entity_t *pEnemy)
{
	cl_entity_t *pLocal;
	Vector vEnemyAim2D, vIdealAim2D, vNullVec1, vNullVec2;

	pLocal = gEngine.GetLocalPlayer();
	if(!pLocal || !pEnemy) return 0.0f;
    
	gEngine.pfnAngleVectors(pEnemy->angles, vEnemyAim2D, vNullVec1, vNullVec2);
	vEnemyAim2D.z = 0;
	vEnemyAim2D = vEnemyAim2D.Normalize();
    
	vIdealAim2D = pLocal->origin - pEnemy->origin;
	vIdealAim2D.z = 0;
	vIdealAim2D = vIdealAim2D.Normalize();

	////add_log("enemy found with aim dotproduct %f", DotProduct(vEnemyAim2D, vIdealAim2D));

	return DotProduct(vEnemyAim2D, vIdealAim2D);
}

bool CAimbot::bIsVisible(bool ducking, int entindex)
{
	cl_entity_s *pMe;
	cl_entity_s *p_det;
	pMe = gEngine.GetLocalPlayer();
	vec3_t playerPosition;

	p_det = gEngine.GetEntityByIndex(entindex);
	playerPosition = p_det->origin;

	if ( ducking )
		playerPosition = playerPosition + Vector(0,0,12);
	else
		playerPosition = playerPosition + Vector(0,0,17);
	
	pmtrace_t *vis;
	
	vis = gEngine.PM_TraceLine(pMe->origin, (float*)playerPosition, PM_TRACELINE_ANYVISIBLE, 2, -1);

	return ( vis->fraction >= .85 ) ? true : false;	
}

void CAimbot::Pre_HUD_Init(void)
{
	HOOK_COMMAND_2("e_aimbot",Aimbot);
	gEngine.pfnRegisterVariable("e2_aim_rate", "1", FCVAR_ARCHIVE);
	gEngine.pfnRegisterVariable("e2_aim_sens", "100", FCVAR_ARCHIVE);
}

void CAimbot::Cmd_Aimbot(void)
{
	if(gAimbot.bAimbot == 0)
	{
		// aimbot on!
		gAimbot.bAimbot = 1;
		econsoleprint("aimbot enabled",true);
		return;
	}

	if(gAimbot.bAimbot == 1)
	{
		// aimbot off
		gAimbot.bAimbot = 0;
		econsoleprint("aimbot disabled",true);
		return;
	}
}