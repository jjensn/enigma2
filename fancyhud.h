#ifndef HUD_H
#define HUD_H

#define FLOATING_TEXT_SLOTS 256

#define HC_HUDLEVEL 0
#define HC_SOUNDPATCH 17
#define HC_SPEEDO_SPEED 18
#define HC_SPEEDO_MAX 19
#define HC_SPEEDO_PERCENT 20
//#define HC_BHOP_PRED 20
#define HC_IN_GAME_COLOR 21
#define HC_STATS_OPTION 25
#define HC_STATS_STATS 26
//#define HC_STATS_RATIO 27
#define HC_WINAMP_CURSONG 28
#define HC_WINAMP_CURSONG_STATUS 29

#define HC_USER_TIMER 48
#define HC_CLOCK 49
#define HC_ROUND_TIMER 50
#define HC_IRC_DISPLAY 51
#define HC_IRC_CHANNEL 52
#define HC_IRC_TO_GAME 53
#define HC_GAME_TO_IRC 54
#define HC_IRC_GLOBALSAY 55

class CHUDChannel
{
	public:

	virtual void WriteHUDChannel (float fPeriod, char *WriteText);
	virtual void DisplayHUDChannel (int iSlot);

	CHUDChannel (void);
	
	float fStartTime;
	float fEndTime;
	char Text[512];
	bool bRendered[1024];
};

void Fancy_HUD_Redraw (float flTime,int intermission);

extern CHUDChannel Channel[64];

#endif