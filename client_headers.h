#ifndef _CLIENT_HEADERS_H_
#define _CLIENT_HEADERS_H_

#include <windows.h>
#include <mmsystem.h>
#include <tlhelp32.h>
#include <gl/gl.h>

#include "ValveSDK/engine/wrect.h"
#include "ValveSDK/engine/cl_dll.h"
#include "ValveSDK/engine/cdll_int.h"
#include "ValveSDK/engine/const.h"
#include "ValveSDK/engine/progdefs.h"
#include "ValveSDK/engine/eiface.h"
#include "ValveSDK/engine/edict.h"
#include "ValveSDK/engine/studio_event.h"
#include "ValveSDK/engine/entity_types.h"
#include "ValveSDK/engine/pmtrace.h" 
#include "ValveSDK/common/ref_params.h"
#include "ValveSDK/common/screenfade.h"
#include "ValveSDK/common/event_api.h"
#include "ValveSDK/common/com_model.h"
#include "ValveSDK/misc/parsemsg.h"
#include "ValveSDK/engine/r_studioint.h"
#include "ValveSDK/engine/triangleapi.h"
#include "ValveSDK/engine/pm_defs.h"
#include "ValveSDK/engine/r_efx.h"
#include "ValveSDK/common/net_api.h"
#include "ValveSDK/engine/studio.h"

#pragma warning (disable:4786)
#pragma warning (disable:4244)
#pragma warning (disable:4731)
#pragma warning	(disable:4996)
#pragma warning	(disable:4005)

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <detours.h>

//#include "ValveSDK\common\mathlib.h"

// - - - - - - - - >

#include "valve-util.h"
#include "util.h"
#include "bunnyhop.h"
#include "speedo.h"
#include "playerinfo.h"
#include "autorecord.h"
#include "engine.h"
#include "anticonc.h"
#include "esp.h"
#include "sprites.h"
#include "RTFM.h"
#include "menu.h"
#include "fancyhud.h"
#include "autodet.h"
#include "aimbot.h"
#include "studio.h"
#include "nosmoke.h"
#include "flagstatus.h"
#include "acdefines.h"
// < - - - - - - - -

using namespace std;

typedef struct ExportTable_s
{
	int( *Initialize )( cl_enginefunc_t *, int );
	int( *HUD_Init )( void );
	int( *HUD_VidInit )( void );
	int( *HUD_Redraw )( float, int );
	int( *HUD_UpdateClientData )( client_data_t *, float );
	int( *HUD_Reset )( void );
	void( *HUD_PlayerMove )( struct playermove_s *, int );
	void( *HUD_PlayerMoveInit )( struct playermove_s * );
	char( *HUD_PlayerMoveTexture )( char * );
	void( *IN_ActivateMouse )( void );
	void( *IN_DeactivateMouse )( void );
	void( *IN_MouseEvent )( int mstate );
	void( *IN_ClearStates )( void );
	void( *IN_Accumulate )( void );
	void( *CL_CreateMove )( float, struct usercmd_s *, int );
	int( *CL_IsThirdPerson )( void );
	void( *CL_CameraOffset )( float * );
	struct kbutton_s *( *KB_Find )( const char *name );
	void( *CAM_Think )( void );
	void( *V_CalcRefdef )( struct ref_params_s *pparams );
	int( *HUD_AddEntity )( int, struct cl_entity_s *, const char * );
	void( *HUD_CreateEntities )( void );
	void( *HUD_DrawNormalTriangles )( void );
	void( *HUD_DrawTransparentTriangles )( void );
	void( *HUD_StudioEvent )( const struct mstudioevent_s *, const struct cl_entity_s * );
	void( *HUD_PostRunCmd )( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
	void( *HUD_Shutdown )( void );
	void( *HUD_TxferLocalOverrides )( struct entity_state_s *, const struct clientdata_s * );
	void( *HUD_ProcessPlayerState )( struct entity_state_s *, const struct entity_state_s * );
	void( *HUD_TxferPredictionData )( struct entity_state_s *, const struct entity_state_s *, struct clientdata_s *, const struct clientdata_s *, struct weapon_data_s *, const struct weapon_data_s * );
	void( *Demo_ReadBuffer )( int, unsigned char * );
	int( *HUD_ConnectionlessPacket )( struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
	int( *HUD_GetHullBounds )( int hullnumber, float *mins, float *maxs );
	void( *HUD_Frame )( double time );
	int( *HUD_Key_Event )( int, int, const char * );
	void( *HUD_TempEntUpdate )( double, double, double, struct tempent_s **, struct tempent_s **, int( *Callback_AddVisibleEntity )( struct cl_entity_s *pEntity ), void( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp ) );
	struct cl_entity_s *( *HUD_GetUserEntity )( int index );
/*	int( *HUD_VoiceStatus )( int entindex, qboolean bTalking );
	int (*HUD_DirectorEvent) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int (*HUD_GetStudioModelInterface)( int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio ); */
} ExportTable_t;

void SetupEngineHooks( void );
void SetupClientHooks( void );
void SetupUserMessageHooks( void );

#define INDEX_INITIALIZE					 0
#define INDEX_HUD_VidInit					 1
#define INDEX_HUD_Init						 2
#define INDEX_HUD_Redraw					 3
#define INDEX_HUD_UpdateClientData			 4
#define INDEX_HUD_Reset						 5
#define INDEX_HUD_PlayerMove				 6
#define INDEX_HUD_PlayerMoveInit			 7
#define INDEX_HUD_PlayerMoveTexture			 8
#define INDEX_IN_ActivateMouse				 9
#define INDEX_IN_DeactivateMouse			10
#define INDEX_IN_MouseEvent					11
#define INDEX_IN_ClearStates				12
#define INDEX_IN_Accumulate					13
#define INDEX_CL_CreateMove					14
#define INDEX_CL_IsThirdPerson				15
#define INDEX_CL_CameraOffset				16
#define INDEX_CAM_Think						17
#define INDEX_KB_Find						18
#define INDEX_V_CalcRefdef					19
#define INDEX_HUD_AddEntity					20
#define INDEX_HUD_CreateEntities			21
#define INDEX_HUD_DrawNormalTriangles		22
#define INDEX_HUD_DrawTransparentTriangles	23
#define INDEX_HUD_StudioEvent				24
#define INDEX_HUD_Shutdown					25
#define INDEX_HUD_TxferLocalOverrides		26
#define INDEX_Demo_ReadBuffer				27
#define INDEX_HUD_ConnectionlessPacket		28
#define INDEX_HUD_GetHullBounds				29
#define INDEX_HUD_Frame						30
#define INDEX_HUD_Key_Event					31
#define INDEX_HUD_PostRunCmd				32
#define INDEX_HUD_TempEntUpdate				33
#define INDEX_HUD_GetUserEntity				34

extern cl_enginefunc_t* pEngine;
extern cl_enginefunc_t	gEngine;
extern engine_studio_api_s oEngstudio;
extern engine_studio_api_s* pEngstudio;
extern ExportTable_t*	pClient;

#pragma comment(lib, "opengl32")

#endif