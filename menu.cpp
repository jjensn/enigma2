/*        _______________   
         |  ___________  |
		 | |   ________| | ____________menu.cpp_______________
         | |  |______    |/ ________   ________   ___     ___ \
         | |   ______|   ||  ____  | |  ____  | |___|___|___| |
         | |  |________  || |    | | | |____| |  ___|___|___  |
         | |___________| ||_|    |_| |________| |___|   |___| |
         |_______________|\___________________________________/  */

/*
	User-Configurable, Multi-Menu System.
	*Basic Menu Code + Draw code - equ1n0x
	*File Parser + Revamped Menu Code - 8BALL
*/
               
#include "client_headers.h"
#pragma warning (disable:4244) // float to int loss of data

struct menuEntry 
{
	char title[128];
	char command[128];
	int r;
	int g;
	int b;
};

struct menu_t
{
	char title[128];
	menuEntry entrys[10];
	char layout[25];
	int title_r;
	int title_g;
	int title_b;
	int bg_r, bg_g, bg_b, bg_a;
	int w;

};

menu_t MenuData;

/*Global*/
CMenu gMenu;

DECLARE_COMMAND_2(gMenu, Menu);
DECLARE_COMMAND_2(gMenu, CloseMenu);

bool CMenu::Pre_HUD_Init (void)
{
	//Hook Commands Here
	gEngine.pfnRegisterVariable("e2_menu_style", "1", FCVAR_ARCHIVE); //menu style, 1 = borders + background, 2 = nothing
	gEngine.pfnRegisterVariable("e2_menu_autoclose", "1", FCVAR_ARCHIVE); //option to close menu after each command
	gEngine.pfnRegisterVariable("e2_menu_x", "200", FCVAR_ARCHIVE); //x coord
	gEngine.pfnRegisterVariable("e2_menu_y", "200", FCVAR_ARCHIVE); //y coord

	HOOK_COMMAND_2("e_menu", Menu);
	HOOK_COMMAND_2("e_menuclose", CloseMenu);
	return true;
}

int CMenu::DrawLen(char *fmt)
{
	int len=0;
	for ( char * p = fmt; *p; p++ ) len += g_screeninfo.charWidths[*p]; 
	return len;
}


void CMenu::Cmd_Menu(void)
{
	char buffer[128];


	if(menu_active)
	{
		menu_active = false;
		return;
	}
	else if(gEngine.Cmd_Argc() < 2)
	{
		Parse("main");
	}
	else
	{
		sprintf(buffer, "%s", gEngine.Cmd_Argv(1));
		Parse(buffer);
	}
}
void CMenu::Cmd_CloseMenu(void) { menu_active = false; }

void CMenu::Parse(char *menuName)
{
	char buff[4096];
	char *line;
	int  w;
	int  entryNum = 0;
	int  layoutNum = 0;
	int  r = 255;
	int  g = 255;
	int  b = 255;
	int  temp=0;
	int  temp2=0;
	bool quoteOpen;
	bool badformat = false;
	FILE *fp;


	
	MenuData.bg_a = 100;
	MenuData.bg_r = 70;
	MenuData.bg_g = 70;
	MenuData.bg_b = 70;
	MenuData.layout[0] = '\0';

	sprintf(buff,"e2\\menus\\%s.mnu",menuName);

	if(fp=fopen(buff, "rt") )
	{

		bool gottitle = false;
		MenuData.title[0] = '\0';
		while(!gottitle)
		{
			if(line = fgets(buff,1024,fp))
			{
				sprintf(buff, "%s",line);
				if(!strncmp(buff,"#rgb",4))
				{
					temp = 0;
					while(!(strchr("1234567890",buff[temp])) )
						temp++;
					r = atoi(buff+temp);
					temp += 2;
					if(r>9)
						temp++;
					if(r>99)
						temp++;
					g = atoi(buff+temp);
					temp += 2;
					if(g>9)
						temp++;
					if(g>99)
						temp++;
					b = atoi(buff+temp);
				}
				else if(!strncmp(buff,"#bgcolor",8))
				{

					temp = 0;
					while(!(strchr("1234567890",buff[temp])) )
						temp++;
					MenuData.bg_r = atoi(buff+temp);
					temp += 2;
					if(MenuData.bg_r>9)
						temp++;
					if(MenuData.bg_r>99)
						temp++;
					MenuData.bg_g = atoi(buff+temp);
					temp += 2;
					if(MenuData.bg_g>9)
						temp++;
					if(MenuData.bg_g>99)
						temp++;
					MenuData.bg_b = atoi(buff+temp);
					temp += 2;
					if(MenuData.bg_b>9)
						temp++;
					if(MenuData.bg_b>99)
						temp++;
					MenuData.bg_a = atoi(buff+temp);
					
				}
				else if(strncmp(buff,"//",2) && strcmp(buff,"\n") && strcmp(buff," "))
				{
					strcpy(MenuData.title,buff);


					temp = strlen(MenuData.title);
					MenuData.title[temp-1] = '\0';
					
					MenuData.w = DrawLen(MenuData.title);

					MenuData.title_r = r;
					MenuData.title_g = g;
					MenuData.title_b = b;
					gottitle = true;
				}

			}
			else
			{
				gottitle = true;
				badformat = true;
			}
		}


		if(MenuData.title[0] == '\0')
			badformat = true;


		while((line = fgets(buff, 1024, fp)) && entryNum<10 && !badformat)
		{
			sprintf(buff,"%s",line);
			if(buff[0] == '#')
			{
				
				if(!strncmp(buff,"#rgb",4))
				{

					temp = 0;
					while(!(strchr("1234567890",buff[temp])) )
						temp++;
					r = atoi(buff+temp);
					temp += 2;
					if(r>9)
						temp++;
					if(r>99)
						temp++;
					g = atoi(buff+temp);
					temp += 2;
					if(g>9)
						temp++;
					if(g>99)
						temp++;
					b = atoi(buff+temp);
				}
				else if(!strncmp(buff,"#bgcolor",8))
				{
					temp = 0;
					while(!(strchr("1234567890",buff[temp])) )
						temp++;
					MenuData.bg_r = atoi(buff+temp);
					temp += 2;
					if(MenuData.bg_r>9)
						temp++;
					if(MenuData.bg_r>99)
						temp++;
					MenuData.bg_g = atoi(buff+temp);
					temp += 2;
					if(MenuData.bg_g>9)
						temp++;
					if(MenuData.bg_g>99)
						temp++;
					MenuData.bg_b = atoi(buff+temp);
					temp += 2;
					if(MenuData.bg_b>9)
						temp++;
					if(MenuData.bg_b>99)
						temp++;
					MenuData.bg_a = atoi(buff+temp);
				}
				else if(!strncmp(buff,"#space",6) )
				{
					MenuData.layout[layoutNum] = 's';
					layoutNum++;
					MenuData.layout[layoutNum] = '\0';
				}
				else if(!strncmp(buff,"#line",5) )
				{
					MenuData.layout[layoutNum] = 'l';
					layoutNum++;
					MenuData.layout[layoutNum] = '\0';
				}
			}//buff[0] == #
			if(buff[0] == '\"')
			{
				
				quoteOpen = true;
				for(temp = 0; temp<127 && quoteOpen; temp++)
				{
					if(buff[temp+1] == '\0')
					{
						quoteOpen = false;
						badformat = true;
					}
					else if(buff[temp+1] == '\"')
						quoteOpen = false;
					else
					{
						MenuData.entrys[entryNum].title[temp] = buff[temp+1];
						MenuData.entrys[entryNum].title[temp+1] = '\0';
					}
				}

				
				w = DrawLen(MenuData.entrys[entryNum].title);

				if(w>MenuData.w)
					MenuData.w = w;

				temp++;

				while(buff[temp] != '\"' && buff[temp] != '\0' && !badformat)
					temp++;
				
				
				if(buff[temp] == '\0' || badformat)
					badformat = true;
				else
				{
					temp++;
					temp2 = 0;
					quoteOpen = true;
					while(quoteOpen && temp2 < 127)
					{
						if(buff[temp] == '\0')
						{
							quoteOpen = false;
							badformat = true;
						}
						else if(buff[temp] == '\"')
						{
							quoteOpen = false;
						}
						else
						{
							MenuData.entrys[entryNum].command[temp2] = buff[temp];
							temp++;
							temp2++;
							MenuData.entrys[entryNum].command[temp2] = '\0';
						}
					}
					

					temp2++;
					MenuData.layout[layoutNum] = ' ';
					layoutNum++;
					MenuData.layout[layoutNum] = '\0';
					MenuData.entrys[entryNum].r = r;
					MenuData.entrys[entryNum].g = g;
					MenuData.entrys[entryNum].b = b;
				}
				entryNum++;
			}//buff[0] = '\"'
		}//while
		


		while(entryNum < 10)
		{
			sprintf(MenuData.entrys[entryNum].command, "e_menu %s", menuName);
			MenuData.entrys[entryNum].title[0]='\0';
			entryNum++;
		}

		fclose(fp);
	}//if
	else
	{
		econsoleprint("menu file could not be opened",false);
		return;
	}
	if(badformat)
		econsoleprint("bad menu file format",false);
	else
		menu_active = true;
}

void CMenu::Post_HUD_Redraw (float flTime, int intermission)
{
	/* Drawing menu - Title near bottom */
	if (menu_active)
	{
		int x = gEngine.pfnGetCvarFloat("e2_menu_x") + 10;
		int y = gEngine.pfnGetCvarFloat("e2_menu_y");
		int i = 0;
		int layoutNum = 0;
		int entryNum = 0;
		int w = MenuData.w;
		char buf[264];

		/* Items */
		while(MenuData.layout[layoutNum] != '\0')
		{
			if(MenuData.layout[layoutNum] == 's') y += 18;	// #space

			else if(MenuData.layout[layoutNum] == 'l')		// #line
			{
				y += 18;
				gEngine.pfnFillRGBA(x, y+15, w, 1, MenuData.title_r, MenuData.title_g, MenuData.title_b, 255);
			}
			else
			{
				y += 18;
				if((entryNum) == 9)		sprintf(buf, "0. %s", MenuData.entrys[entryNum].title); //change to draw '0' instead of'10'
				else					sprintf(buf, "%d. %s", entryNum+1, MenuData.entrys[entryNum].title);
				
				DrawHudString(x, y, MenuData.entrys[entryNum].r, MenuData.entrys[entryNum].g, MenuData.entrys[entryNum].b, buf);
				entryNum++;
			}

			layoutNum++;
		}

		int x2 = gEngine.pfnGetCvarFloat("e2_menu_x");
		int x3 = MenuData.w+40;
		int y2 = gEngine.pfnGetCvarFloat("e2_menu_y");
		int y3 = (strlen(MenuData.layout)*18+18);
		int tx = x2+((MenuData.w-DrawLen(MenuData.title)+40)/2);

		/* Draw Background + Borders */
		if (gEngine.pfnGetCvarFloat("e2_menu_style") == 1)
		{
			//					  x			y			w	 h		r					g					b					a
			gEngine.pfnFillRGBA(x2,		y2,			1,	 y3+14,	MenuData.title_r,	MenuData.title_g,	MenuData.title_b,	255); //left
			gEngine.pfnFillRGBA(x2+x3,	y2,			1,	 y3+14,	MenuData.title_r,	MenuData.title_g,	MenuData.title_b,	255); //right
			gEngine.pfnFillRGBA(x2,		y2,			x3,  1,		MenuData.title_r,	MenuData.title_g,	MenuData.title_b,	255); //top
			gEngine.pfnFillRGBA(x2,		y2+20,		x3,  1,		MenuData.title_r,	MenuData.title_g,	MenuData.title_b,	255); //middle
			gEngine.pfnFillRGBA(x2,		y2+y3+14,	x3+1,1,		MenuData.title_r,	MenuData.title_g,	MenuData.title_b,	255); //bottom
			gEngine.pfnFillRGBA(x2,		y2,			x3,  y3+14,	MenuData.bg_r,		MenuData.bg_g,		MenuData.bg_g,		MenuData.bg_a); //background
			/* Draw Title */
			DrawHudString(tx, y2, MenuData.title_r, MenuData.title_g, MenuData.title_b, MenuData.title);
		}
		else
		{
			/* Draw Title */
			DrawHudString(x2, y2, MenuData.title_r, MenuData.title_g, MenuData.title_b, MenuData.title);
			gEngine.pfnFillRGBA(x2, y2+20, DrawLen(MenuData.title), 1, MenuData.title_r, MenuData.title_g, MenuData.title_b, 255);
		}
	}
}

bool CMenu::Pre_HUD_Key_Event (int eventcode, int keynum, const char *pszCurrentBinding)
{

	//keynums 1=49; 2=50; ect....
	int keynum2 = keynum - 49;
	
	if(keynum2 == -1)
		keynum2 = 9;

	if (menu_active && eventcode == 1)
	{
		if(keynum2 >= 0 && keynum2 <= 9)
		{
			if (gEngine.pfnGetCvarFloat("e2_menu_autoclose") == 1) menu_active = false;
			gEngine.pfnClientCmd(MenuData.entrys[keynum2].command);
			return false;
		}
		else if(keynum == 27)
		{
			menu_active = false;
			return false;
		}
	}
	
	return true;
}

CMenu::CMenu(void)
{
	menu_active = false;
}
