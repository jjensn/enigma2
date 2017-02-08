/*  ------------------------------------------------------------------------
	  consoleoutput.h
		- by Mulchman
	------------------------------------------------------------------------ */

#ifndef RETRY_H
#define RETRY_H

class CRetry
{
	public:

	virtual bool Pre_HUD_Init (void);

	CRetry (void);

	virtual void Post_Hook_ConsoleOutput (const char *pszString);

	void Cmd_ToggleRetry (void);

	bool bRetryOn;
};

extern CRetry gRetry;

#endif