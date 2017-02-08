#include "client_headers.h"
#include "mempatcher.h"

typedef struct usermsg_s
{
int number;
int size;
char name[16];
struct usermsg_s* next;
pfnUserMsgHook pfn;
} usermsg_t;

usermsg_t** gUserMsgList = NULL;

bool bWasInAimbot;

pfnUserMsgHook pConcuss;
pfnUserMsgHook pTextMsg;
pfnUserMsgHook pHudText;
pfnUserMsgHook pResetHUD;

static int Concuss( const char *pszName, int iSize, void *pBuffer )
{
	int conc;
	BEGIN_READ( pBuffer, iSize );
	conc = READ_BYTE();
	
	gAnticonc.iConcAmt = conc;	//just store the conc cache. just incase we need it later.

	if(gAnticonc.iConcAmt > 0){
		gAnticonc.bInConc=true;
		if(gAimbot.bAimbot)
		{
			gAimbot.bAimbot = false;
			bWasInAimbot = true;

		}
	}
	else {
		gAnticonc.bInConc=false; 
		if(bWasInAimbot)
		{
			bWasInAimbot = false;
			gAimbot.bAimbot = true;
		}
	}

	if(gAnticonc.bNoConc)
		return 0;
	else
        return pConcuss( pszName, iSize, pBuffer );
}
static int TextMsg( const char *pszName, int iSize, void *pBuffer )
{
	BEGIN_READ (pBuffer, iSize);
    int id = READ_BYTE();
	int retval = 0;
    const char * str1 = READ_STRING();

	gAutorecord.Post_Hook_TextMsg(str1);

	gFlagStatus.Post_Hook_TxtMsg(str1);

	//add_log("txt: %s",str1);

	//pEngine->pfnClientCmd("flaginfo");

	return pTextMsg( pszName, iSize, pBuffer );
}

static int HudText( const char *pszName, int iSize, void *pBuffer )
{
	BEGIN_READ (pBuffer, iSize);
    int id = READ_BYTE();
	int retval = 0;
    const char * str1 = READ_STRING();

	//add_log("hud: %s",str1);

	gFlagStatus.Post_Hook_HudText(str1);

	//pEngine->pfnClientCmd("flaginfo");

	return pHudText( pszName, iSize, pBuffer );
}

static int ResetHUD 	(const char *pszName, int iSize, void *pBuffer)
{
	if(gFlagStatus.IsActive) {
		pEngine->pfnClientCmd("flaginfo"); }

	return pResetHUD (pszName, iSize, pBuffer);
}

/*
static int InitHUD  ( const char *pszName, int iSize, void *pBuffer )
static int ScoreInfo( const char *pszName, int iSize, void *pBuffer )
static int TeamNames( const char *pszName, int iSize, void *pBuffer )
static int TeamScore( const char *pszName, int iSize, void *pBuffer )
static int CurWeapon( const char *pszName, int iSize, void *pBuffer )
static int TeamInfo ( const char *pszName, int iSize, void *pBuffer )
static int Spectator( const char *pszName, int iSize, void *pBuffer )
static int AmmoX( const char *pszName, int iSize, void *pBuffer )
static int Damage( const char *pszName, int iSize, void *pBuffer )
static int ScoreAttrib( const char *pszName, int iSize, void *pBuffer )
static int DeathMsg( const char *pszName, int iSize, void *pBuffer )
static int Battery( const char *pszName, int iSize, void *pBuffer )
static int TextMsg( const char *pszName, int iSize, void *pBuffer )
static int WeaponList( const char *pszName, int iSize, void *pBuffer )
static int Money( const char *pszName, int iSize, void *pBuffer )
static int ViewMode( const char *pszName, int iSize, void *pBuffer )
static int SpecHealth( const char *pszName, int iSize, void *pBuffer )
static int Concuss( const char *pszName, int iSize, void *pBuffer )
static int ReloadSound( const char *pszName, int iSize, void *pBuffer )
static int BombDrop( const char *pszName, int iSize, void *pBuffer )
static int NVGToggle( const char *pszName, int iSize, void *pBuffer )
static int BombPickup( const char *pszName, int iSize, void *pBuffer )
static int HostagePos( const char *pszName, int iSize, void *pBuffer )
static int MOTD( const char *pszName, int iSize, void *pBuffer )
static int BuildSt( const char *pszName, int iSize, void *pBuffer )
static int ServerName( const char *pszName, int iSize, void *pBuffer )
static int HideWeapon( const char *pszName, int iSize, void *pBuffer )
static int TutorLine( const char *pszName, int iSize, void *pBuffer )
static int HLTV( const char *pszName, int iSize, void *pBuffer )
static int TutorState( const char *pszName, int iSize, void *pBuffer )
static int ItemStatus( const char *pszName, int iSize, void *pBuffer )
static int VGUIMenu( const char *pszName, int iSize, void *pBuffer )
static int ItemPickup( const char *pszName, int iSize, void *pBuffer )
static int ADStop( const char *pszName, int iSize, void *pBuffer )
static int AllowSpec( const char *pszName, int iSize, void *pBuffer )
static int ForceCam( const char *pszName, int iSize, void *pBuffer )
static int GameMode( const char *pszName, int iSize, void *pBuffer )
static int Crosshair( const char *pszName, int iSize, void *pBuffer )
static int Health( const char *pszName, int iSize, void *pBuffer )
static int Radar( const char *pszName, int iSize, void *pBuffer )
static int SayText( const char *pszName, int iSize, void *pBuffer )
static int AmmoPickup( const char *pszName, int iSize, void *pBuffer )
static int TutorClose( const char *pszName, int iSize, void *pBuffer )
static int BuyClose( const char *pszName, int iSize, void *pBuffer )
static int SendAudio( const char *pszName, int iSize, void *pBuffer )
static int ArmorType( const char *pszName, int iSize, void *pBuffer )
static int Flashlight( const char *pszName, int iSize, void *pBuffer )
static int WeapPickup( const char *pszName, int iSize, void *pBuffer )
static int GameTitle( const char *pszName, int iSize, void *pBuffer )
static int StatusText( const char *pszName, int iSize, void *pBuffer )
static int SecAmmoVal( const char *pszName, int iSize, void *pBuffer )
static int SecAmmoIcon( const char *pszName, int iSize, void *pBuffer )
static int RoundTime( const char *pszName, int iSize, void *pBuffer )
static int ShowMenu( const char *pszName, int iSize, void *pBuffer )
static int VoiceMask( const char *pszName, int iSize, void *pBuffer )
static int FlashBat( const char *pszName, int iSize, void *pBuffer )
static int HudText( const char *pszName, int iSize, void *pBuffer )
static int ReqState( const char *pszName, int iSize, void *pBuffer )
static int StatusValue( const char *pszName, int iSize, void *pBuffer )
*/


//-------------------------------------------------------------------------------------
pfnUserMsgHook InterceptUserMsg( const char* pszName, pfnUserMsgHook pMsgHook )
{
	
	if ( gUserMsgList == NULL )
		return NULL;

	usermsg_t* msg = *gUserMsgList;

	pfnUserMsgHook pReturn = NULL;

	while ( msg )
	{
		if( strcmp( msg->name, pszName ) == 0 )
		{
			pReturn = msg->pfn;
			msg->pfn  = pMsgHook;
			break;
		}

		msg = msg->next;
	}

	return pReturn;
}

void SetupUserMessageHooks(void)
{
	static bool bApplied = false;

	if (bApplied == false)
	{
		//add_log("usr msgs v2 started...");
		bApplied = true;
		DWORD dwUsrDispatch		 = dwFindPattern( 0x01901000, 0x000645a4,(BYTE*)"\x53\x8B\x5C\x24\x0C\x55\x8B\x6C\x24\x0C\x56\x8B\x35\xFF\xFF\xFF\xFF","xxxxxxxxxxxxx????");
		
		if ( dwUsrDispatch ) // 1.5 and mods
		{
			gUserMsgList	= (usermsg_t**)			 (*(DWORD*)(dwUsrDispatch+2));
		}
		else	// 1.6 and DOD
		{
			// Credits: LanceVorgin
			DWORD	dwAddress = NULL;

			dwAddress = (DWORD) pEngine->pfnHookUserMsg + ( 0x1d0d15b -  0x1d0d140 );
			dwAddress += *(int*)( dwAddress ) + 4;
			dwAddress += (0x1d1d36d - 0x1d1d360);
			gUserMsgList	= (usermsg_t**)			 (*(DWORD*)( dwAddress ));
		}

		// ------------------------------------------------------------------------------------->

		InitDrawing();

		// <------------------------------------------------------------------------------------
		

		pConcuss = InterceptUserMsg( "Concuss", Concuss );
		pTextMsg = InterceptUserMsg( "TextMsg", TextMsg );
		pHudText = InterceptUserMsg( "HudText", HudText );
		pResetHUD = InterceptUserMsg( "ResetHUD", ResetHUD);

		//add_log("usr msgs v2 ended");
	}
}