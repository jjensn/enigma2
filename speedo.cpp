#include "client_headers.h"
#include "speedo.h"

CSpeedo gSpeedo;
int x,y;

DECLARE_COMMAND_2(gSpeedo, ToggleSpeedo);

bool CSpeedo::Pre_HUD_Init	(void)
{
    HOOK_COMMAND_2("e_speedo",ToggleSpeedo);
	return true;
}

void CSpeedo::Cmd_ToggleSpeedo		(void)
{
    if(gEngine.Cmd_Argc() < 2)
	{
		econsoleprint("valid e_speedo commands: text graphic off",true);
		return;
	}

	if(!strcmp(gEngine.Cmd_Argv(1), "text"))
	{
		gSpeedo.text = gSpeedo.text ? 0 : 1;
		gSpeedo.graphic = 0;

		if(gSpeedo.text)
			econsoleprint("text speedo enabled",true);
		else
			econsoleprint("text speedo disabled",true);
	}

	if(!strcmp(gEngine.Cmd_Argv(1), "graphic"))
	{
		gSpeedo.graphic = gSpeedo.graphic ? 0 : 1;
		gSpeedo.text = 0;

		x = g_screeninfo.iWidth*0.4;
		y = g_screeninfo.iCharHeight;

		if(gSpeedo.graphic)
			econsoleprint("graphic speedo enabled",true);
		else
			econsoleprint("graphic speedo disabled",true);
	}

	if(!strcmp(gEngine.Cmd_Argv(1), "off"))
	{
		gSpeedo.graphic = 0;
		gSpeedo.text = 0;

		econsoleprint("speedo disabled",true);
	}

}

void CSpeedo::Pre_HUD_Redraw (float flTime, int intermission)
{
	char buf[1024];
	int spdR, spdG, spdB = 0;

	int speed = (int)Vector(gPlayerinfo.ppmove_cache.velocity.x, gPlayerinfo.ppmove_cache.velocity.y, (float)0).Length();
	int maxSpeed = (int)gPlayerinfo.ppmove_cache.maxspeed;

	int barWd = g_screeninfo.iWidth*0.20;
	int spd = (int)((float)speed / ((float)maxSpeed * 2.0) * (barWd - 1) );
	if (spd > (barWd - 1))
		spd = (barWd - 1);
		
	/*240 = 240;	
		179 = 179;
		16 = 16;*/

	if(gSpeedo.text)
	{
		if(gPlayerinfo.bCached)
		{

			spdR = speed / (maxSpeed * 1.7) * 255;
				if (spdR > 255)
					spdR = 255;
			spdG = 255 - spdR;
		}

			sprintf(buf, "- - > speed - %d", (int)Vector(gPlayerinfo.ppmove_cache.velocity.x, gPlayerinfo.ppmove_cache.velocity.y, (float)0).Length());
			DrawHudString(16,416,spdR,spdG,spdB,buf);
		if(gPlayerinfo.ppmove_cache.maxspeed == 0)
			sprintf(buf, "- - > percent - infinity");
		else
			sprintf(buf, "- - > percent - %d", (int)(100*((float)Vector(gPlayerinfo.ppmove_cache.velocity.x, gPlayerinfo.ppmove_cache.velocity.y, (float)0).Length()/(float)gPlayerinfo.ppmove_cache.maxspeed)));

		DrawHudString(16,432,spdR,spdG,spdB,buf);
	}

	if(gSpeedo.graphic)
	{
		if(gPlayerinfo.bCached)
		{

			if (speed <= maxSpeed) {
				spdR = 0;
				spdG = 255;
			} else {
				spdR = speed / (maxSpeed * 1.7) * 255;
				if (spdR > 255)
					spdR = 255;
				spdG = 255 - spdR;
			}

			gEngine.pfnFillRGBA (x+1, y+1, spd, (g_screeninfo.iCharHeight)-1, spdR, spdG, spdB, 0xFF);

			gEngine.pfnFillRGBA (x, y, barWd, 1, 240, 179, 16, 0xFF);
			gEngine.pfnFillRGBA (x, y+(g_screeninfo.iCharHeight), barWd, 1, 240, 179, 16, 0xFF);

			//					   x  y  w  h	 r	  g	 b     a
			gEngine.pfnFillRGBA (x, y+1, 1, (g_screeninfo.iCharHeight)-1, 240, 179, 16, 0xFF);
			gEngine.pfnFillRGBA (x+barWd, y, 1, (g_screeninfo.iCharHeight)+1, 240, 179, 16, 0xFF);
			gEngine.pfnFillRGBA (x+barWd/2, y+1, 1, (g_screeninfo.iCharHeight)-1, 240, 179, 16, 0xFF);
			gEngine.pfnFillRGBA (x+barWd/1.1764705, y+1, 1, (g_screeninfo.iCharHeight)-1, 240, 179, 16, 0xFF);  
		}

	}
}