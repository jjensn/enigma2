#include <Windows.h>
#include "client_headers.h"
#include "client.h"
#include "mempatcher.h"
#include "opengl.h"

#define BUF_SIZE 256
TCHAR szName[]=TEXT("helloacs");

char buff2[45];

HANDLE hMapFile;
LPCTSTR pBuf;

bool bQPC = false;

DWORD g_dwBasePointer = 0x01D0DC74; // just an orientation, tampered below

CreateThread_t pCreateThread;

DETOUR_TRAMPOLINE(BOOL WINAPI tr_QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount), QueryPerformanceCounter);
DETOUR_TRAMPOLINE(BOOL WINAPI tr_OutputDebugStringA( const char *pszString ),OutputDebugStringA);

DWORD		dwNewEAT[35]	= { NULL }; // set your address by index at this table

// offsets in the EAT for each function original credit goes to organner but it was fucked up so I redid it
DWORD  pLinkerTable[37] = { 0x00, 0x23, 0x47, 0x6B, 0x8E, 0xB2, 0xD6, 0xF9, 0x11D, 0x141, //0..9
                            0x164, 0x188, 0x1AC, 0x1CF, 0x1F3, 0x217, 0x23A, 0x25E, 0x282, 0x2A5, //10..19
                            0x2C9, 0x2ED, 0x310, 0x334, 0x358, 0x37B, 0x39F, 0x3C3, 0x3E6, 0x40A, //20..29
                            0x42E, 0x451, 0x475, 0x499, 0x4BC, 0x4E0, 0x504  }; //30..36

DWORD dwSetImportbyIndex( int Index, DWORD dwHook ) // make this inline ?
{
	PBYTE pbAddress = ( PBYTE ) ( g_dwBasePointer + pLinkerTable[ Index ] );

	if( *pbAddress != 0xA3 )	// current offset wrong :(
		return NULL;

	pbAddress++;

	PDWORD pdwCurrentExport = ( PDWORD ) pbAddress;
	PDWORD pdwReturn = ( PDWORD ) *pdwCurrentExport;
	DWORD dwReturn = ( DWORD ) *pdwReturn;

	if ( dwHook ) { // only apply the hook if one was set
		*pdwReturn = dwHook;
		add_log("[success] clientfunction (%i) hooked properly",Index); }

	return dwReturn;
}

DWORD dwThread( LPVOID lpArgs )
{
	pEngine		= ( cl_enginefunc_t* )	0x1EA0A08;

	Sleep(10000);

	// uncomment to debug clientfunc hooking
	// GetExportFunctions();

	SetupOpenGLHooks();

	if( pClient == NULL || g_dwBasePointer == NULL )
		return 0; // something went terribly wrong

	// find the EAT base
	DWORD dwEATBase = dwFindPattern( 0x01D172C0, 0x000645A4,(BYTE*)"\x56\x8B\x74\x24\x08\x56\xE8\x00\x00\x00\x00\x83\xC4\x04\xA3\x00\x00\x00\x00\x85\xC0\x75\x13\x56\x68","xxxxxxx????xxxx????xxxxxx");

	// sig outdated :/
	if ( dwEATBase == 0 ) { 
		add_log("[fail] export table address is outdated");
		return 1; 
	}

	// fix the sig (basepointer is always where Init is mov'd)
	g_dwBasePointer = dwEATBase + 0x44;

	// prepare our struct with the original functions for copying
	PDWORD pdwClientEAT = ( PDWORD ) pClient;

	// apply the hooks
	SetupClientHooks();

	// copy the functions and set up the hooks
	for( int i = 0;i < 35; i++ )
		pdwClientEAT[ i ] = dwSetImportbyIndex( i, dwNewEAT[ i ] );

	FindModID();
	gBunnyhop.Pre_HUD_Init();
	gSpeedo.Pre_HUD_Init();
	gAutorecord.Pre_HUD_Init();
	gAnticonc.Pre_HUD_Init();
	gESP.Pre_HUD_Init();
	gRTFM.Pre_HUD_Init();
	gMenu.Pre_HUD_Init();
	gAutodet.Pre_HUD_Init();
	gAimbot.Pre_HUD_Init();
	gNoSmoke.Pre_HUD_Init();
	gFlagStatus.Pre_HUD_Init();

	econsoleprint("enigma2",false);
	econsoleprint("dajuda!",false);
	econsoleprint("< - - - - - - - -",false);
	econsoleprint("released under gpl -- if you did not",false);
	econsoleprint("receive a copy of the source code with this",false);
	econsoleprint("binary, you have a right to request it from the distributor.",false);
	econsoleprint("< - - - - - - - -",false);
}

BOOL WINAPI dt_QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount)
{
	BOOL ret;

    // QPC is only called once the engine is loaded, so we can initialize engine-specific things here
	if(!bQPC)
	{
		// poor test of whether engfuncs are initialized
		if(pEngine->Con_Printf != NULL && pEngstudio->SetupPlayerModel != NULL)
		{
			bQPC = true;
			SetupEngineHooks();

			InitStudioHooks();
			InitSprites();

			hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   szName);               // name of mapping object
 
			if (hMapFile == NULL) 
			{ 
				ret = tr_QueryPerformanceCounter(lpPerformanceCount);
				return ret;
			}

			pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
							FILE_MAP_ALL_ACCESS, // read/write permission
							0,                   
							0,                   
							BUF_SIZE);       

			if (!pBuf || pBuf == NULL) 
			{ 
				ret = tr_QueryPerformanceCounter(lpPerformanceCount);
				return ret;
			}

			sprintf(buff2,"%s",CHEAT_ENABLED);
			CopyMemory((PVOID)pBuf, buff2, strlen(buff2));
		}
	}

	ret = tr_QueryPerformanceCounter(lpPerformanceCount);

	return ret;
}

void WINAPI dt_OutputDebugStringA(const char *pszString)
{
		// add_log(pszString);
		tr_OutputDebugStringA(pszString);
}

HANDLE WINAPI hook_CreateThread ( IN LPSECURITY_ATTRIBUTES lpThreadAttributes, IN SIZE_T dwStackSize, IN LPTHREAD_START_ROUTINE lpStartAddress, IN LPVOID lpParameter, IN DWORD dwCreationFlags, OUT LPDWORD lpThreadId )
{
	if ( ( DWORD ) lpStartAddress > 0x10001000 && ( DWORD ) lpStartAddress < 0x1000AFFE /* affe lol */ )
		return INVALID_HANDLE_VALUE; // don't allow VAC2 to create any threads

	return pCreateThread( lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId );
}

int WINAPI DllMain( HANDLE hmModule, DWORD dwReason, LPVOID lpUseless )
{
	switch( dwReason )
	{
	    case DLL_PROCESS_ATTACH:
			setup_logging();
		//	create the hooking thread
			CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) dwThread, NULL, 0, new DWORD );
			pCreateThread = (CreateThread_t) dwDetour((DWORD) CreateThread, (DWORD) hook_CreateThread );
			DetourFunctionWithTrampoline((PBYTE)tr_QueryPerformanceCounter, (PBYTE)dt_QueryPerformanceCounter);
			DetourFunctionWithTrampoline((PBYTE)tr_OutputDebugStringA,(PBYTE)dt_OutputDebugStringA);
	        break;

		case DLL_PROCESS_DETACH:

			if(pBuf) 
			{
				sprintf(buff2,"%s",CHEAT_QUIT);
				CopyMemory((PVOID)pBuf, buff2, strlen(buff2));
				UnmapViewOfFile(pBuf);
				CloseHandle(hMapFile);
			}
			end_logging();
			break;
	}

	return 1;
}