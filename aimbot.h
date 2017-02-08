#ifndef AIMBOT_H
#define AIMBOT_H

#define MAX_KEY_INDEX		1024

extern bool bOldKeyState[MAX_KEY_INDEX];

class CAimbot
{
public:

	void DoAimbot(usercmd_t *cmd);
	//int ChooseTarget(void);
public:
	int ChooseTarget(usercmd_t *cmd);
	float GetBlockingDistance(float *vec1, float *vec2);
	float GetEnemyAimDotProduct(cl_entity_t *pEnemy);
	bool IsAimingAtPlayer (int ax);
	bool bIsVisible(bool ducking, int entindex);
	void Pre_HUD_Init();
	void Cmd_Aimbot();

	bool bAimbot;

	inline bool GetOldKeyState(int iKey)
	{
		if(iKey < MAX_KEY_INDEX)
			return bOldKeyState[iKey];

		return false;
	}

inline void SetOldKeyState(int iKey, bool bState)
	{
		if(iKey < MAX_KEY_INDEX)
			bOldKeyState[iKey] = bState;
	}

};

extern CAimbot gAimbot;


#endif