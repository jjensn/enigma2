/*
tabrix mini-hack for Half-Life
Copyright (C) 2004 Niall FitzGibbon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//#include "client_headers.h"
#include "studio.h"

BoneMatrix_t bonecache[32][2];

bool g_bBBCheckAlwaysPass;

void InitStudioHooks(void)
{
	g_bBBCheckAlwaysPass = true;

	pEngstudio->StudioSetRemapColors = hook_StudioSetRemapColors;
	pEngstudio->StudioSetupModel = hook_StudioSetupModel;
	pEngstudio->StudioDrawPoints = hook_StudioDrawPoints;
	pEngstudio->StudioCheckBBox = hook_StudioCheckBBox;

	int i;

	for(i = 0; i < 32; i++)
	{
		memset(bonecache[i][0], 0, sizeof(BoneMatrix_t));
		memset(bonecache[i][1], 0, sizeof(BoneMatrix_t));
	}
}

void hook_StudioSetRemapColors(int top, int bottom)
{
	cl_entity_t *pEnt;

	pEnt = pEngstudio->GetCurrentEntity();
	if(pEnt && pEnt->player)
	{
		GetHeadPosition(pEnt);
		//DoRagdoll(pEnt);
	}

	oEngstudio.StudioSetRemapColors(top, bottom);
}

void hook_StudioSetupModel(int bodypart, void **ppbodypart, void **ppsubmodel)
{
	cl_entity_t *pEnt;

	oEngstudio.StudioSetupModel(bodypart, ppbodypart, ppsubmodel);

	//g_glstate.bLastBodypartWasPack = false;

	pEnt = pEngstudio->GetCurrentEntity();

	if(!pEnt || !pEnt->player)
		return;

	g_glstate.bLastBodypartWasPack = false;

	//add_log("body part: %d",bodypart);

	if(bodypart == 1)
	{
		////add_log("setting bLastBodypartWasPack to true...");
		g_glstate.bLastBodypartWasPack = true;
		//pEnt = pEngstudio->GetCurrentEntity();
	}
}

void hook_StudioDrawPoints(void)
{
	oEngstudio.StudioDrawPoints();
}

int hook_StudioCheckBBox(void)
{
	int ret;

	ret = oEngstudio.StudioCheckBBox();

	if(g_bBBCheckAlwaysPass)
		return 1;
	else
		return ret;
}

void GetHeadPosition(cl_entity_t *pEnt)
{
	model_t *pModel;
	studiohdr_t *pStudioHeader;
	BoneMatrix_t *pBoneMatrix;
	Vector vBBMin = Vector(0, 0, 0);
	Vector vBBMax = Vector(0, 0, 0);
	mstudiobbox_t *pHitbox;
	int i;

	if(!gESP.g_playerdata[pEnt->index - 1].bGotHead)
	{
		pModel = pEngstudio->SetupPlayerModel(pEnt->index);
		pStudioHeader = (studiohdr_t*)pEngstudio->Mod_Extradata(pModel);
		pBoneMatrix = (BoneMatrix_t*)pEngstudio->StudioGetBoneTransform();

		pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);

		for(i = 0; i < pStudioHeader->numhitboxes; i++)
		{
			//add_log("hitbox %d: [ bone %d, group %d, bbmin (%f, %f, %f), bbmax (%f, %f, %f) ]", i, pHitbox[i].bone, pHitbox[i].group, pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z, pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
			if(pHitbox[i].bone == 7 && pHitbox[i].group == 1)
			{
				// average the bounding box max and min to get the centre point to aim at
				gESP.g_playerdata[pEnt->index - 1].vPlayerHead = (vBBMax + vBBMin)*0.5f;
				gESP.g_playerdata[pEnt->index - 1].bGotHead = true;
				
			}
		}
	}
}