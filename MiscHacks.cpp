#include "MiscHacks.h"
#include "TF2Offsets.h"
#include "TF2Entity.h"
#include "TF2PlayerTools.h"

CMiscHacks* g_pMiscHacks;
ConVar* CMiscHacks::host_framerate;
ConVar* CMiscHacks::host_timescale;
void InAttack()
{
    g_pMiscHacks->m_bAttack1 = true;
}
void OutAttack()
{
    g_pMiscHacks->m_bAttack1 = false;
}
void InAttack2()
{
    g_pMiscHacks->m_bAttack2 = true;
}
void OutAttack2()
{
    g_pMiscHacks->m_bAttack2 = false;
}

void IForw(){
    g_pMiscHacks->m_bforward = true;
}
void IBack(){
    g_pMiscHacks->m_bback = true;
}
void IRigh(){
    g_pMiscHacks->m_bright = true;
}
void ILeft(){
    g_pMiscHacks->m_bleft = true;
}
void OForw(){
    g_pMiscHacks->m_bforward = false;
}
void OBack(){
    g_pMiscHacks->m_bback = false;
}
void ORigh(){
    g_pMiscHacks->m_bright = false;
}
void OLeft(){
    g_pMiscHacks->m_bleft = false;
}
void IJump(){
    g_pMiscHacks->m_bjump = true;
}
void OJump(){
    g_pMiscHacks->m_bjump = false;
}

void CMiscHacks::InSpeed()
{
    CMiscHacks::host_framerate->SetValue(g_pMiscHacks->m_pSpeedAmt->GetFloat());
}
void CMiscHacks::OutSpeed()
{
    CMiscHacks::host_framerate->SetValue(0.f);
}

CMiscHacks::CMiscHacks()
{
    g_pMiscHacks = this;
    m_pSlidingTaunt = new ConVar( PREFIX "misc_slidingtaunt", "0",0, "Slide n stuff");
    m_pForceAttacks = new ConVar( PREFIX "misc_forceattacks", "0",0,"Good for taunting and attacking");
    m_pBhop = new ConVar( PREFIX "misc_bhop", "1", 0, "Jump n stuff");
    m_pForceAngles = new ConVar( PREFIX "misc_forceangles", "0",0, "Good for taunting and attacking");

    m_pInAttack = pCvar->FindCommand("+attack");
    m_pOutAttack = pCvar->FindCommand("-attack");
    m_pInAttack2 = pCvar->FindCommand("+attack2");
    m_pOutAttack2 = pCvar->FindCommand("-attack2");

    m_pIForw = pCvar->FindCommand("+forward");
    m_pOForw = pCvar->FindCommand("-forward");
    //"+forward"

    m_pIBack = pCvar->FindCommand("+back");
    m_pOBack = pCvar->FindCommand("-back");

    m_pIRigh = pCvar->FindCommand("+moveright");
    m_pORigh = pCvar->FindCommand("-moveright");

    m_pILeft = pCvar->FindCommand("+moveleft");
    m_pOLeft = pCvar->FindCommand("-moveleft");

    m_pIJump = pCvar->FindCommand("+jump");
    m_pOJump = pCvar->FindCommand("-jump");

    m_fnInAttackOrig = m_pInAttack->m_fnCommandCallbackV1;
    m_fnOutAttackOrig = m_pOutAttack->m_fnCommandCallbackV1;
    m_fnInAttack2Orig = m_pInAttack2->m_fnCommandCallbackV1;
    m_fnOutAttack2Orig = m_pOutAttack2->m_fnCommandCallbackV1;

    m_fnIForw = m_pIForw->m_fnCommandCallbackV1;
    m_fnOForw = m_pOForw->m_fnCommandCallbackV1;

    m_fnIBack = m_pIBack->m_fnCommandCallbackV1;
    m_fnOBack = m_pOBack->m_fnCommandCallbackV1;

    m_fnIRigh = m_pIRigh->m_fnCommandCallbackV1;
    m_fnORigh = m_pORigh->m_fnCommandCallbackV1;

    m_fnILeft = m_pILeft->m_fnCommandCallbackV1;
    m_fnOLeft = m_pOLeft->m_fnCommandCallbackV1;

    m_fnIJump = m_pIJump->m_fnCommandCallbackV1;
    m_fnOJump = m_pOJump->m_fnCommandCallbackV1;

    m_bAttack1 = false;
    m_bAttack2 = false;
    m_bAttack1 = false;
    m_bAttack2 = false;
    m_bforward = false;
    m_bback = false;
    m_bleft = false;
    m_bright = false;
    m_bjump = false;
    m_bPatchedAttacks = false;
    m_bPatchedMovement = false;

    host_framerate = pCvar->FindVar("host_framerate");
    host_timescale = pCvar->FindVar("host_timescale");
    m_pSpeedAmt = new ConVar(PREFIX"misc_speed_desired", "10");
    m_pInSpeed = new ConCommand("+"PREFIX"misc_speed",CMiscHacks::InSpeed);
    m_pOutSpeed = new ConCommand("-"PREFIX"misc_speed",CMiscHacks::OutSpeed);
}

CMiscHacks::~CMiscHacks()
{
    //this gives off a heap corruption error in debug mode
    //lolwhy
    FreeConCMD(m_pSlidingTaunt);
    FreeConCMD(m_pForceAttacks);
    FreeConCMD(m_pBhop);
    FreeConCMD(m_pForceAngles);
    UnPatchAttacks();
    UnPatchMoveKeys();
}

void CMiscHacks::UnPatchAttacks()
{
    m_pInAttack->m_fnCommandCallbackV1 = m_fnInAttackOrig;
    m_pOutAttack->m_fnCommandCallbackV1 = m_fnOutAttackOrig;
    m_pInAttack2->m_fnCommandCallbackV1 = m_fnInAttack2Orig;
    m_pOutAttack2->m_fnCommandCallbackV1 = m_fnOutAttack2Orig;
}
void CMiscHacks::UnPatchMoveKeys()
{
    m_pIForw->m_fnCommandCallbackV1 = m_fnIForw;
    m_pOForw->m_fnCommandCallbackV1 = m_fnOForw ;

    m_pIBack->m_fnCommandCallbackV1 = m_fnIBack ;
    m_pOBack->m_fnCommandCallbackV1 = m_fnOBack ;

    m_pIRigh->m_fnCommandCallbackV1 = m_fnIRigh ;
    m_pORigh->m_fnCommandCallbackV1 = m_fnORigh ;

    m_pILeft->m_fnCommandCallbackV1 = m_fnILeft ;
    m_pOLeft->m_fnCommandCallbackV1 = m_fnOLeft ;

    m_pIJump->m_fnCommandCallbackV1 = m_fnIJump;
    m_pOJump->m_fnCommandCallbackV1 = m_fnOJump;
}

void CMiscHacks::Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active)
{
    //http://hg.alliedmods.net/releases/sourcemod-1.3/file/tip/plugins/include/tf2.inc
    //TFCond_Taunting = 7
    //if (m_pSlidingTaunt->GetBool()) {
    //    int* asdf = (int*)Skim::TF2Entity::Me() + BaseEnt::shared + TFPlayerShared::nPlayerCond;
    //    *asdf = *asdf & ~(1<<7);
    //}
}
void CMiscHacks::Run(CUserCmd* pCmd)
{
    if (!m_pSlidingTaunt->GetBool() && this->m_bPatchedMovement)
    {
        UnPatchMoveKeys();
        m_bPatchedMovement = false;
    }
    else if (m_pSlidingTaunt->GetBool() && !m_bPatchedMovement)
    {
        m_pIForw->m_fnCommandCallbackV1 = IForw;
        m_pOForw->m_fnCommandCallbackV1 = OForw;

        m_pIBack->m_fnCommandCallbackV1 = IBack;
        m_pOBack->m_fnCommandCallbackV1 = OBack;

        m_pIRigh->m_fnCommandCallbackV1 = IRigh;
        m_pORigh->m_fnCommandCallbackV1 = ORigh;

        m_pILeft->m_fnCommandCallbackV1 = ILeft;
        m_pOLeft->m_fnCommandCallbackV1 = OLeft;

        m_pIJump->m_fnCommandCallbackV1 = IJump;
        m_pOJump->m_fnCommandCallbackV1 = OJump;
    }
    // We do not want patched attacks but the attacks are patched
    if (!m_pForceAttacks->GetBool() && m_bPatchedAttacks)
    {
        UnPatchAttacks();
        m_bPatchedAttacks = false;
    }
    // We want patched attacks but the attacks are not patched
    else if (m_pForceAttacks->GetBool() && !m_bPatchedAttacks)
    {
        m_pInAttack->m_fnCommandCallbackV1 = InAttack;
        m_pOutAttack->m_fnCommandCallbackV1 = OutAttack;
        m_pInAttack2->m_fnCommandCallbackV1 = InAttack2;
        m_pOutAttack2->m_fnCommandCallbackV1 = OutAttack2;
        m_bPatchedAttacks = true;
    }

    if (m_pForceAttacks->GetBool())
    {
        if (m_bAttack1)
            pCmd->buttons |= IN_ATTACK;
        //else
        //    pCmd->buttons &= ~IN_ATTACK;
        if (m_bAttack2)
            pCmd->buttons |= IN_ATTACK2;
        //else
        //    pCmd->buttons &= IN_ATTACK2;
    }
    if (m_pSlidingTaunt->GetBool())
    {
        if(m_bforward)
            pCmd->forwardmove = 500.0f;
        else if(m_bback)
            pCmd->forwardmove = -500.0f;
        if(m_bright)
            pCmd->sidemove = 500.0f;
        else if(m_bleft)
            pCmd->sidemove = -500.0f;
        if(m_bjump)
            pCmd->buttons |= IN_JUMP;
    }

    if (m_pBhop->GetBool())
    {
        //manualJump = true when we released
        static bool manualJump = false;
        if (pCmd->buttons & IN_JUMP)
        {
            if (!manualJump){
                if (!CTF2PlayerTools::PlayerOnGround(TF2Entity::Me()))
                    pCmd->buttons &= ~IN_JUMP;
            }
            else //Don't unset jump button because on ground
                manualJump = false;
        }
        else //Don't unset jump button because it was released
            manualJump = true;
    }

    if (m_pForceAngles->GetBool()){
        QAngle q;
        pEngine->GetViewAngles(q);
        pCmd->viewangles = q;
    }
}