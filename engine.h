#ifndef HOOKFUNCS_H
#define HOOKFUNCS_H

int		_cdecl DrawConsoleString(int x, int y, char *string);
//int		_cdecl hookHookUserMsg	(char *szMsgName, pfnUserMsgHook pfn);
//void	_cdecl hookSPR_Set		(HSPRITE hPic, int r, int g, int b );
//HSPRITE _cdecl SPR_Load			(const char *szPicName);
//void	_cdecl SPR_Draw			(int frame, int x, int y, const wrect_t *prc);
//void	_cdecl SPR_DrawHoles	(int frame, int x, int y, const wrect_t *prc);
//void	_cdecl SPR_DrawAdditive	(int frame, int x, int y, const wrect_t *prc);
int	_cdecl	my_ClientCmd		(char *szCmdString);
void nop_smoke(event_args_t *args);
void beh();
//int		AddCommand		(char *cmd_name, void (*function)(void));
//int		_cdecl ServerCmd		(char *szCmdString);
//int		_cdecl ServerCmd2		(char *szCmdString);
//void	_cdecl ConDPrintf		(char *fmt, ...);
//void	_cdecl ConNPrintf		(int pos, char *fmt, ...);
//void	_cdecl ConPrintf		(char *fmt, ...);
//void	_cdecl ConNXPrintf		(struct con_nprint_s *info, char *fmt, ...);
void	_cdecl ConPrint			(const char *string);

#endif HOOKFUNCS_H