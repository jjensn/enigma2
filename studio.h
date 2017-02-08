#ifndef STUDIOHOOKS_H
#define STUDIOHOOKS_H

#include "client_headers.h"

extern bool g_bBBCheckAlwaysPass;

void InitStudioHooks(void);

// studio hooks
void hook_StudioSetRemapColors(int top, int bottom);
void hook_StudioSetupModel(int bodypart, void **ppbodypart, void **ppsubmodel);
void hook_StudioDrawPoints(void);
int hook_StudioCheckBBox(void);

void GetHeadPosition(cl_entity_t *pEnt);

typedef float BoneMatrix_t[MAXSTUDIOBONES][3][4];

#endif