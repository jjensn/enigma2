#ifndef _OPENGL_H_
#define _OPENGL_H_
#include "client_headers.h"
#include <Windows.h>
#include <gl/GL.h>
#include <stdio.h>
#define MACRO_REPORT_ERROR( x ) MessageBox( 0, x, "Error", 0 );

#define VP_X	0
#define VP_Y	1
#define VP_W	2
#define	VP_H	3

void SetupOpenGLHooks( void );
void DestroyOpenGLHooks( void );

typedef struct
{
	int iViewportSinceSwapBuffersCounter;	// counts the number of calls to glViewport since start of frame
	int iEnableSinceViewportCounter;		// counts the number of calls to glEnable since the last glViewport
	Vector vCamera;
	float fLastViewport[4];
	bool bRenderedFirstModel;
	bool bRenderedWorld;
	bool bLastBodypartWasPack;				// used to determine if the packs set up by the studio are actually rendered
} glstate_t;

extern glstate_t g_glstate;

typedef void ( APIENTRY* glBegin_t ) ( GLenum mode );
typedef void ( APIENTRY* glVertex3f_t ) ( GLfloat x, GLfloat y, GLfloat z );
typedef void ( APIENTRY* glVertex3fv_t ) ( const GLfloat *v );
typedef void ( APIENTRY* glPopMatrix_t ) ( void );
typedef void ( APIENTRY* glViewport_t ) ( GLint x, GLint y, GLsizei width, GLsizei height );
typedef void ( APIENTRY* glClear_t ) ( GLbitfield mask );
typedef void ( APIENTRY* wglSwapBuffers_t ) ( HDC hDC);

inline bool CheckAgainstViewport(int x, int y)
{
    if(x < g_glstate.fLastViewport[VP_X] || x > g_glstate.fLastViewport[VP_W] || y < g_glstate.fLastViewport[VP_Y] || y > g_glstate.fLastViewport[VP_H])
		return false;
	else
		return true;
}

extern glBegin_t		pglBegin;
extern glVertex3f_t		pglVertex3f;
extern glVertex3fv_t	pglVertex3fv;
extern glPopMatrix_t	pglPopMatrix;
extern glViewport_t		pglViewport;
extern glClear_t		pglClear;
extern wglSwapBuffers_t	pwglSwapBuffers;


#endif