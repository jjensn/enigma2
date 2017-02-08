#include "client_headers.h"

#define LOG_TO_ROOT_DIR

FILE *g_fpLog = NULL;
char g_szLogFile[MAX_PATH] = "";
SCREENINFO g_screeninfo;
int bVerbose = 1;

int g_iModID = MODID_NOTCHECKED;

#define InitPoint 0x01D17304 // Initialize exports offset
char szOffset[255], szTemp[255];

//Client.dll linker table
DWORD  tExportsList[35] ={  (DWORD)InitPoint,                //INITIALIZE
                            (DWORD)InitPoint + 0x23,        //HUD_VidInit
                            (DWORD)InitPoint + 0x47,        //HUD_Init
                            (DWORD)InitPoint + 0x6B,        //HUD_Redraw    
                            (DWORD)InitPoint + 0x8E,        //HUD_UpdateClientData
                            (DWORD)InitPoint + 0xB2,        //HUD_Reset
                            (DWORD)InitPoint + 0xD6,        //HUD_PlayerMove
                            (DWORD)InitPoint + 0xF9,        //HUD_PlayerMoveInit
                            (DWORD)InitPoint + 0x11D,        //HUD_PlayerMoveTexture
                            (DWORD)InitPoint + 0x141,        //IN_ActivateMouse
                            (DWORD)InitPoint + 0x164,        //IN_DeactivateMouse
                            (DWORD)InitPoint + 0x188,        //IN_MouseEvent
                            (DWORD)InitPoint + 0x1AC,        //IN_ClearStates
                            (DWORD)InitPoint + 0x1CF,        //IN_Accumulate
                            (DWORD)InitPoint + 0x1F3,        //CL_CreateMove
                            (DWORD)InitPoint + 0x217,        //CL_IsThirdPerson
                            (DWORD)InitPoint + 0x23A,        //CL_CameraOffset
                            (DWORD)InitPoint + 0x25E,        //CAM_Think
                            (DWORD)InitPoint + 0x282,        //KB_Find
                            (DWORD)InitPoint + 0x2A5,        //V_CalcRefdef
                            (DWORD)InitPoint + 0x2C9,        //HUD_AddEntity
                            (DWORD)InitPoint + 0x2ED,        //HUD_CreateEntities
                            (DWORD)InitPoint + 0x310,        //HUD_DrawNormalTriangles
                            (DWORD)InitPoint + 0x334,        //HUD_DrawTransparentTriangles
                            (DWORD)InitPoint + 0x358,        //HUD_StudioEvent
                            (DWORD)InitPoint + 0x37B,        //HUD_Shutdown
                            (DWORD)InitPoint + 0x39F,        //HUD_TxferLocalOverrides
                            (DWORD)InitPoint + 0x40A,        //Demo_ReadBuffer
                            (DWORD)InitPoint + 0x42E,        //HUD_ConnectionlessPacket
                            (DWORD)InitPoint + 0x451,        //HUD_GetHullBounds
                            (DWORD)InitPoint + 0x475,        //HUD_Frame
                            (DWORD)InitPoint + 0x499,        //HUD_Key_Event
                            (DWORD)InitPoint + 0x4BC,        //HUD_PostRunCmd
                            (DWORD)InitPoint + 0x4E0,        //HUD_TempEntUpdate
                            (DWORD)InitPoint + 0x504  };    //HUD_GetUserEntity


//Thats the function which checks for mov opcode, and then scan for Offset
void GetClientExport( DWORD CurrentOffset )
{
    sprintf( szTemp, "Searching in -> 0x%x", CurrentOffset );
    add_log( szTemp );    
    
    PBYTE CurrentLocation = (PBYTE)CurrentOffset;
    if( CurrentLocation[0] == 0xA3 )
    {
        sprintf( szOffset, "0x%x%x%x%x", CurrentLocation[4], CurrentLocation[3], CurrentLocation[2], CurrentLocation[1] );
        sprintf( szTemp, "Client.dll export found at -> %s", szOffset );
        add_log( szTemp );
    }
    else
    {
        add_log( "invalid offset" );    
    }

    CurrentLocation = NULL;
}

void GetExportFunctions()
{
    for( int z = 0; z < 35; z++ )
    {
        GetClientExport(  tExportsList[z]  );
    }
}  

void __cdecl add_log(const char *fmt, ...)
{
#ifndef NO_LOGGING
	va_list va_alist;
	char buf[4096] = "";

	g_fpLog = fopen(g_szLogFile, "at");
   
	va_start (va_alist, fmt);
	_vsnprintf (buf + strlen(buf), sizeof(buf) - strlen(buf), fmt, va_alist);
	va_end (va_alist);

    if(g_bEngineLoaded)
		pEngine->Con_Printf("[enigma] %s\n", buf);

	if(g_fpLog)
	{
		char tbuf[256];
		_strtime(tbuf);
		fprintf (g_fpLog, "[enigma - %s] %s\n", tbuf, buf);
		fclose(g_fpLog);
	}
#endif
}

void __cdecl add_log_noformat(const char *buf)
{
#ifndef NO_LOGGING
	g_fpLog = fopen(g_szLogFile, "at");

	if(g_bEngineLoaded)
		pEngine->Con_Printf("[enigma] %s\n", buf);

	if(g_fpLog)
	{
		char tbuf[256];
		_strtime(tbuf);
		fprintf (g_fpLog, "[enigma - %s] %s\n", tbuf, buf);
		fclose(g_fpLog);
	}
#endif
}

void __cdecl setup_logging(void)
{
	HMODULE hMod;

	/*
	FILE *fpPreLog = fopen("tabrix_prelog.log", "at");

	if(!fpPreLog)
		return;
	*/

	char buf[MAX_PATH];
	char *p;

	hMod = GetModuleHandle("loader.dll");
	GetModuleFileName(hMod, buf, MAX_PATH);

	if(buf[0] == '\0')
		return;

	// scan back to '\'
	p = &(buf[strlen(buf)-2]);

	while(*p != '\\' && p > buf)
	{
		p--;
	}

	//fprintf(fpPreLog, "splitting string...\nfull: %s\nsecond part: %s\n", buf, p);
	
	*p = '\0';
	
	//fprintf(fpPreLog, "first part: %s\n", buf);

	strcat(buf, "\\enigma.log");

	strcpy(g_szLogFile, buf);

#ifdef LOG_TO_ROOT_DIR
	strcpy(g_szLogFile, "\\enigma_root.log");
#endif

	//fprintf(fpPreLog, "final log filename: %s", buf);

	add_log(" ");
	add_log("---> logging begins");

	//fclose(fpPreLog);
}

void __cdecl end_logging(void)
{
	add_log("---> logging ends");
}



const char * dump (void * buf, int size)
{
	static char textbuf[256];

    sprintf(textbuf, "%d bytes: ", size);

	for ( int i = 0; i < size; i++ )
	{
		unsigned char byte = ((unsigned char *)buf)[i];
		sprintf(textbuf+strlen(textbuf), "0x%02X(%c) ", 
				(unsigned int) byte, 
				(byte >= ' ' && byte < 128) ? byte : '?' );

		if ( strlen(textbuf) > sizeof(textbuf) - 20 )
			break;
	}

	return textbuf;
}


bool str_prefix (const char * substr, const char * str)
{
	if ( !*substr )
		return true;

	for ( ; *substr && *str; substr++, str++ )
		if ( *substr != *str )
			return true;

	return false;
}


const char * get_argument (const char *string, char *buf, size_t bufsize)
{
    while ( isspace(*string) )
        string++;

    while ( *string != '\0' && bufsize > 1 )
    {
        if ( isspace(*string) )
        {
            string++;
            break;
        }

        *buf++ = *string++;
        bufsize--;
    }

    *buf = '\0';

    while ( isspace(*string) )
        string++;

    return string;
}

bool CalcScreen(float *origin,float *vecScreen)
{
	SCREENINFO m_scrinfo;
	m_scrinfo.iSize = sizeof(m_scrinfo);
	gEngine.pfnGetScreenInfo (&m_scrinfo);
	int result = gEngine.pTriAPI->WorldToScreen(origin,vecScreen);
	if(vecScreen[0] < 1 && vecScreen[1] < 1 && vecScreen[0] > -1 && vecScreen[1] > -1 && !result)
	{
	vecScreen[0] = vecScreen[0] * m_scrinfo.iWidth/2 + m_scrinfo.iWidth/2;
	vecScreen[1] = -vecScreen[1] * m_scrinfo.iHeight/2 + m_scrinfo.iHeight/2;
	return true;
	}
	return false;
}

char *NameFromModel(const char *szModel)
{
	static char buf[512], temp[512];
	sprintf(buf, "%s", szModel);
	int i, startpos = 0, endpos = strlen(buf);
	for(i = 0; i < int(strlen(buf)); i++)
	{
		if(buf[i] == '/') startpos = i+1;
		if(buf[i] == '.') {buf[i] = '\0'; break;}
	}
	sprintf(temp, "%s", &buf[startpos]);
	
	return temp;
}

char *NameFromFileName(const char *szFileName)
{
	static char buf[512], temp[512];
	sprintf(buf, "%s", szFileName);
	int i, startpos = 0, endpos = strlen(buf);
	for(i = 0; i < int(strlen(buf)); i++)
	{
		if(buf[i] == '/') startpos = i+1;
		if(buf[i] == '.') {buf[i] = '\0'; break;}
	}
	sprintf(temp, "%s", &buf[startpos]);
	
	return temp;
}


char *NameFromClass(int playerclass)
{
	if(playerclass == 0) return "spectator";
	if(playerclass == 1) return "scout";
	if(playerclass == 2) return "sniper";
	if(playerclass == 3) return "soldier";
	if(playerclass == 4) return "demoman";
	if(playerclass == 5) return "medic";
	if(playerclass == 6) return "hwguy";
	if(playerclass == 7) return "pyro";
	if(playerclass == 8) return "spy";
	if(playerclass == 9) return "engineer";
	if(playerclass == 10) return "random";
	if(playerclass == 11) return "civilian";
	return "unknown";
}

Vector ColorFromTeam(int team)
{
	if(team == 0) return Vector(255, 255, 255);
	if(team == 1) return Vector(64, 64, 255);
	if(team == 2) return Vector(255, 64, 64);
	if(team == 3) return Vector(64, 255, 64);
	if(team == 4) return Vector(64, 255, 255);
	return Vector(255,255,255);
}

void RemoveIllegalsFromFileName(char *szFileName)
{
	char *illegal = ".,/:;[]{}\\\"\'*?~`@|\t\n<>";
	if(strlen(szFileName) > 255) szFileName[255] = '\0';
	for (int i = 0; i < int(strlen(szFileName)); i++)
	{
		for(int j = 0; j < int(strlen(illegal)); j++)
		{
			if((szFileName[i] == illegal[j]) || (szFileName[i] == '\0'))
				szFileName[i] = '-';
		}
	}
}

void RemoveIllegalsFromDemoName(char *szFileName) // Added by Zoltarc
{
	char *illegal = ".,/:;\\\"\'*?~`@|\t\n<>";
	if(strlen(szFileName) > 255) szFileName[255] = '\0';
	for (int i = 0; i < int(strlen(szFileName)); i++)
	{
		for(int j = 0; j < int(strlen(illegal)); j++)
		{
			if((szFileName[i] == illegal[j]) || (szFileName[i] == '\0'))
				szFileName[i] = '_';
		}
	}
}

char *StringSearchReplace(const char *Search, const char *Replace, const char *Input)
{
	static char Output[8192], Temp[8192];
	char *ptr;
	sprintf(Output, "%s", Input);
	while(ptr = strstr(Output, Search))
	{
		*ptr = '\0';
		sprintf(Temp, "%s%s%s", Output, Replace, ptr + strlen(Search));
		sprintf(Output, "%s", Temp);
	}
	return Output;
}



void InitDrawing(void)
{
	g_screeninfo.iSize = sizeof(g_screeninfo);
	pEngine->pfnGetScreenInfo(&g_screeninfo);
}

int DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	// draw the string until we hit the null character or a newline character
	for ( char * p = buf; *p; p++ )
	{
		int next = x + g_screeninfo.charWidths[*p]; // variable-width fonts look cool

		pEngine->pfnDrawCharacter (x, y, *p, r, g, b);
		x = next;
	}

	return x;
}
void econsoleprint(const char *what, bool hud)
{
	char buf[1024];
	char buf2[1024];
	sprintf(buf,"- [e2] -> %s\n",what);
	sprintf(buf2,"%s",what);

	if(!bVerbose) return;

	pEngine->pfnConsolePrint(buf);    

	if(hud)
		WriteHUDChannel(HC_IN_GAME_COLOR, 2, buf2);

}

void FindModID(void)
{
	////add_log("trying to determine mod name... mod directory is \"%s\"", pEngfuncs->pfnGetGameDirectory());
	if(strstr(pEngine->pfnGetGameDirectory(), "cstrike"))
	{
		g_iModID = MODID_CS;
		////add_log("found mod to be CS");
		return;
	}

	if(strstr(pEngine->pfnGetGameDirectory(), "tfc"))
	{
		g_iModID = MODID_TFC;
		////add_log("found mod to be TFC");
		return;
	}

    //add_log("found mod to be UNKNOWN");
	g_iModID = MODID_UNKNOWN;
    
	if(g_iModID == MODID_UNKNOWN)
	{
		MessageBox(NULL, "sorry, but this version of enigma2 can only be used with counter strike and\nteam fortress classic.", "enigma2", MB_OK);
		HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, GetCurrentProcessId());
		if(hProc != NULL)
		{
			TerminateProcess(hProc, 0);
			CloseHandle(hProc);
		}
		else
			ExitProcess(0);

		exit(0);
	}
	
}

bool DrawAt3DPos(float *fPos, float *fOffset, char *szText, float *fColor, int iAlign)
{
	Vector vScreenPos;

	////add_log("DrawAt3DPos started, text: %s", szText);

	if(CalcScreen(fPos, vScreenPos))
	{
		if(iAlign == HUDTEXT_ALIGN_CENTRE)
		{

			vScreenPos[1] -= g_screeninfo.iCharHeight;
			vScreenPos[0] -= CalcStringWidth(szText)/2;
			vScreenPos[0] += fOffset[0];
			vScreenPos[1] -= fOffset[1];

			DrawHudStringSafe(vScreenPos, szText, fColor);

			return true;
		}
		if(iAlign == HUDTEXT_ALIGN_LEFT)
		{

			vScreenPos[1] -= g_screeninfo.iCharHeight;
			vScreenPos[0] += fOffset[0];
			vScreenPos[1] -= fOffset[1];

			DrawHudStringSafe(vScreenPos, szText, fColor);

			return true;
		}
		if(iAlign == HUDTEXT_ALIGN_RIGHT)
		{

			vScreenPos[1] -= g_screeninfo.iCharHeight;
			vScreenPos[0] -= CalcStringWidth(szText);
			vScreenPos[0] += fOffset[0];
			vScreenPos[1] -= fOffset[1];

			DrawHudStringSafe(vScreenPos, szText, fColor);

			return true;
		}
		return false;
	}
	else
		return false;
}

int CalcStringWidth(char *szText)
{
	int i, len;

	int iWidth = 0;

	len = (int)strlen(szText);
	
	for(i = 0; i < len; i++)
	{
		iWidth += g_screeninfo.charWidths[szText[i]];
	}

	return iWidth;
}

void DrawHudStringSafe(float *fPos, char *szText, float *fColor)
{
	int x, y, i, len;
	int r, g, b;
	
	x = (int)fPos[0];
	y = (int)fPos[1];

	////add_log("DrawHudStringSafe at %d, %d called for text: %s", x, y, szText);

	r = (int)(fColor[0]*255);
	g = (int)(fColor[1]*255);
	b = (int)(fColor[2]*255);

	len = (int)strlen(szText);

	for(i = 0; i < len; i++)
	{
		////add_log("trying to draw character '%c' at %d, %d with color %d, %d, %d", szText[i], x, y, r, g, b);
		if(x >= g_screeninfo.iWidth)
			continue;
		//if(x > g_glstate.fLastViewport[VP_X] && x < g_glstate.fLastViewport[VP_W])
		//{
			////add_log("calling pfnDrawCharacter(%d, %d, %c, %d, %d, %d);", x, y, szText[i], r, g, b);
			gEngine.pfnDrawCharacter(x, y, (int)szText[i], r, g, b);
			////add_log("pdnDrawCharacter completed");
		//}
		x += g_screeninfo.charWidths[szText[i]];
	}

	//DrawHudString(x,y,r,g,b,szText);
}

void WriteHUDChannel(int iChannel, float fPeriod, char *WriteText)
{
	if((iChannel < 0) || (iChannel > 63)) return;

	Channel[iChannel].WriteHUDChannel(fPeriod, WriteText);
}

void EqualizeVecs(Vector *begin, Vector *end, float rate)
{
	add_log("1 - x: %f, y: %f, z: %f",begin->x,begin->y,begin->z);


	if(begin->x > end->x)
	{
		begin->x -= rate;
	}
	else if(begin->x < end->x)
	{
		begin->x += rate;
	}

//	gEngine.SetViewAngles(&begin);

	if(begin->y > end->y)
	{
		begin->y -= rate;
	}
	else if(begin->y < end->y)
	{
		begin->y += rate;
	}

//	gEngine.SetViewAngles(&begin);

	add_log("2 - x: %f, y: %f, z: %f",begin->x,begin->y,begin->z);
}