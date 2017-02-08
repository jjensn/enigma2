#include "client_headers.h"
#include "speedo.h"

CSpeedo gSpeedo;

DECLARE_COMMAND_2(gSpeedo, ToggleSpeedo);

bool CBunnyhop::Pre_HUD_Init	(void)
{
    HOOK_COMMAND_2("e_speedo",ToggleSpeedo);
	return true;
}

void CSpeedo::Cmd_ToggleSpeedo		(void)
{
	
}