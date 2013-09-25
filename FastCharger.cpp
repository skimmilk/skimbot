#include "FastCharger.h"
#include "Interfaces.h"
#include "TF2Player.h"
#include "TF2Entity.h"
#include "TF2Weapon.h"


FastCharger *FastCharger::g_pInstance;
FastCharger::FastCharger()
{
    FastCharger::g_pInstance = this;
    isSpeeding = false;
    m_bRun = false;

    ShouldRun = new ConVar(PREFIX "charger_auto", "0", 0, 
        "Attempts to speed up the charging of the ubercharge, spy cloack, and sniper rifle automatically by speedhacking when you are standing still.");
    TargetTimescale = new ConVar(PREFIX "charger_target_timescale", "10",0, 
        "What the charger will ");
    CurrentClass = new ConVar(PREFIX "currentclass","0");
    new ConCommand("+skim_chargebot",FastCharger::TurnOn,0,0,0);
    new ConCommand("-skim_chargebot",FastCharger::TurnOn,0,0,0);
}
FastCharger::~FastCharger()
{
    FreeConCMD(ShouldRun);
    FreeConCMD(TargetTimescale);
    FreeConCMD(CurrentClass);
    FreeConCMD(c1);
    FreeConCMD(c2);
}

void FastCharger::TurnOff()
{
    FastCharger::g_pInstance->m_bRun = false;
}

void FastCharger::TurnOn()
{
    FastCharger::g_pInstance->m_bRun = true;
}

void FastCharger::SetSpeedhack( bool on )
{
    //rebuild this, valve diabled host_timescale and host_framerate
    if(on)
    {
        isSpeeding = true;

    }
    else
    {
        isSpeeding = false;

    }
}

void FastCharger::Run(CUserCmd* pCmd)
{
    //if(!(Skim::ValveInterfaces::pEngine->IsInGame() &&
    //    (m_bRun || ShouldRun->GetBool()) &&
    //    (
    //        CurrentClass->GetInt() == TF2Class::Medic ||
    //        CurrentClass->GetInt() == TF2Class::Sniper ||
    //        CurrentClass->GetInt() == TF2Class::Spy
    //    )
    //    ))//Should improve getting classes and need to get weapon, especially since I could be a cloaked spy without the CnD
    //    return;

    //using namespace Skim;
    //using namespace Skim::Weapons;

    //TF2Player* pBaseEntMe = Skim::TF2Entity::Me();
    //if(!pBaseEntMe) return;

    ////TF2Weapon* pWeapon = pBaseEntMe->pActiveWeapon();

    //if(pCmd->forwardmove == 0 && pCmd->sidemove == 0)
    //{
    //    if (CurrentClass->GetInt() == TF2Class::Medic){
    //        //check if I am a medic and might be healing(in attack)
    //        if(
    //        (pCmd->weaponselect == TF2WeaponID_t::Medigun || 
    //        pCmd->weaponselect == TF2WeaponID_t::MedigunB || 
    //        pCmd->weaponselect == TF2WeaponID_t::Kritzkrieg) &&
    //        pCmd->buttons & IN_ATTACK){
    //            SetSpeedhack( true );
    //        }
    //        //if(pCmd->buttons & IN_ATTACK){
    //        //    SetSpeedhack( true );
    //        //}
    //        else
    //        {//We are not attacking, speeds is off
    //            SetSpeedhack( false );
    //        }
    //    }
    //    else{
    //        // I am not a medic, I am a sniper or spy
    //        SetSpeedhack( true );
    //    }
    //}
    //else if (isSpeeding){
    //    SetSpeedhack( false );
    //}
}