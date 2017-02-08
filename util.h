#ifndef UTIL_H
#define UTIL_H

#define HOOK_COMMAND_2(x, y) 		gEngine.pfnAddCommand (x, __CmdFunc_##y );
#define DECLARE_COMMAND_2(y, x) 	static void __CmdFunc_##x( void )	\
								{ 									\
									##y.Cmd_##x( ); 			\
								}

#define MODID_UNKNOWN		0
#define MODID_CS			1
#define MODID_TFC			2

#define MODID_NOTCHECKED	-1

#define HUDTEXT_ALIGN_LEFT			0
#define HUDTEXT_ALIGN_RIGHT			1
#define HUDTEXT_ALIGN_CENTRE		2

extern int g_iModID;

const char * dump (void * buf, int size);
bool 		str_prefix (const char * substr, const char * str);
const char * get_argument (const char *string, char *buf, size_t bufsize);

bool CalcScreen(float *origin,float *vecScreen);
extern bool g_bEngineLoaded;
void FindModID (void);

char *NameFromModel(const char *szModel);
char *NameFromFileName(const char *szFileName);
char *NameFromClass(int playerclass);
void RemoveIllegalsFromFileName(char *szFileName);
void RemoveIllegalsFromDemoName(char *szFileName); // Added by Zoltarc
char *StringSearchReplace(const char *Search, const char *Replace, const char *Input);

bool DrawAt3DPos(float *fPos, float *fOffset, char *szText, float *fColor, int iAlign);
int CalcStringWidth(char *szText);

extern FILE *g_fpLog;

void __cdecl add_log (const char *fmt, ...);
void __cdecl add_log_noformat(const char *buf);
void __cdecl setup_logging(void);
void __cdecl end_logging(void);
extern SCREENINFO g_screeninfo;
void InitDrawing(void);
int DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... );
void DrawHudStringSafe(float *fPos, char *szText, float *fColor);
void econsoleprint(const char *what, bool hud);
void WriteHUDChannel(int iChannel, float fPeriod, char *WriteText);
void EqualizeVecs(Vector *begin, Vector *end, float rate);
void GetExportFunctions();
extern int bVerbose;
extern int bHackOn;

#pragma comment (lib,"winmm.lib")

class CTimer {
public:
	CTimer( void )	{ dwStop = 0; }
	~CTimer( void )	 { }
	void Start( float fSec ) { dwStop = (DWORD)(fSec*1000) + timeGetTime(); }
	void Stop( void ) { dwStop = 0; }
	float TimeLeft( void ) { if( Running() ) return ( ((float)(dwStop - timeGetTime())/1000) ); return 0; }
	bool Running( void ) { return ( dwStop > timeGetTime() ); }
protected:
	DWORD dwStop;
};

extern HANDLE hMapFile;
extern LPCTSTR pBuf;
#endif