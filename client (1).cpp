#include "client_headers.h"

ExportTable_t*		pClient = new ExportTable_t; // storage for the original functions we hook

bool g_bEngineLoaded = false;
int bFirst = false;

int bHackOn = 1;
int iPod = 0;
int z = 0;
char buf[45];

//--Primarily unused client functions
int Hooked_Initialize( cl_enginefunc_t *pEngine, int iVersion ) 
{ 
	////add_log("WHAT");
    return pClient->Initialize( pEngine, iVersion ); 
} 

int Hooked_HUD_Redraw(float a, int b)
{
	int post;

	SetupUserMessageHooks();

	if(bHackOn == 1)
	{
		if(iPod >= 30)
		{
			post = pClient->HUD_Redraw(a,b);
			gESP.Pre_HUD_Redraw(a,b);
			gSpeedo.Pre_HUD_Redraw(a,b);
			gAnticonc.Pre_HUD_Redraw(a,b);
			gRTFM.Post_HUD_Redraw(a,b);
			gMenu.Post_HUD_Redraw(a,b);
			Fancy_HUD_Redraw(a,b);
			gAutodet.Pre_HUD_Redraw(a,b);
			gFlagStatus.Pre_HUD_Redraw(a,b);
		}
		else
		{
			iPod++;
			post = pClient->HUD_Redraw(a,b);
		}
		return post;
	}
	else
	{
		post = pClient->HUD_Redraw(a,b);
		//z++;

		/*if(z >= 30)
		{
			sprintf(buf,"%s",CHEAT_DISABLED);
			CopyMemory((PVOID)pBuf, buf, strlen(buf));
			z = 0;
			iPod = 0;
		}*/

		return post;		
	}
}

void Hooked_HUD_PlayerMove( struct playermove_s *pPmove, int server)
{
	 pClient->HUD_PlayerMove(pPmove, server);
	 gPlayerinfo.Post_HUD_PlayerMove(pPmove, server);
}

int Hooked_HUD_AddEntity(int type, struct cl_entity_s *ent, const char* modelname)
{
	//gAutodet.Post_HUD_AddEntity(type,ent,modelname);
	//gNoSmoke.Post_HUD_AddEntity(type,ent,modelname);

	gESP.Pre_CL_CreateMove();

	////add_log("type: %i, name: %s, other: %s",type,modelname,ent->model->name);
	return pClient->HUD_AddEntity(type,ent,modelname);	
}


void Hooked_V_CalcRefdef( struct ref_params_s *pparams )
{
	pClient->V_CalcRefdef(pparams);
}

void Hooked_CL_CreateMove(float frametime, struct usercmd_s *cmd, int active)
{  
		pClient->CL_CreateMove(frametime, cmd, active);

		gAimbot.DoAimbot(cmd);

//		gESP.Pre_CL_CreateMove(frametime,cmd,active); //edit

		gBunnyhop.Pre_CL_CreateMove(frametime,cmd,active);
			
}

void Hooked_HUD_Key_Event( int eventcode, int keynum, const char *pszCurrentBinding )
{
	pClient->HUD_Key_Event(eventcode,keynum, pszCurrentBinding);
	//gMenu.Pre_HUD_Key_Event(eventcode,keynum,pszCurrentBinding);
}

void Hooked_HUD_PostRunCmd( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed)
{
	pClient->HUD_PostRunCmd(from,to,cmd,runfuncs,time,random_seed);
}




//--Primarily unused client functions
int Hooked_HUD_Init(void)
{
	return pClient->HUD_Init();
}

void Hooked_HUD_PlayerMoveInit ( struct playermove_s * move)
{
	pClient->HUD_PlayerMoveInit(move);
}

char Hooked_HUD_PlayerMoveTexture ( char *pTex )
{
	return pClient->HUD_PlayerMoveTexture(pTex);
}

void Hooked_IN_MouseEvent( int mstate )
{
	pClient->IN_MouseEvent( mstate );
}
void Hooked_IN_Accumulate( void )
{
	pClient->IN_Accumulate( );
}

void Hooked_IN_ActivateMouse( void )
{
	pClient->IN_ActivateMouse( );
}

void Hooked_IN_ClearStates( void )
{
	pClient->IN_ClearStates( );
}

void Hooked_IN_DeactivateMouse( void )
{
	pClient->IN_DeactivateMouse( );
}

void Hooked_CL_CameraOffset( float *ofs )
{
	pClient->CL_CameraOffset( ofs );
}

int Hooked_CL_IsThirdPerson( void )
{
	return pClient->CL_IsThirdPerson( );
}

void Hooked_Demo_ReadBuffer( int size, unsigned char *buffer )
{
	pClient->Demo_ReadBuffer( size, buffer );
}

int Hooked_HUD_ConnectionlessPacket( struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
	return pClient->HUD_ConnectionlessPacket( net_from, args, response_buffer, response_buffer_size );
}

void Hooked_HUD_CreateEntities( void )
{
	pClient->HUD_CreateEntities( );
}


void Hooked_HUD_DrawNormalTriangles( void )
{
	pClient->HUD_DrawNormalTriangles( );
}

void Hooked_HUD_DrawTransparentTriangles( void )
{
	pClient->HUD_DrawTransparentTriangles( );
}

void Hooked_HUD_Frame( double time )
{
	pClient->HUD_Frame( time );
}

int Hooked_HUD_GetHullBounds( int hullnumber, float *mins, float *maxs )
{

	return pClient->HUD_GetHullBounds( hullnumber, mins, maxs );
}

struct cl_entity_s *Hooked_HUD_GetUserEntity( int index )
{
	return pClient->HUD_GetUserEntity( index );	
}

void Hooked_HUD_ProcessPlayerState( struct entity_state_s *dst, const struct entity_state_s *src )
{
	pClient->HUD_ProcessPlayerState( dst, src );
}

int Hooked_HUD_Reset( void )
{
	return pClient->HUD_Reset( );	
}

void Hooked_HUD_Shutdown( void )
{
	pClient->HUD_Shutdown( );
}

void Hooked_HUD_StudioEvent( const struct mstudioevent_s *event, const struct cl_entity_s *entity )
{
	pClient->HUD_StudioEvent( event, entity );
}

void Hooked_HUD_TxferLocalOverrides( struct entity_state_s *state, const struct clientdata_s *client )
{
	pClient->HUD_TxferLocalOverrides( state, client );
}

void Hooked_HUD_TxferPredictionData( struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd )
{
	pClient->HUD_TxferPredictionData( ps, pps, pcd, ppcd, wd, pwd );
}

int Hooked_HUD_UpdateClientData( client_data_t *cdata, float flTime )
{
	return pClient->HUD_UpdateClientData( cdata, flTime );	
}

int Hooked_HUD_VidInit( void )
{
	return pClient->HUD_VidInit( );
}

extern DWORD dwNewEAT[35];

void SetupClientHooks( void )
{
	//add_log("setting up client hooks...");

	//*dwNewEAT[ INDEX_INITIALIZE ] = (DWORD) &Hooked_Initialize;
	dwNewEAT[ INDEX_HUD_Redraw ] = (DWORD) &Hooked_HUD_Redraw;

		
	// uncomment if needed ;)
	dwNewEAT[ INDEX_HUD_PlayerMove ] = (DWORD) &Hooked_HUD_PlayerMove;
	dwNewEAT[ INDEX_CL_CreateMove ] = (DWORD) &Hooked_CL_CreateMove;
	//dwNewEAT[ INDEX_V_CalcRefdef ] = (DWORD) &Hooked_V_CalcRefdef;
	dwNewEAT[ INDEX_HUD_AddEntity ] = (DWORD) &Hooked_HUD_AddEntity;
	dwNewEAT[ INDEX_HUD_Key_Event ] = (DWORD) &Hooked_HUD_Key_Event;
	//dwNewEAT[ INDEX_HUD_PostRunCmd ] = (DWORD) &Hooked_HUD_PostRunCmd;

	
	//--Basically unused functions
	//dwNewEAT[ INDEX_HUD_VidInit ] = (DWORD) &Hooked_HUD_VidInit;
	//dwNewEAT[ INDEX_HUD_UpdateClientData ] = (DWORD) &Hooked_HUD_UpdateClientData;
	//dwNewEAT[ INDEX_HUD_TxferLocalOverrides ] = (DWORD) &Hooked_HUD_TxferLocalOverrides;
	//dwNewEAT[ INDEX_HUD_StudioEvent ] = (DWORD) &Hooked_HUD_StudioEvent;
	//dwNewEAT[ INDEX_HUD_Shutdown ] = (DWORD) &Hooked_HUD_Shutdown;
	//dwNewEAT[ INDEX_HUD_Reset ] = (DWORD) &Hooked_HUD_Reset;
	//dwNewEAT[ INDEX_HUD_GetUserEntity ] = (DWORD) &Hooked_HUD_GetUserEntity;
	//dwNewEAT[ INDEX_HUD_GetHullBounds ] = (DWORD) &Hooked_HUD_GetHullBounds;
	//dwNewEAT[ INDEX_HUD_Frame ] = (DWORD) &Hooked_HUD_TxferPredictionData;
	//dwNewEAT[ INDEX_HUD_DrawTransparentTriangles ] = (DWORD) &Hooked_HUD_DrawTransparentTriangles;
	//dwNewEAT[ INDEX_HUD_DrawNormalTriangles ] = (DWORD) &Hooked_HUD_DrawNormalTriangles;
	//dwNewEAT[ INDEX_HUD_CreateEntities ] = (DWORD) &Hooked_HUD_CreateEntities;
	//dwNewEAT[ INDEX_HUD_ConnectionlessPacket ] = (DWORD) &Hooked_HUD_ConnectionlessPacket;
	//dwNewEAT[ INDEX_CL_IsThirdPerson ] = (DWORD) &Hooked_CL_IsThirdPerson;
	//dwNewEAT[ INDEX_CL_CameraOffset ] = (DWORD) &Hooked_CL_CameraOffset;
	//dwNewEAT[ INDEX_IN_DeactivateMouse ] = (DWORD) &Hooked_IN_DeactivateMouse;
	//dwNewEAT[ INDEX_IN_ClearStates ] = (DWORD) &Hooked_IN_ClearStates;
	//dwNewEAT[ INDEX_IN_ActivateMouse ] = (DWORD) &Hooked_IN_ActivateMouse;
	//dwNewEAT[ INDEX_IN_Accumulate ] = (DWORD) &Hooked_IN_Accumulate;
	//dwNewEAT[ INDEX_IN_MouseEvent ] = (DWORD) &Hooked_IN_MouseEvent;
	//dwNewEAT[ INDEX_HUD_PlayerMoveTexture ] = (DWORD) &Hooked_HUD_PlayerMoveTexture;
	//dwNewEAT[ INDEX_HUD_PlayerMoveInit ] = (DWORD) &Hooked_HUD_PlayerMoveInit;
	//dwNewEAT[ INDEX_HUD_Init ] = (DWORD) &Hooked_HUD_Init;
	
	//add_log("finished client hooks");
}
