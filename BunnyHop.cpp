#include "BunnyHop.h"
#include "Interfaces.h"
#include "TF2Entity.h"

using namespace Skim;

CBunnyHop::CBunnyHop()
{
	m_pPatchJump = new ConVar( PREFIX "bhop", "0");
}
void CBunnyHop::Run(CUserCmd* pCmd)
{
	if (!m_pPatchJump->GetBool()) return;

	//manualJump = true when we released
	static bool manualJump = false;
	if (pCmd->buttons & IN_JUMP)
	{
		if (!manualJump){
			if (!(TF2Entity::Me()->fFlags() & FL_ONGROUND ))
				pCmd->buttons &= ~IN_JUMP;
		}
		else //Don't unset jump button
			manualJump = false;
	}
	else
		manualJump = true;
}