/*  ------------------------------------------------------------------------
	  retry.cpp by Mulchman
		- gets console text
		- retry script
	------------------------------------------------------------------------ */

#include "client_headers.h"

/*Global*/
CRetry gRetry;

DECLARE_COMMAND_2(gRetry, ToggleRetry);

bool CRetry::Pre_HUD_Init (void)
{
	HOOK_COMMAND_2("e_retry", ToggleRetry);

	return true;
}

CRetry::CRetry (void)
{
	bRetryOn = 0;
}

void CRetry::Cmd_ToggleRetry (void)
{
	bRetryOn = bRetryOn ? 0 : 1;

	if (bRetryOn)
		econsoleprint("server bust enabled");
	else
		econsoleprint("server bust disabled");
}

void CRetry::Post_Hook_ConsoleOutput (const char *pszString)
{
	if (bRetryOn)
	{
		if ((strstr(pszString, "Connection failed after 4 retries")) ||
			(strstr(pszString, "Server is full")) ||
			(strstr(pszString, "Host_EndGame: Server disconnected")) ||
			(strstr(pszString, "There are no reserved slots available on the server")) ||
			(strstr(pszString, "You have been kicked from the server")) ||
			(strstr(pszString, "kicked by console")) ||
			(strstr(pszString, "Kicked by Console")) ||
			(strstr(pszString, "Connection to server timed out")) ||
			(strstr(pszString, "Invalid authentication information"))
			)
		{
			gEngine.pfnClientCmd("retry;");
		}
	}
}

