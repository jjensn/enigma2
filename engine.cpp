#include "client_headers.h"
#include "engine.h"

cl_enginefunc_t*		pEngine =	NULL;
cl_enginefunc_t			gEngine = { NULL };
engine_studio_api_s		oEngstudio;
engine_studio_api_s		*pEngstudio = (engine_studio_api_s*)0x1EBC978;

bool bInit			 = false;

void nop_smoke(event_args_t *args)
{
	if(gNoSmoke.IsActive)
		return;
	else
		gEngine.pfnHookEvent;
}

void beh()
{
	pEngine->pfnConsolePrint("behh\n");
}

void SetupEngineHooks(void)
{
	memcpy(&gEngine, pEngine, sizeof(cl_enginefunc_t));
	memcpy(&oEngstudio,pEngstudio, sizeof(engine_studio_api_t));

// apply your hooks here
//	pEngine->example = example;

	pEngine->pfnClientCmd = &my_ClientCmd;
	pEngine->pfnConsolePrint = &ConPrint;
	pEngine->pfnDrawConsoleString = &DrawConsoleString;

	gFlagStatus.x1 = 0;
	gFlagStatus.x2 = 0;

}

int __cdecl my_ClientCmd(char *szCmdString)
{
	gAutorecord.Pre_Hook_ConsoleCommand(szCmdString);

	// to-do : cleanup/rename bleh and cleanup flagstatus code.

	if(strstr(szCmdString,"flaginfo"))
	{
		if(gFlagStatus.x1 == 0)
			gFlagStatus.fEnemyTimer = gEngine.GetClientTime();

		if(gFlagStatus.x2 == 0)
			gFlagStatus.fMyTimer = gEngine.GetClientTime(); 

	}

	if(strstr(szCmdString,"creensho"))
	{
		pEngine->pfnConsolePrint("screenshot2\n");
		bHackOn = 0;
	}

	return gEngine.pfnClientCmd(szCmdString);
}

void __cdecl ConPrint(const char *string)
{
	if(gFlagStatus.IsActive)
	{

		if(!strstr(string, "flag"))
			gEngine.pfnConsolePrint(string);

		return;
	}

	//gFlagStatus.Pre_Hook_ConPrint(string);

	//gEngine.pfnClientCmd("flaginfo");

	gEngine.pfnConsolePrint(string);
}

int	_cdecl DrawConsoleString(int x, int y, char *string)
{

	if(strstr(string,"flag"))
	{
		gFlagStatus.Pre_Hook_ConPrint(string);		
	}
	else
		return gEngine.pfnDrawConsoleString(x,y,string);

	if(!gFlagStatus.IsActive)
		return gEngine.pfnDrawConsoleString(x,y,string);
}
