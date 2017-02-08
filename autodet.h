#ifndef AUTODET_H
#define AUTODET_H

#define MAX_PIPES 8

typedef struct
{
	float X;
	float Y;
	float Z;
	int index;
} pipedata_t;

class CAutodet
{
	public:

	virtual bool Pre_HUD_Init (void);
	
	virtual void Cmd_Autodet (void);

	virtual void Cmd_Autodetdebug(void);

	virtual void Pre_HUD_Redraw (float flTime, int intermission);
	
	virtual int  Post_HUD_AddEntity (int type, struct cl_entity_s *ent, const char* modelname);

	virtual void UpdatePipes(void);

	virtual bool bIsVisible(bool ducking, int entindex);
	
	virtual bool bIsNear (float x1, float x2, float y1, float y2, float z1, float z2);

	bool bAutodet;

	bool bSafe;
	
	bool bStupid;

	pipedata_t pipes[MAX_PIPES];

	int iCurrPipe;

	int sens;

};

extern CAutodet gAutodet;

#endif