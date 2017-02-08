#include <windows.h>

// credits to el domo
bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask )
			return false;
	return (*szMask) == NULL;
}

// credits to el domo
DWORD dwFindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask)
{
	for(DWORD i=0; i < dwLen; i++)
		if( bDataCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
			return (DWORD)(dwAddress+i);

	return 0;
}

DWORD dwDetour( DWORD dwOrgFunc, DWORD dwNewFunc )
{
	// function removed to not be forced to GPL this base
	// re-add a detour function to get the base VAC2 Proof
	return dwOrgFunc;
}