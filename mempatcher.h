#ifndef _MEMPATCHER_H_
#define _MEMPATCHER_H_
#include <Windows.h>

typedef HANDLE (WINAPI* CreateThread_t)	( IN LPSECURITY_ATTRIBUTES lpThreadAttributes, IN SIZE_T dwStackSize, IN LPTHREAD_START_ROUTINE lpStartAddress, IN LPVOID lpParameter, IN DWORD dwCreationFlags, OUT LPDWORD lpThreadId );
BOOL WINAPI dt_QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount);
// void WINAPI dt_OutputDebugStringA(const char *pszString);

DWORD dwFindPattern( DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask );
DWORD dwDetour( DWORD dwOrgFunc, DWORD dwNewFunc );


#endif