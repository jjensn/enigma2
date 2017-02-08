#include "opengl.h"
#include "client_headers.h"
#include "mempatcher.h"

glstate_t g_glstate;
int q;
int w;

char buff[45];

typedef FARPROC (WINAPI* GetProcAddress_t) ( IN HMODULE hModule, IN LPCSTR lpProcName );

glBegin_t		 pglBegin;
glVertex3f_t	 pglVertex3f;
glVertex3fv_t	 pglVertex3fv;
glPopMatrix_t	 pglPopMatrix;
glViewport_t	 pglViewport;
glClear_t		 pglClear;
GetProcAddress_t pGetProcAddress;
//wglSwapBuffers_t pwglSwapBuffers;

FARPROC WINAPI hook_GetProcAddress ( IN HMODULE hModule, IN LPCSTR lpProcName )
{
	// use this only if the mod you use doesn't support the other method
	return pGetProcAddress( hModule, lpProcName );
}

void APIENTRY hooked_glBegin ( GLenum mode )
{

	if(pBuf == NULL || !pBuf)
		return pglBegin( mode );

	if(strstr(pBuf,SCREEN_SHOULD_BE_TAKEN))
	{
		bHackOn = 0;

		q++;

		if(q >= 5000)
		{
			q = 0;
			sprintf(buff,"%s",CHEAT_DISABLED);
			CopyMemory((PVOID)pBuf, buff, strlen(buff));
		}
	}

	if(strstr(pBuf,SCREEN_WAS_TAKEN))
	{
		w++;

		if(w >= 5000)
		{
			w = 0;
			bHackOn = 1;
		}
		
		//pEngine->pfnConsolePrint("snapshot!\n");
	}

	return pglBegin( mode );
}

void APIENTRY hooked_glClear ( GLbitfield mask )
{
	//MessageBoxA(NULL,"glClear","glClear",MB_OK);
	return pglClear( mask );
}

void APIENTRY hooked_glViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
	//MessageBoxA(NULL,"viewport","viewport",MB_OK);

	pEngine->pfnConsolePrint("viewport\n");
	g_glstate.fLastViewport[VP_X] = x;
	g_glstate.fLastViewport[VP_Y] = y;
	g_glstate.fLastViewport[VP_W] = width;
	g_glstate.fLastViewport[VP_H] = height;
	return pglViewport( x, y, width, height );
}

void APIENTRY hooked_glPopMatrix ( void )
{;
//MessageBoxA(NULL,"pop","pop",MB_OK);
	pglPopMatrix();
}

void APIENTRY hooked_glVertex3f( GLfloat x, GLfloat y, GLfloat z )
{
	cl_entity_t *pEnt;

	pEnt = pEngstudio->GetCurrentEntity();
	//add_log("hook_glVertex3f... orig_glVertex3f = 0x%X", orig_glVertex3f);    

	if(g_glstate.bLastBodypartWasPack)
	{
		//add_log("hook_glVertex3f: g_glstate.bLastBodypartWasPack is true, checking for pack...");
		if(g_iModID == MODID_CS)
		{
			//add_log("CS client loaded, continuing check...");
			if(pEnt && pEnt->player)
			{
				int index = pEnt->index - 1;

				//add_log("hook_glVertex3f: found player %d has bomb or defuse kit", index + 1);

				if(gESP.g_playerdata[index].iTeam == 1)
					gESP.g_playerdata[index].bHasBomb = true;
				else
					gESP.g_playerdata[index].bHasDefuse = true;

				
			}
		}
		g_glstate.bLastBodypartWasPack = false;
	}
	pglVertex3f( x, y, z );
}

void APIENTRY hooked_glVertex3fv( const GLfloat *v )
{
	pglVertex3fv( v );
}

void CodeWalk( DWORD dwStartAddress, DWORD dwEndAddress )
{
	for ( DWORD dwCurrentAddress = dwStartAddress; dwCurrentAddress <= dwEndAddress; dwCurrentAddress++ )
	{
		PDWORD pdwTableAddress = ( PDWORD ) dwCurrentAddress;
		
		HMODULE hmOpenGL = LoadLibrary( "opengl32.dll" );

		if ( *pdwTableAddress == ( DWORD ) GetProcAddress( hmOpenGL, "glClear" ) )
		{
			pglClear = ( glClear_t ) *pdwTableAddress;
			*pdwTableAddress = ( DWORD ) &hooked_glClear;
		}
		else if ( *pdwTableAddress == ( DWORD ) GetProcAddress( hmOpenGL, "glBegin" ) )
		{
			pglBegin = ( glBegin_t ) *pdwTableAddress;
			*pdwTableAddress = ( DWORD ) &hooked_glBegin;
		}
		else if ( *pdwTableAddress == ( DWORD ) GetProcAddress( hmOpenGL, "glViewport" ) )
		{
			pglViewport = ( glViewport_t ) *pdwTableAddress;
			*pdwTableAddress = ( DWORD ) &hooked_glViewport;
		}
		else if ( *pdwTableAddress == ( DWORD ) GetProcAddress( hmOpenGL, "glPopMatrix" ) )
		{
			pglPopMatrix = ( glPopMatrix_t ) *pdwTableAddress;
			*pdwTableAddress = ( DWORD ) &hooked_glPopMatrix;
		}
		else if ( *pdwTableAddress == ( DWORD ) GetProcAddress( hmOpenGL, "glVertex3f" ) )
		{
			pglVertex3f = ( glVertex3f_t ) *pdwTableAddress;
			*pdwTableAddress = ( DWORD ) &hooked_glVertex3f;
		}
		else if ( *pdwTableAddress == ( DWORD ) GetProcAddress( hmOpenGL, "glVertex3fv" ) )
		{
			pglVertex3fv = ( glVertex3fv_t ) *pdwTableAddress;
			*pdwTableAddress = ( DWORD ) &hooked_glVertex3fv;
		}
	}
}


void SetupOpenGLHooks( void )
{
	//add_log("setting up ogl hooks...");
	DWORD dwImportCode = dwFindPattern( 0x01D7B000, 0x000FF000 , (BYTE*) "\x83\xC4\x04\x8B\x4D\x08\x51\xE8\x00\x00\x00\x00\x83\xC4\x04\xA3\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x00\x75\x49\xC7\x85\xF4\xFB\xFF\xFF\x00\x6A\x00\x6A\x00\x8D\x95\xF4\xFB\xFF\xFF" ,"xxxxxxxx????xxxx????xx????" );

	g_glstate.fLastViewport[0] = 0.0f;
	g_glstate.fLastViewport[1] = 0.0f;
	g_glstate.fLastViewport[2] = 0.0f;
	g_glstate.fLastViewport[3] = 0.0f;
	g_glstate.iEnableSinceViewportCounter = 0;
	g_glstate.iViewportSinceSwapBuffersCounter = 0;
	g_glstate.vCamera = Vector(0, 0, 0);

	if ( dwImportCode == NULL )
	{
		// that shouldn't be, but can be used, works for 3rd party mods if they import opengl when you join a game
		pGetProcAddress = ( GetProcAddress_t ) dwDetour( (DWORD) GetProcAddress, (DWORD) hook_GetProcAddress );
	}
	else
	{
		PDWORD pdwTableBase = (PDWORD) ( dwImportCode + 0x66 + 0x13 );
		CodeWalk( *pdwTableBase, *pdwTableBase + 0xFF00 );
	}
	//add_log("finsished ogl hooks");
}