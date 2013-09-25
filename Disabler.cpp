#include "Disabler.h"
#include "client_textmessage.h"

#pragma region Static stuff
//----------------------------------------
//------Static stuff compiler needs-------
//----------------------------------------
bool CDisabler::m_bDisableSetAngles;
CUserCmd *CDisabler::m_pOldCmd;
CVMTHook *CDisabler::m_hSetViewAngles;
#pragma endregion
//========================================
//============CDisabler class=============
//========================================
//    Notes: 
//        -Can disable annoying crap
//        -Only works for functions that are
//          virtual
//        -Should be expanded to hook
//          commands issued through 
//          ClientCmd
//        -Also VAC banned me so nearly
//          everything has been removed
//========================================

void __stdcall Hooked_SetViewAngles(QAngle &a)
{
    QAngle copy;
    copy = a;
    if(CDisabler::m_bDisableSetAngles){
        a = CDisabler::m_pOldCmd->viewangles;
        a.z = 0.0f;
    }
    CDisabler::m_hSetViewAngles->UnHook();
    Skim::ValveInterfaces::pEngine->SetViewAngles(copy);
    CDisabler::m_hSetViewAngles->ReHook();
}
void DisableSetAngles()
{
    CDisabler::m_bDisableSetAngles = true;
}
void EnableSetAngles()
{
    CDisabler::m_bDisableSetAngles = false;
}
CDisabler::CDisabler()
{
    m_pDisableSetang = new ConCommand( "+" PREFIX "disable_setangles", DisableSetAngles);
    m_pEnableSetang = new ConCommand( "-" PREFIX "disable_setangles", EnableSetAngles);

    m_hSetViewAngles = new CVMTHook();
    m_hSetViewAngles->Hook(Hooked_SetViewAngles, Skim::ValveInterfaces::pEngine, 20);
}
using namespace Skim::ValveInterfaces;
CDisabler::~CDisabler()
{
    FreeConCMD(m_pDisableSetang);
    FreeConCMD(m_pEnableSetang);
    m_hSetViewAngles->UnHook();
    delete m_hSetViewAngles;
}
//----------------------------------------
// SetViewAngles get called many times during 1 frame
// Setting viewangles to pcmd's angles will still work
//----------------------------------------
void CDisabler::Run(CUserCmd* pCmd)
{
    m_pOldCmd = pCmd;
}