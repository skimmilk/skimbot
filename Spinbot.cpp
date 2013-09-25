#include "Spinbot.h"
#include "AimAndMovementHelper.h"
#include "Interfaces.h"

using namespace Skim;
CSpinBot *CSpinBot::g_pInstance;

//========================================
// Static functions called by concommmands
//========================================
void SBSetAng(const CCommand &args)
{
    if(args.ArgC() != 2)
    {
        Msg("Usage: skim_spin_angle x y");
        return;
    }
    CSpinBot::g_pInstance->PutAng(args);
}
// Its helper
void CSpinBot::PutAng(const CCommand &args)
{

}

//========================================
//========Constructor & Destructor========
//========================================
CSpinBot::CSpinBot()
{
    m_enabled = new ConVar(PREFIX "spin", "0", 0, "Set at 1 for spin, 2 for reverse, 3 for set specific angle");
    m_spinAmount = new ConVar(PREFIX "spin_factor", "12", 0, "Angle you spin every tick");
    m_stopOnA1 = new ConVar(PREFIX "spin_stopon_a1", "1", 0, "Stops spinning when you use your attack");
    m_stopOnA2 = new ConVar(PREFIX "spin_stopon_a2", "0", 0, "Stops spinning when you use your attack 2");
    c1 = new ConCommand(PREFIX "spin_angle", SBSetAng, "Sets your angles to this");
    m_bRunning = false;
    m_aOldAngle = QAngle(0,0,0);
}
CSpinBot::~CSpinBot()
{
    FreeConCMD(m_enabled);
    FreeConCMD(m_spinAmount);
    FreeConCMD(m_stopOnA1);
    FreeConCMD(m_stopOnA2);
    FreeConCMD(c1);
}

//========================================
//=======Figures out if we go or no=======
//========================================
inline bool CSpinBot::CanGo(CUserCmd* pCmd)
{
    if(!(m_enabled->GetInt())
    || (m_stopOnA1->GetBool() && pCmd->buttons & IN_ATTACK)
    || (m_stopOnA2->GetBool() && pCmd->buttons & IN_ATTACK2))
        return false;
    return true;
}

//========================================
//========Functions from basehack=========
//========================================
void CSpinBot::Run(CUserCmd* pCmd)
{
    if(CanGo(pCmd))
    {
        m_aOldAngle = pCmd->viewangles;
        m_bRunning = true;

        if(m_enabled->GetInt() == 1)
            Spin(pCmd);
        else if(m_enabled->GetInt() == 2)
            Reverse(pCmd);
        else if(m_enabled->GetInt() == 3)
            SetAng(pCmd);

        CSpinBot::m_pOldCmd = pCmd;
    }
}
void CSpinBot::Run_PaintTraverse()
{
    // I added the paint traverse hook thinking changing pCmd viewangles changes view angles you see ;[
}
void CSpinBot::Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active){}

//========================================
//========Set angles on createmove========
//========================================
void CSpinBot::Spin(CUserCmd* pCmd)
{
    float spin = pCmd->tick_count * m_spinAmount->GetInt() % 360;// + pCmd->viewangles.y;
    
    Skim::AimAndMovementHelper::SetAngles(pCmd->forwardmove, pCmd->sidemove, spin, pCmd->viewangles.y);

    pCmd->viewangles.y = spin;
}
void CSpinBot::Reverse(CUserCmd* pCmd)
{
    //pCmd->viewangles = QAngle(pCmd->viewangles.x, pCmd->viewangles.y + 180 % 360, 0);
    pCmd->viewangles = QAngle(pCmd->viewangles.x + 180 % 360, pCmd->viewangles.y, 0);
    pCmd->forwardmove *= -1;
    //pCmd->sidemove *= -1;
}
void CSpinBot::SetAng(CUserCmd* pCmd)
{
    Skim::AimAndMovementHelper::SetAngles(pCmd->forwardmove, pCmd->sidemove, m_aSetAngle.y, pCmd->viewangles.y);
    pCmd->viewangles = QAngle(m_aSetAngle.x, m_aSetAngle.y, 0);
}
