#ifndef __req_
#define __req_

//SDK stuff
#define CLIENT_DLL
#pragma warning (disable : 4005 )
#include "..\game\client\cdll_client_int.h"
#include "..\game\client\cbase.h"
#include "..\game\client\input.h"
#include "vgui\IPanel.h"
#include "VGuiMatSurface\IMatSystemSurface.h"
#include "achievementmgr.h"

#include "miscTools.h"


#define PLUGIN_NAME        "[skimbot]"
#define PLUGIN_ERROR    "[skimbot] ERROR: "
#define PREFIX            "st_"
#define PLUGIN_VACBANNABLES 0

//Defined this stuff here so I didn't have to include more sdk stuff
#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2
#endif