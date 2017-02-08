// HUD Channel stuff

#include "client_headers.h"
#include "fancyhud.h"

CHUDChannel Channel[64];

void Fancy_HUD_Redraw(float flTime, int intermission)
{

	int i;

	// Display the HUD channels
	for(i = 0; i < 64; i++)
	{
		Channel[i].DisplayHUDChannel(i);
	}

	/*for(i = 0; i < 1024; i++)
		Channel[i].bRendered[i] = 0;*/
	
}

void CHUDChannel::WriteHUDChannel (float fPeriod, char *WriteText)
{
	if(strlen(WriteText) < 1) return;

	sprintf(Text, "- - > %s", WriteText);
	fStartTime = gEngine.GetClientTime();
	fEndTime = fStartTime + fPeriod;
}

void CHUDChannel::DisplayHUDChannel (int iSlot)
{
	// (int)((iTimelimit - gEngine.GetClientTime())/60)
	int ix;
	int iy;
	
	// Check that the times aren't messed
	if(fStartTime > gEngine.GetClientTime())
	{
		fStartTime = 0;
		fEndTime = 0;
		return;
	}

	////add_log("wha");

	if((fEndTime > gEngine.GetClientTime()) && (strlen(Text) > 0))
	{
		if(iSlot < 32)
		{
			ix = 16;
			iy = 128 + iSlot*16;
		}
		else
		{
			ix = (g_screeninfo.iWidth - 16) - (g_screeninfo.charWidths['0']*16);
			iy = 128 + (iSlot-32)*16;
		}
		////add_log("fuck");

		/*if(iSlot == HC_ROUND_TIMER) {
			if(((int)((gRulesManager.iTimelimit - gEngine.GetClientTime())/60) < 1) && (gRulesManager.iTimelimit > 0)) {
				DrawHudString(ix, iy, 255, 15, 9, Text);
			} else {
				if(gSuperHUD.fHUDBrightness > -1)
					DrawHudString(ix, iy, (gSuperHUD.HUDColor.x*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), (gSuperHUD.HUDColor.y*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), (gSuperHUD.HUDColor.z*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), Text);
				else
					DrawHudString(ix, iy, (127*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), (255*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), (127*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), Text);
			}
		} else {*/
			/*if(gSuperHUD.fHUDBrightness > -1)
			{
				DrawHudString(ix, iy, (gSuperHUD.HUDColor.x*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), (gSuperHUD.HUDColor.y*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), (gSuperHUD.HUDColor.z*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), Text);
			}
			else
			{*/
				DrawHudString(ix, iy, (127*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), (255*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), (127*(fEndTime - gEngine.GetClientTime()))/(fEndTime-fStartTime), Text);

		//		//add_log("shit");
			/*}*/
		/*}*/
	}
	else
	{
		fStartTime = 0;
		fEndTime = 0;
	}
}

CHUDChannel::CHUDChannel (void)
{
	sprintf(Text, "\0");
	fStartTime = 0;
	fEndTime = 0;
}