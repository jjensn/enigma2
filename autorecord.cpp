#include "client_headers.h"
#include "autorecord.h"

CAutorecord gAutorecord;

DECLARE_COMMAND_2(gAutorecord, AutoRecord);
DECLARE_COMMAND_2(gAutorecord, EasyRecord);

static char DemoName [2048];
static char DemoRecord [2048];
static char PlayerName [256];

bool CAutorecord::Pre_HUD_Init (void)
{
	HOOK_COMMAND_2("e_autorecord", AutoRecord);
	HOOK_COMMAND_2("e_easyrecord", EasyRecord);
	return true;
}

void CAutorecord::Post_Hook_TextMsg (const char *pszString)
{
	if((!strcmp(pszString, "#Match_results") && bAutoRecord))
	{
		bRecordOK = 0;
		gEngine.pfnClientCmd("stop");
		bAutoRecordFix = false;
	}

	if((!strcmp(pszString, "#Match_results") && bMatchRecord))
	{
		bRecordOK = 0;
		gEngine.pfnClientCmd("stop");
	}

	if((!strcmp(pszString, "#Game_thirtysecleft") && bMatchRecord))
	{
		bRecordOK = 1;
		Cmd_EasyRecord();
	}

	if(!strcmp(pszString, "#Game_joinedteam"))
	{
		bAutoRecordOK = 1;

		if ( !bAutoRecordFix )
		{
			if(bAutoRecord)
			{
				Cmd_EasyRecord();
				bAutoRecordFix = true;
			}
		}
	}

}

bool CAutorecord::Pre_Hook_ConsoleCommand (char *pszString)
{
	if((!strcmp(pszString, "reconnect")) || (!strcmp(pszString, "retry")) || (!strcmp(pszString, "quit")))
	{
		gEngine.pfnClientCmd("stop");
		bVerbose = 1;
	}

	if((!strcmp(pszString, "stop")))
		bVerbose = 1;


	return true;
}

void CAutorecord::Cmd_EasyRecord(void)
{
	/* Setup the time */
	SYSTEMTIME st;
	GetLocalTime ( &st);

	if(bRecordOK || bAutoRecordOK) // FB's really (really!) sloppy hack to check whether we can record
	{
		char MapName	[2048];			// Holds the map name

		/* Setup the MapName and PlayerName */
		strcpy(MapName, NameFromFileName(gEngine.pfnGetLevelName()));		
		strcpy(PlayerName, gEngine.pfnGetCvarString("name"));
		RemoveIllegalsFromDemoName(PlayerName);

		/* Adds the date string and map name onto the record command */
		sprintf(DemoRecord, "record \"%s-%d-%d-%d-%s\"", PlayerName, st.wDay, st.wMonth, st.wYear, MapName);
		sprintf(DemoName, "%s-%d-%d-%d-%s", PlayerName, st.wDay, st.wMonth, st.wYear, MapName);

		IncreaseDemoName();

		char buf[2064];
		sprintf(buf, "autorecord: initiating recoring (%s)", DemoName);
		econsoleprint(buf,false);
	}
	else
		econsoleprint("delaying the recording until connected",false);

}

void CAutorecord::IncreaseDemoName(void)
{
	/* Based off the demo code found in FuhQuake
	 * Got buffer? */
	char name2 [2048];
	char name3 [2048];
	char NameHack [2048];
	char NewRecord [2048];
	FILE * file;
	int i;

	sprintf(name2, "%s\\%s.dem", gEngine.pfnGetGameDirectory(), DemoName);

	file = fopen (name2, "rb");
	if (file) 
	{
		i = 0;
		do {
			fclose (file);
			/* Copy the current demo name, and the extention string to a buffer */
			sprintf(name3, "%s-%d.dem", DemoName, i);

			/* This is needed to stop zero byte files outside of the tfc directory */
			sprintf(NameHack, "%s\\%s-%d.dem", gEngine.pfnGetGameDirectory(), DemoName, i);
			file = fopen (NameHack, "rb");
			i++;			
		} while (file);
	}
	fopen (NameHack, "wb");
	if (!NameHack) 
	{
		econsoleprint("error: couldn't open",false);
		return;
	}
	
	sprintf(NewRecord, "record \"%s\"", name3);

	file = fopen (name2, "rb");

	bVerbose=0; // no console printing

	if (file)
	{
		fclose (file);
		gEngine.pfnClientCmd(NewRecord); // Record increased demo
	}
	else
	{
		gEngine.pfnClientCmd(DemoRecord); // Record first demo
	}
}

void CAutorecord::Cmd_AutoRecord(void)
{
	int setting;
	if(gEngine.Cmd_Argc() > 1)
	{
		setting = atoi(gEngine.Cmd_Argv(1));
		if((setting >= 0) && (setting <= 1))
		{
			bAutoRecord = (bool)setting;
			if(setting == 0) econsoleprint("autorecording enabled",false);
			if(setting == 1) econsoleprint("autorecording disabled",false); 
		}
	}
	if (bAutoRecord)
	{
		Cmd_EasyRecord();
	}

	if (!bAutoRecord)
	{
		gEngine.pfnClientCmd("stop");
	}

	if(!strcmp(gEngine.Cmd_Argv(1), "match"))
	{
		bMatchRecord = bMatchRecord ? 0 : 1;
		if(bMatchRecord)
		{
			econsoleprint("match record enabled",false);
		}
		else
		{
			econsoleprint("match record disabled",false);
			gEngine.pfnClientCmd("stop");
		}
	}	
}

CAutorecord::CAutorecord(void)
{
	bAutoRecord = 0;
	bMatchRecord = 0;
	bRecordOK = 0;
	bAutoRecordFix = false;
}