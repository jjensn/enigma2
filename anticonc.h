#ifndef ANTICONC_H
#define ANTICONC_H

class CAnticonc
{
	public:
	virtual bool Pre_HUD_Init (void);
	
	virtual void Cmd_Anticonc (void);

	virtual void Pre_HUD_Redraw (float flTime, int intermission);

	bool bInConc;
	float  iConcAmt;
	bool bNoConc;
	bool bXHair;
};

extern CAnticonc gAnticonc;

#endif