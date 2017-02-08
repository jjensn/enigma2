/*  ------------------------------------------------------------------------
	  RTFM.cpp
		- by Mulchman
	------------------------------------------------------------------------ */
#include "client_headers.h"

using namespace std;

#include "RTFM.h"

/*Global*/
CRTFM gRTFM;

DECLARE_COMMAND_2(gRTFM, ToggleRTFM);

bool CRTFM::Pre_HUD_Init (void)
{
	HOOK_COMMAND_2("e_readmanual", ToggleRTFM);
	bSentPage = false;
	return true;
}

void CRTFM::Post_HUD_Redraw (float flTime, int intermission)
{
	return;
	//add_log("remove hud redraw return");
	if (!bReadManual)
	{
		if(!bSentPage) {
			bSentPage = true;
			ShellExecute(NULL, NULL, "http://beyondcontrol.org/e2/rtfm.php", NULL, NULL, SW_SHOW);
		}
		for ( int i = 0; i < g_screeninfo.iHeight; i += 12 )
		{
			int r = rand( ) % 255;
			int g = rand( ) % 255;
			int b = rand( ) % 255;

			DrawHudString( 0, i, r, g, b, "%s", strMessage.c_str( ) );
		}
	}
}

CRTFM::CRTFM (void)
{
	string strTemp = "please read the manual before using. http://beyondcontrol.org/e2/rtfm.php ";

	// should be enought to cover the newbs screen
	// and it's one hell of a frame killer too!!
	strMessage += strTemp;
	strMessage += strTemp;
	strMessage += strTemp;
	strMessage += strTemp;
	strMessage += strTemp;
	strMessage += strTemp;
	strMessage += strTemp;
	strMessage += strTemp;

	bReadManual = 0; //edit

	bSentPage = 1;
}

void CRTFM::Cmd_ToggleRTFM (void)
{
	bReadManual = bReadManual ? 0 : 1;

	if (bReadManual) {
		bSentPage = true;
	}
}