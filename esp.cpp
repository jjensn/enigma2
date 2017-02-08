#include "client_headers.h"
#include "esp.h"

CESP gESP;

DECLARE_COMMAND_2(gESP, ESP);

char meh[128];
int a = 0;

enum { 
SEQUENCE_SHOOT=1,
SEQUENCE_RELOAD=2,
SEQUENCE_DIE=3,
SEQUENCE_THROW=4,
SEQUENCE_ARM_C4=5 
}; 

int Cstrike_SequenceInfo[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   
    0, 1, 2, 0, 1, 2, 0, 1, 2, 0,  
    1, 2, 0, 1, 1, 2, 0, 1, 1, 2,  
    0, 1, 2, 0, 1, 2, 0, 1, 2, 0,  
    1, 2, 0, 1, 2, 0, 1, 2, 0, 1,  
    2, 0, 1, 2, 0, 0, 0, 4, 0, 4, 
    0, 5, 0, 5, 0, 0, 1, 1, 2, 0, 
    1, 1, 2, 0, 1, 0, 1, 0, 1, 2, 
    0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
    3
};  

bool CESP::Pre_HUD_Init(void)
{
	HOOK_COMMAND_2("e_esp",ESP);

	memset(&g_playerdata, 0, sizeof(g_playerdata));

	return true;
}

void CESP::Cmd_ESP(void)
{
	if(gESP.bESP == 0)
	{
		// esp on!
		gESP.bESP = 1;
		econsoleprint("esp enabled",true);
		return;
	}

	if(gESP.bESP == 1)
	{
		// esp off!
		gESP.bESP = 0;
		econsoleprint("esp disabled",true);
		return;
	}

}

void CESP::Pre_HUD_Redraw(float flTime,int intermission)
{

	cl_entity_t *pLocal, *pEnt;
	Vector vColor1 = Vector(1.0f, 1.0f, 1.0f);
	Vector vColor2 = Vector(1.0f, 1.0f, 1.0f);
	Vector vColor3 = Vector(1.0f, 1.0f, 1.0f);
	Vector vViewOffset, vScreenPos, vTagOffset;
	int i, index, spriteboxindex;
	float fStrWidth, fBoxWidth, fBoxHeight, fBoxMinWidth, fTaperWidth;
	wrect_t rcSpriteBox;
	int spriteboxborder, boxspritex, boxspritey;
	bool bUsingSpriteBox;

	if(!gESP.bESP)
		return;

	pLocal = pEngine->GetLocalPlayer();
	if(!pLocal) return;

	pEngine->pEventAPI->EV_LocalPlayerViewheight(vViewOffset);

	if(g_iModID == MODID_CS)
		vTagOffset = Vector(0, 0, 32);
	else
		vTagOffset = Vector(0, 0, 40);

	fBoxMinWidth = g_screeninfo.charWidths['A'] * 8 + 16.0f;
	fBoxHeight = g_screeninfo.iCharHeight * 2.0f;
	fTaperWidth = fBoxHeight * 0.5f;
	spriteboxborder = 2;

	for(i = 0; i < 32; i++)
	{
		if(g_playerdata[i].bCurrent)
		{
			spriteboxindex = -1;
			bUsingSpriteBox = false;

			pEnt = pEngine->GetEntityByIndex(i + 1);
			if(pEnt && pEnt->player && pEnt->index != pLocal->index)
			{
				if(g_playerdata[i].szName[0])
				{
					if(g_iModID == MODID_CS)
					{
						if(g_playerdata[i].bHasBomb)
						{
							spriteboxindex = GetSpriteIndex("c4");
							bUsingSpriteBox = true;
						}
						else
						{
							if(g_playerdata[i].bHasDefuse)
							{
								spriteboxindex = GetSpriteIndex("defuser");
								bUsingSpriteBox = true;
							}
						}

						if(pEnt->curstate.solid == SOLID_NOT)
						{
							spriteboxindex = GetSpriteIndex("smallskull");
							bUsingSpriteBox = true;
						}
					}

					if(spriteboxindex == -1)
						bUsingSpriteBox = false;

					fStrWidth = CalcStringWidth(g_playerdata[i].szName);
					fBoxWidth = fStrWidth + 16.0f;

					if(fBoxWidth < fBoxMinWidth)
						fBoxWidth = fBoxMinWidth;

					vColor1 = g_playerdata[i].vColor1;
					vColor2 = g_playerdata[i].vColor2;
					vColor3 = g_playerdata[i].vColor3;

					if(CalcScreen(pEnt->origin + vTagOffset, vScreenPos))
					{
						vScreenPos.x = (int)vScreenPos.x;
						vScreenPos.y = (int)vScreenPos.y;

						if(bUsingSpriteBox)
						{
							rcSpriteBox.bottom = vScreenPos.y - fBoxHeight;
							rcSpriteBox.top = rcSpriteBox.bottom - (g_spritedata.spriteinfo[spriteboxindex].rc.bottom - g_spritedata.spriteinfo[spriteboxindex].rc.top) - spriteboxborder * 2.0f;
							rcSpriteBox.right = vScreenPos.x + (fBoxWidth * 0.5f) - (fTaperWidth * 0.5f);
							rcSpriteBox.left = rcSpriteBox.right - (g_spritedata.spriteinfo[spriteboxindex].rc.right - g_spritedata.spriteinfo[spriteboxindex].rc.left) - spriteboxborder * 2.0f;
							boxspritex = rcSpriteBox.left + spriteboxborder;
							boxspritey = rcSpriteBox.top + spriteboxborder;
						}

						glDisable(GL_TEXTURE_2D);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						
						// shaded background
						glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

						// top wide quad
						glBegin(GL_QUADS);
						glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f), vScreenPos.y - fBoxHeight);
						glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f), vScreenPos.y - fBoxHeight);
						glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f), vScreenPos.y - (fBoxHeight * 0.5f));
						glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f), vScreenPos.y - (fBoxHeight * 0.5f));
						// bottom narrow quad
						glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f) + fTaperWidth, vScreenPos.y - (fBoxHeight * 0.5f));
						glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f) - fTaperWidth, vScreenPos.y - (fBoxHeight * 0.5f));
						glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f) - fTaperWidth, vScreenPos.y);
						glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f) + fTaperWidth, vScreenPos.y);
						glEnd();

						// taper triangles
						glBegin(GL_TRIANGLES);
						glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f) + fTaperWidth, vScreenPos.y);
						glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f) + fTaperWidth, vScreenPos.y - (fBoxHeight * 0.5f));
						glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f), vScreenPos.y - (fBoxHeight * 0.5f));
						glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f) - fTaperWidth, vScreenPos.y);
						glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f) - fTaperWidth, vScreenPos.y - (fBoxHeight * 0.5f));
						glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f), vScreenPos.y - (fBoxHeight * 0.5f));
						glEnd();

						// if player has defuse kit or bomb, draw that on a tab at the top right
						if(bUsingSpriteBox)
						{
							glBegin(GL_TRIANGLES);
							glVertex2f(rcSpriteBox.left - fTaperWidth * 0.5f, rcSpriteBox.bottom);
							glVertex2f(rcSpriteBox.left, rcSpriteBox.top);
							glVertex2f(rcSpriteBox.left, rcSpriteBox.bottom);
							glVertex2f(rcSpriteBox.right + fTaperWidth * 0.5f, rcSpriteBox.bottom);
							glVertex2f(rcSpriteBox.right, rcSpriteBox.top);
							glVertex2f(rcSpriteBox.right, rcSpriteBox.bottom);
							glEnd();

							glBegin(GL_QUADS);
							glVertex2f(rcSpriteBox.left, rcSpriteBox.top);
							glVertex2f(rcSpriteBox.right, rcSpriteBox.top);
							glVertex2f(rcSpriteBox.right, rcSpriteBox.bottom);
							glVertex2f(rcSpriteBox.left, rcSpriteBox.bottom);
							glEnd();
						}

						if(bUsingSpriteBox)
						{
							// outline for defuse/bomb tab
							glColor4f(vColor3.x, vColor3.y, vColor3.z, 0.5f);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE);
							glBegin(GL_LINE_LOOP);
							glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f), vScreenPos.y - fBoxHeight);
							glVertex2f(rcSpriteBox.left - fTaperWidth * 0.5f, rcSpriteBox.bottom);
							glVertex2f(rcSpriteBox.left, rcSpriteBox.top);
							glVertex2f(rcSpriteBox.right, rcSpriteBox.top);
							glVertex2f(rcSpriteBox.right + fTaperWidth * 0.5f, rcSpriteBox.bottom);
							glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f), vScreenPos.y - (fBoxHeight * 0.5f));
							glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f) - fTaperWidth, vScreenPos.y);
							glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f) + fTaperWidth, vScreenPos.y);
							glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f), vScreenPos.y - (fBoxHeight * 0.5f));
							glEnd();
						}
						else
						{
							// regular outline
							glColor4f(vColor3.x, vColor3.y, vColor3.z, 0.5f);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE);
							glBegin(GL_LINE_LOOP);
							glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f), vScreenPos.y - fBoxHeight);
							glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f), vScreenPos.y - fBoxHeight);
							glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f), vScreenPos.y - (fBoxHeight * 0.5f));
							glVertex2f(vScreenPos.x + (fBoxWidth * 0.5f) - fTaperWidth, vScreenPos.y);
							glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f) + fTaperWidth, vScreenPos.y);
							glVertex2f(vScreenPos.x - (fBoxWidth * 0.5f), vScreenPos.y - (fBoxHeight * 0.5f));
							glEnd();
						}

						glDisable(GL_BLEND);
						glEnable(GL_TEXTURE_2D);

						// draw sprite in spritebox
						if(bUsingSpriteBox)
						{
							//DrawAddSpriteByIndex(spriteboxindex, 0, boxspritex, boxspritey, (int)255*vColor3.x, (int)255*vColor3.y, (int)255*vColor3.z);
						}
					}

					//[2]
					DrawAt3DPos(pEnt->origin + vTagOffset, Vector(0, g_screeninfo.iCharHeight, 0), g_playerdata[i].szName, vColor1, HUDTEXT_ALIGN_CENTRE);

					if(g_iModID == MODID_CS) {
					//	//if(pEnt)
						if(pEnt->curstate.solid != SOLID_NOT)
							a = Cstrike_SequenceInfo[pEnt->curstate.sequence];
					}

					Cstrike_SequenceInfo[pEnt->curstate.sequence];

					if(a > 5)
						a = 0;

					if(a == 1)
						sprintf(meh,"%s","- shooting -");
					else if(a == 2)
						sprintf(meh,"%s","- reloading -");
					else if(a == 3)
						sprintf(meh,"%s","- hit! -");
					else if(a == 4)
						sprintf(meh,"%s","- throw -");
					else if (a == 5)
						sprintf(meh,"%s","- planting -");
					else
						sprintf(meh,"%s","NULL");

					if(!strstr(meh,"NULL"))
						DrawAt3DPos(pEnt->origin + vTagOffset, Vector(0, -15, 0), meh, vColor3, HUDTEXT_ALIGN_CENTRE);					

					if(pEnt->curstate.solid == SOLID_NOT || pEnt->curstate.movetype == MOVETYPE_TOSS)
						DrawAt3DPos(pEnt->origin + vTagOffset, Vector(0, 0/*g_screeninfo.iCharHeight * 2*/, 0), "DEAD", vColor3, HUDTEXT_ALIGN_CENTRE);
						//add_log("dead");
					else /* [1] */
					{
						if(0/*GetEnemyAimDotProduct(pEnt) > DOT_AIMING*/)
							DrawAt3DPos(pEnt->origin + vTagOffset, Vector(0, 0/*g_screeninfo.iCharHeight * 2*/, 0), "AIM", vColor3, HUDTEXT_ALIGN_CENTRE);
					}

					index = GetSpriteIndex(GetSpriteNameFromModelName(g_playerdata[i].szWeaponModel));
					if(index != -1)
					{
						if(CalcScreen(pEnt->origin + vTagOffset, vScreenPos))
						{
							vScreenPos.x -= (g_spritedata.spriteinfo[index].rc.right - g_spritedata.spriteinfo[index].rc.left) * 0.5f;
							vScreenPos.y -= g_screeninfo.iCharHeight;
							DrawAddSpriteByIndex(index, 0, (int)vScreenPos.x, (int)vScreenPos.y, (int)255*vColor2.x, (int)255*vColor2.y, (int)255*vColor2.z);
						}
					}
					else
					{
						if(pEnt->curstate.solid == SOLID_NOT || pEnt->curstate.movetype == MOVETYPE_TOSS)
							DrawAt3DPos(pEnt->origin + vTagOffset, Vector(0, 0, 0), g_playerdata[i].szWeaponModel, vColor2, HUDTEXT_ALIGN_CENTRE);
					}
				}/*[/1]*/
			}
		}
	}
}

void CESP::Pre_CL_CreateMove(void)
{
	hud_player_info_t pPlayerInfo;
	int i, j, len;
	cl_entity_t *pLocal, *pEnt;
	model_t *pWeaponModel;

	//add_log("starting...");

	pLocal = pEngine->GetLocalPlayer();

	for(i = 0; i < 32; i++)
	{
		g_playerdata[i].szName[0] = '\0';
		g_playerdata[i].szModel[0] = '\0';
		g_playerdata[i].szWeaponModel[0] = '\0';
		g_playerdata[i].bGotHead = false;
		//g_playerdata[i].bCurrent = false;
		g_playerdata[i].iAimWeighting = 0;
		pEnt = pEngine->GetEntityByIndex(i+1); 
		if(pEnt && pLocal && pEnt->player)
		{
			if(PlayerIsCurrent(pEnt, pLocal))
			{
				// set up the origin and velocity
				float fDeltaTime = gEngine.GetClientTime() - g_playerdata[i].fLastUpdateTime;
				if(fDeltaTime != 0.0f)
				{
					g_playerdata[i].vVelocity = (pEnt->origin - g_playerdata[i].vOrigin) / (gEngine.GetClientTime() - g_playerdata[i].fLastUpdateTime);
				}
				g_playerdata[i].vOrigin = pEnt->origin;
				g_playerdata[i].fLastUpdateTime = gEngine.GetClientTime();

				if(!g_playerdata[i].bCurrent)
				{
					// player just became current, zero velocity
					g_playerdata[i].vVelocity = Vector(0.0f, 0.0f, 0.0f);
				}

				pEngine->pfnGetPlayerInfo(i+1, &pPlayerInfo);
				if(&pPlayerInfo)
				{
					if(pPlayerInfo.name && pPlayerInfo.name[0])
						strncpy(g_playerdata[i].szName, pPlayerInfo.name, MAX_PLAYER_INFO_STRING);
					if(pPlayerInfo.model && pPlayerInfo.model[0])
						strncpy(g_playerdata[i].szModel, pPlayerInfo.model, MAX_PLAYER_INFO_STRING);
				}

				pWeaponModel = pEngstudio->GetModelByIndex(pEnt->curstate.weaponmodel);
				if(pWeaponModel && pWeaponModel->name && pWeaponModel->name[0])
				{
					// remove model path, p_ and trailing extension
					len = (int)strlen(pWeaponModel->name);
					for(j = 0; j < len; j++)
					{
						if(pWeaponModel->name[j] == '_')
							strncpy(g_playerdata[i].szWeaponModel, &(pWeaponModel->name[j+1]), 32);
					}
					if(strlen(g_playerdata[i].szWeaponModel) > 4)
						g_playerdata[i].szWeaponModel[strlen(g_playerdata[i].szWeaponModel) - 4] = '\0';
				}

				CalculatePlayerTeam(i);


				GetColorsForTeam(g_playerdata[i].iTeam, pLocal->curstate.team, g_playerdata[i].vColor1, g_playerdata[i].vColor2, g_playerdata[i].vColor3);

				g_playerdata[i].bHasBomb = false;
				g_playerdata[i].bHasDefuse = false;
				g_playerdata[i].bHasShield = false;
				g_playerdata[i].bIsReloading = false;
				g_playerdata[i].bTarget = false;

				g_playerdata[i].bCurrent = true;
			}
			else
				g_playerdata[i].bCurrent = false;
		}
	}

	//add_log("ending...");
}

bool CESP::PlayerIsCurrent(cl_entity_t *pEnt, cl_entity_t *pLocal)
{
	if(pEnt->player && pLocal && pLocal->curstate.messagenum - pEnt->curstate.messagenum < 20)
		return true;
	else
		return false;
}

void CESP::CalculatePlayerTeam(int index)
{
	// terrorist models: leet, terror, arctic, guerilla
	// counter-terrorist models: gign, gsg9, urban, sas, vip
	char *p = gESP.g_playerdata[index].szModel;

	if(p[0] == '\0')
	{
		gESP.g_playerdata[index].iTeam = 0;
		return;
	}

	if(g_iModID == MODID_TFC)
	{
		cl_entity_t *pEnt = pEngine->GetEntityByIndex(index + 1);
		if(pEnt && pEnt->player)
			gESP.g_playerdata[index].iTeam = pEnt->curstate.team;
		return;
	}

	if(g_iModID == MODID_CS)
	{
		if(!strcmp(p, "leet") || !strcmp(p, "terror") || !strcmp(p, "arctic") || !strcmp(p, "guerilla"))
			gESP.g_playerdata[index].iTeam = 1;
		else
			gESP.g_playerdata[index].iTeam = 2;
		return;
	}
    
	gESP.g_playerdata[index].iTeam = 0;
}

void CESP::GetColorsForTeam(int iTeam, int myTeam, float *vColor1, float *vColor2, float *vColor3)
{
	if(g_iModID == MODID_CS)
	{
		if(iTeam == 1)
		{
			{
				VectorCopy(Vector(1.0f, 0.1f, 0.1f), vColor1);
				VectorCopy(Vector(1.0f, 0.2f, 0.1f), vColor3);
				vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
				vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
				vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;
				return;
			}
		}

		if(iTeam == 2)		
		{
			VectorCopy(Vector(0.2f, 0.3f, 1.0f), vColor1);
			VectorCopy(Vector(0.3f, 0.5f, 1.0f), vColor3);
			vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
			vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
			vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;
			return;
		}

		VectorCopy(Vector(0.8f, 0.8f, 0.8f), vColor1);
		VectorCopy(Vector(0.4f, 0.4f, 0.4f), vColor3);
		vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
		vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
		vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;
		return;
	}

	if(g_iModID == MODID_TFC)
	{
		if(iTeam == 1)
		{
			VectorCopy(Vector(0.2f, 0.3f, 1.0f), vColor1);
			VectorCopy(Vector(0.3f, 0.5f, 1.0f), vColor3);
			vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
			vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
			vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;
			return;
		}

		if(iTeam == 2)
		{
			VectorCopy(Vector(1.0f, 0.1f, 0.1f), vColor1);
			VectorCopy(Vector(1.0f, 0.2f, 0.1f), vColor3);
			vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
			vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
			vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;
			return;
		}

		if(iTeam == 3)
		{
			VectorCopy(Vector(1.0f, 1.0f, 0.2f), vColor1);
			VectorCopy(Vector(1.0f, 0.8f, 0.4f), vColor3);
			vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
			vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
			vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;
			return;
		}

		if(iTeam == 4)
		{
			VectorCopy(Vector(0.2f, 1.0f, 0.3f), vColor1);
			VectorCopy(Vector(0.1f, 0.5f, 0.2f), vColor3);
			vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
			vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
			vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;
			return;
		}

		VectorCopy(Vector(0.8f, 0.8f, 0.8f), vColor1);
		VectorCopy(Vector(0.4f, 0.4f, 0.4f), vColor3);
		vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
		vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
		vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;
		return;
	}
	VectorCopy(Vector(0.8f, 0.8f, 0.8f), vColor1);
	VectorCopy(Vector(0.4f, 0.4f, 0.4f), vColor3);
	vColor2[0] = (vColor1[0] + vColor3[0]) * 0.5f;
	vColor2[1] = (vColor1[1] + vColor3[1]) * 0.5f;
	vColor2[2] = (vColor1[2] + vColor3[2]) * 0.5f;

}

char *CESP::GetSpriteNameFromModelName(const char *szModelName)
{
	static char buf[4096];

	int i;
	char buf2[4096];

	buf[0] = '\0';

	if(!szModelName || !szModelName[0])
	{
		strcpy(buf, "_");
		return buf;
	}

	if(g_iModID == MODID_CS)
	{
		sprintf(buf, "d_%s", szModelName);
		return buf;
	}

	if(g_iModID == MODID_TFC)
	{
		// also the weapon model name may be trailed by "2" for new models, so take account of this
		char names[32][2][32] =
		{
			{"spanner",			"d_spanner"},
			{"medkit",			"d_medikit"},
			{"knife",			"d_knife"},
			{"glauncher",		"d_gl_grenade"},
			{"sniper",			"d_sniperrifle"},
			{"mini",			"d_ac"},
			{"smallshotgun",	"d_shotgun"},
			{"shotgun",			"d_supershotgun"},
			{"rpg",				"d_rocket"},
			{"srpg",			"d_rocket"},
			{"nailgun",			"d_nails"},
			{"snailgun",		"d_nails"},
			{"egon",			"d_flames"},
			{"crowbar",			"d_axe"},
			{"umbrella",		"d_axe"},
			{"spygun",			"d_railgun"},
			{"9mmhandgun",		"d_railgun"},
			{"END",				"END"}
		};

		for(i = 0; i <32; i++)
		{
			if(!strncmp(names[i][0], "END", 31))
				continue;

            strncpy(buf2, names[i][0], 31);

			if(!strncmp(szModelName, buf2, 31))
			{
				strncpy(buf, names[i][1], 31);
				return buf;
			}

			strcat(buf, "2");
			if(!strncmp(szModelName, buf2, 31))
			{
				strncpy(buf, names[i][1], 31);
				return buf;
			}
		}

		strcpy(buf, "_");
		return buf;
	}

	sprintf(buf, "d_%s", szModelName);
	return buf;
}
