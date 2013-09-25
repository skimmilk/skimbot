#include "Triggerbot.h"
#include "TF2.h"
#include "TF2Entity.h"
#include "TraceHelper.h"
//#include "util_shared.h"

namespace Skim{
CTriggerBot *g_pTBot;

using namespace Skim::ValveInterfaces;
//----------------------------------------
//---------Main running function----------
//----------------------------------------
void CTriggerBot::Run(CUserCmd *pCmd)
{
    if
    (((IsEnabled->GetBool()) || m_bManualEnabled) &&
    ShouldIShoot(pCmd))
        pCmd->buttons |= IN_ATTACK;
}
bool CTriggerBot::ShouldIShoot(CUserCmd* pCmd)
{
    TF2Player* pBaseEntMe = Skim::TF2Entity::Me();

    int aimIndex = TraceHelper::GetAimIndex(pBaseEntMe->vecEyeOrigin(), pCmd->viewangles, pEngine->GetLocalPlayer()); 
    if(!aimIndex)
        return false;

    IClientEntity* pClientEntAimed = ValveInterfaces::pClientEntList->GetClientEntity( aimIndex );
    TF2Player* pBaseEntAimed = (TF2Player*)pClientEntAimed->GetBaseEntity();

    if((pBaseEntAimed->iTeamNum() == 2 || pBaseEntAimed->iTeamNum() == 3) &&pBaseEntAimed->iTeamNum() != pBaseEntMe->iTeamNum())
        return true;
    return false;
}
//----------------------------------------
//----Functions called by concommands-----
//----------------------------------------
void TriggerbotTurnOn()
{
    g_pTBot->m_bManualEnabled = true;
}
void TriggerbotTurnOff()
{
    g_pTBot->m_bManualEnabled = false;
}

//----------------------------------------
//---------------Debuggery----------------
//----------------------------------------
//    Notes:    
//        Save this here since I might be
//        able to use this for things like
//        getting useful info and checking
//        offsets
//----------------------------------------
void CTriggerBot::GetInfoFromIndex(int index)
{
    if(!index)
        return;
    IClientEntity* pClientEnt = ValveInterfaces::pClientEntList->GetClientEntity( index );
    if( !pClientEnt)
        return;
    TF2Player* pPlayer = (TF2Player*)pClientEnt->GetBaseEntity();

    player_info_t * pPlInfo = Skim::TF2Entity::GetInfo(pPlayer);

    Msg("Base ent:      0x%p\n", pPlayer);
    Msg("Entity index:  %i\n", pPlayer->index());
    Msg("Position:      %f, %f, %f\n", pPlayer->vecOrigin().x, pPlayer->vecOrigin().y, pPlayer->vecOrigin().z);
    Msg("Eye position:  %f, %f, %f\n", pPlayer->vecEyeOrigin().x, pPlayer->vecEyeOrigin().y, pPlayer->vecEyeOrigin().z);
    Msg("Eye angles:    %f, %f, %f\n",pPlayer->angEyeAngles().x, pPlayer->angEyeAngles().y, pPlayer->angEyeAngles().z);
    Msg("Player height: %f\n", pPlayer->flPlayerHeight());
    Msg("Health:        %i\n", pPlayer->iHealth());
    Msg("TeamNum:       %i\n", pPlayer->iTeamNum());
    Msg("Player class:  %i\n", pPlayer->playerClass());
    Msg("\n");
    // bit flags
    char demflags[33];

    itoa(pPlayer->nPlayerState(), demflags, 2);
    Msg("Player state:  %s\n", demflags);

    itoa(pPlayer->nPlayerCond(), demflags, 2);
    Msg("Player cond:   %s\n", demflags);

    itoa(pPlayer->fFlags(), demflags, 2);
    Msg("Player flags:  %s\n", demflags);

    itoa(pPlayer->iStunFlags(), demflags, 2);
    Msg("Stun flags:    %s\n", demflags);

    itoa(pPlayer->lifeState(), demflags, 2);
    Msg("Life state:    %s\n", demflags);
    
    Msg("\n");
    Msg("Cloak meter:   %f\n", pPlayer->flCloakMeter());
    Msg("Drink meter:   %f\n", pPlayer->flEnergyDrinkMeter());
    Msg("Charge meter:  %f\n", pPlayer->flChargeMeter());
    Msg("Revenge crits: %i\n", pPlayer->iRevengeCrits());
    Msg("Disguise targ: %i\n", pPlayer->iDisguiseTargetIndex());
    Msg("Disguise class: %i\n", pPlayer->nDisguiseClass());
    Msg("Is disguised:  %i\n", pPlayer->isDisguised());
    Msg("Item ptr:      0x%p\n", pPlayer->hItem());

    Msg("\n");

    Msg("Name:          %s\n", pPlInfo->name);
    Msg("User ID:       %i\n", pPlInfo->userID);
    Msg("Friend ID:     %u\n", pPlInfo->friendsID);
}
void CTriggerBot::LocalPlayerInfo()
{
    GetInfoFromIndex(pEngine->GetLocalPlayer());
}
void CTriggerBot::AimedPlayerInfo()
{
    TF2Player* pBaseEntMe = Skim::TF2Entity::Me();

    int zuhzuhzuh = TraceHelper::GetAimIndex(pBaseEntMe, pEngine->GetLocalPlayer());
    if(!zuhzuhzuh){
        Msg("Point at something first, %s.\n", Skim::TF2Entity::GetInfo(Skim::TF2Entity::Me())->name);
        return;
    }
    GetInfoFromIndex(zuhzuhzuh);
}
void CTriggerBot::GetInfoFromConCMD(const CCommand &args)
{
    if (args.ArgC() == 0 || strlen(args.Arg(1)) == 0)
        return;
    GetInfoFromIndex(atoi(args.Arg(1)));
}

CTriggerBot::CTriggerBot()
{
    IsEnabled = new ConVar(PREFIX "tbot_autoenabled", "0", 0, "Will automatically fire dose gunz");
    //Nospread = new ConVar(PREFIX "tbot_nospread", "1", 0, "Factors gun spread in prediction");
    //HitFriends = new ConVar(PREFIX "tbot_hitfriends", "1", 0, "Will or will not hit people on your steam friends list");
    c1 = new ConCommand("+" PREFIX "tbot", TriggerbotTurnOn);
    c2 = new ConCommand("-" PREFIX "tbot", TriggerbotTurnOff);
//#ifdef _DEBUG
    c3 = new ConCommand(PREFIX "dev_playerinfo_local", CTriggerBot::LocalPlayerInfo);
    c4 = new ConCommand(PREFIX "dev_playerinfo_pointed", CTriggerBot::AimedPlayerInfo);
    c5 = new ConCommand(PREFIX "dev_playerinfo_index", CTriggerBot::GetInfoFromConCMD);
//#endif
    m_bManualEnabled = false;
    g_pTBot = this;
}
CTriggerBot::~CTriggerBot()
{
    FreeConCMD(IsEnabled);
    FreeConCMD(c1);
    FreeConCMD(c2);
    FreeConCMD(c3);
    FreeConCMD(c4);
    FreeConCMD(c5);
}
}