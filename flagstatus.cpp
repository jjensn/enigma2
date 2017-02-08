#include "client_headers.h"
#include "flagstatus.h"

CFlagStatus gFlagStatus;

DECLARE_COMMAND_2(gFlagStatus, ToggleFlag);

float fNadeTimer;
int i = 0;
CTimer tFlag;

bool CFlagStatus::Pre_HUD_Init	(void)
{
	sprintf(myflag,"unknown");
	sprintf(enemyflag,"unknown");
    HOOK_COMMAND_2("e_flagstatus",ToggleFlag);
	gFlagStatus.bEnemyT = false;
	gFlagStatus.bMyT = false;
	return true;
}

void CFlagStatus::Cmd_ToggleFlag(void)
{

	if(g_iModID==MODID_CS)
	{
		gFlagStatus.IsActive = 0;
		return;
	}

	if(gFlagStatus.IsActive == 0)
	{
		// draw flagstatus
		gFlagStatus.IsActive = 1;
		econsoleprint("flag status on",true);
		return;
	}

	if(gFlagStatus.IsActive == 1)
	{
		// disable flag status
		gFlagStatus.IsActive = 0;
		econsoleprint("flag status off",true);
		return;
	}
}

void CFlagStatus::Post_Hook_HudText(const char *pszString)
{

	// flag status doesn't update properly.

	//if(!strstr(pszString,"flag")) return;
	
	//if(strstr(pszString,"our_flag_taken"))
	//{
	//	// enemy team has my flag!
	//	sprintf(myflag,"carried");
	//}

	//if(strstr(pszString,"nemy_lost_flag"))
	//{
	//	// enemy dropped my flag
	//	sprintf(myflag,"dropped");
	//	fMyTimer = gEngine.GetClientTime();
	//}

	//if(strstr(pszString,"our_flag_capped")) { sprintf(myflag,"at base"); }

	//if(strstr(pszString,"ou_capped_flag")) { sprintf(enemyflag,"at base"); }

	//if((strstr(pszString,"ougotflag")) || (strstr(pszString,"ou_take_enemy_flag")) || (strstr(pszString,"ell_got_enemy_flag")))
	//	{ sprintf(enemyflag,"carried");}

	//if(strstr(pszString,"ost_enemy_flag"))
	//	{ sprintf(enemyflag,"dropped");	fEnemyTimer = gEngine.GetClientTime(); }

	//if((strstr(pszString,"our_team_capped")) || (strstr(pszString,"ell_capture")))
	//{
	//	sprintf(enemyflag,"at base");
	//}
	//if(strstr(pszString,"akes_enemy_flag"))
	//{
	//	sprintf(enemyflag,"carried");
	//}
	//if(strstr(pszString,"our_flag_returned"))
	//{
	//	sprintf(myflag,"at base");
	//}
	/*if(strstr(pszString,"nemy_flag_returned"))
	{
		sprintf(enemyflag,"at base");
	}*/

	/*if(blueflag == 1)
		sprintf(bluebuff, "base");
	else if(blueflag == 2)
		sprintf(bluebuff, "carried");
	else {
		sprintf(bluebuff, "dropped");


	if(redflag == 1)
		sprintf(redbuff, "base");
	else if(redflag == 2)
		sprintf(redbuff, "carried");
	else
		sprintf(redbuff, "dropped");*/
	//}

	//pEngine->pfnClientCmd("flaginfo");

	if(gFlagStatus.IsActive)
			pEngine->pfnClientCmd("flaginfo");

	return;
}

void CFlagStatus::Post_Hook_TxtMsg (const char *pszString)
{

}

void CFlagStatus::Pre_HUD_Redraw (float flTime, int intermission)
{
	if(!gFlagStatus.IsActive) return;

	if(!tFlag.Running())
	{
		pEngine->pfnClientCmd("flaginfo");
		//add_log("10 seconds");
		tFlag.Start ( 10 );
	}

	if(strstr(enemyflag,"unknown")) pEngine->pfnClientCmd("flaginfo");

	//pEngine->pfnClientCmd("flaginfo");

	DrawHudString(16,(g_screeninfo.iHeight/11) + 14,127,255,127,"my flag:");
	DrawHudString(16,(g_screeninfo.iHeight/11) - 15,127,255,127,"enemy flag:");

	if(strstr(myflag,"carried")) {
		DrawHudString(82,(g_screeninfo.iHeight/11) + 14,255,0,0,myflag); } else {
			DrawHudString(82,(g_screeninfo.iHeight/11) + 14,255,255,255,myflag); }

	DrawHudString(110,(g_screeninfo.iHeight/11) - 15,255,255,255,enemyflag);

	if(strstr(enemyflag,"dropped")) 
	{
		// code
		float time = gEngine.GetClientTime();
		int barWd, nade, barR, barG, barB, x, y, h = 0;

		x = 16;
		y = (g_screeninfo.iHeight/11)+5;
		h = g_screeninfo.iCharHeight*0.5;
		barWd = 100;

		float flagtime = (time - (float)fEnemyTimer);
		int maxTime = 65;

		if (flagtime > maxTime)
		{
			fEnemyTimer = 0;
		}

		nade = (int)((float)flagtime / ((float)maxTime) * (barWd - 1) );//(int)((float)speed * (barWd - 1));
		if (nade > (barWd - 1))
			nade = (barWd - 1);

		barR = 127;	
		barG = 255;
		barB = 127;

		int borderR = barR - 100;
		int	borderG = barG - 100;
		int borderB = barB - 100;

		
		gEngine.pfnFillRGBA (x+1, y+1, nade, h-1, barR, barG, barB, 0xFF);

		gEngine.pfnFillRGBA (x, y, barWd, 1, borderR, borderG, borderB, 0xFF);
		gEngine.pfnFillRGBA (x, y+h, barWd, 1, borderR, borderG, borderB, 0xFF);

		//					   x  y  w  h  r   g	b     a
		gEngine.pfnFillRGBA (x, y, 1, h, borderR, borderG, borderB, 0xFF);
		gEngine.pfnFillRGBA (x+barWd, y, 1, h+1, borderR, borderG, borderB, 0xFF);
	}

	if(strstr(myflag,"dropped")) 
	{
		// code
		float time = gEngine.GetClientTime();
		int barWd, nade, barR, barG, barB, x, y, h = 0;

		x = 16;
		y = (g_screeninfo.iHeight/11)+35;
		h = g_screeninfo.iCharHeight*0.5;
		barWd = 100;

		float flagtime = (time - (float)fMyTimer);
		int maxTime = 65;

		if (flagtime > maxTime)
		{
			fMyTimer = 0;
		}

		nade = (int)((float)flagtime / ((float)maxTime) * (barWd - 1) );//(int)((float)speed * (barWd - 1));
		if (nade > (barWd - 1))
			nade = (barWd - 1);

		barR = 127;	
		barG = 255;
		barB = 127;

		int borderR = barR - 100;
		int	borderG = barG - 100;
		int borderB = barB - 100;
		
		gEngine.pfnFillRGBA (x+1, y+1, nade, h-1, barR, barG, barB, 0xFF);

		gEngine.pfnFillRGBA (x, y, barWd, 1, borderR, borderG, borderB, 0xFF);
		gEngine.pfnFillRGBA (x, y+h, barWd, 1, borderR, borderG, borderB, 0xFF);

		//					   x  y  w  h  r   g	b     a
		gEngine.pfnFillRGBA (x, y, 1, h, borderR, borderG, borderB, 0xFF);
		gEngine.pfnFillRGBA (x+barWd, y, 1, h+1, borderR, borderG, borderB, 0xFF);
	}
}

void CFlagStatus::Pre_Hook_ConPrint(const char *pszString)
{

	//add_log("%s",pszString);

	if(strstr(pszString, "The enemy flag is in their base"))
		{
			/*if(gFlagStatus.team == 1)
				gFlagStatus.redflag = 1;
			else
				gFlagStatus.blueflag = 1;*/
			sprintf(enemyflag,"base");
			x1 = 0;
		}
		if(strstr(pszString, "Your flag is in your base"))
		{
			/*if(gFlagStatus.team == 1)
				gFlagStatus.blueflag = 1;
			else
				gFlagStatus.redflag = 1;*/
			sprintf(myflag,"base");
			x2 = 0;
		}
		if(strstr(pszString, "The enemy flag is being carried"))
		{
			sprintf(enemyflag,"carried");

			x1 = 0;

		}
		if(strstr(pszString, "Your flag is being carried"))
		{
			/*if(gFlagStatus.team == 1)
				gFlagStatus.blueflag = 2;
			else
				gFlagStatus.redflag = 2;*/
			sprintf(myflag,"carried");

			x2 = 0;
		}
		if((strstr(pszString, "The enemy flag is lying around")) || (strstr(pszString,"enemy flag has been dropped")))
		{

			sprintf(enemyflag,"dropped");

			x1 = 1;

		}

		if((strstr(pszString, "Your flag is lying around")) || (strstr(pszString,"your flag has been dropped")))
		{
			/*if(gFlagStatus.team == 1)
				gFlagStatus.blueflag = 3;
			else
				gFlagStatus.redflag = 3;*/
			sprintf(myflag,"dropped");

			x2 = 1;

		}
}