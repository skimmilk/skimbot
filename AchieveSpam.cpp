#include "AchieveSpam.h"

namespace Skim{
using namespace ValveInterfaces;

//CGameID gameID;
uint numAchievements;

//================================================
//=====Global functions called by concommands=====
//================================================
void AchUnlockAll() {
    AchievementSpam::UnlockAll();
}
void AchUnlockSingle() {
    AchievementSpam::UnlockSingle();
}
void AchRelockAll() {
    AchievementSpam::RelockAll();
}
void AchRelockNamed(const CCommand &args)
{
    if(!strlen(args.Arg(1))){
        Msg("Usage: st_ach_lock <name>\n   Use st_ach_list if you don't know what you're doing\n");
        return;
    }
    AchievementSpam::RelockSingleByName(args.Arg(1));
}
void AchUnlockNamed(const CCommand &args){
    if(!strlen(args.Arg(1))){
        Msg("Usage: st_ach_unlock <name>\n   Use st_ach_list if you don't know what you're doing\n");
        return;
    }
    AchievementSpam::UnlockSingleByName(args.Arg(1));
}
void AchList(){
    AchievementSpam::ListAchievements();
}

//================================================
//=============Small helpful functions============
//================================================
void AchievementSpam::LockAchievement(IAchievement* chie)
{
    pSteamUserStats010->ClearAchievement(chie->GetName());
    pSteamUserStats010->StoreStats();
    pSteamUserStats010->RequestCurrentStats();
}
void AchievementSpam::UnlockSingleByIndex(uint index, bool store)
{
    UnlockAchievement(pIAchievementMgr->GetAchievementByIndex( index ), store);
}
void AchievementSpam::UnlockAchievement(IAchievement* pAch, bool store)
{
    pIAchievementMgr->AwardAchievement( pAch->GetAchievementID() );
    
    if(store){
        Msg("%s Unlocked %s\n",PLUGIN_NAME, pAch->GetName());
        pSteamUserStats010->StoreStats();
    }
}
//Potentially slow, use unlocksinglebyindex or unlockachievement if you can
void AchievementSpam::UnlockSingleByName(const char* name)
{
    //This gives linker error
    //CBaseAchievement *p = pCAchievementMgr->GetAchievementByName(name);
    //pIAchievementMgr->AwardAchievement(p->GetAchievementID());
    for( uint i = 0; i < numAchievements; i++)
    {
        if(!strcmp(pIAchievementMgr->GetAchievementByIndex(i)->GetName(), name))
        {
            if (pIAchievementMgr->GetAchievementByIndex(i)->IsAchieved())
                RelockSingleByName(name);
            UnlockSingleByIndex(i, true);
            return;
        }
    }
    Msg("%s Could not find achievement", PLUGIN_NAME);
}
void AchievementSpam::RelockSingleByName(const char* name)
{
    pSteamUserStats010->ClearAchievement(name);
    pSteamUserStats010->StoreStats();
    pSteamUserStats010->RequestCurrentStats();
}

//================================================
//================****All() stuff=================
//================================================
void AchievementSpam::UnlockAll() 
{ 
    for(uint i = 0; i < numAchievements; i++ )
        UnlockSingleByIndex(i, false);
} 
void AchievementSpam::RelockAll() 
{ 
    for( uint32 i = 0; i < numAchievements; i++ )
        pSteamUserStats010->ClearAchievement( pIAchievementMgr->GetAchievementByIndex(i)->GetName());
    pSteamUserStats010->StoreStats();
    pSteamUserStats010->RequestCurrentStats();
}
void AchievementSpam::ListAchievements()
{
    char full[64];
    for( uint32 i = 0; i < numAchievements; i++ ){
        strcpy(full,pIAchievementMgr->GetAchievementByIndex(i)->GetName());
        Msg("%s\n", full);
    }
}

void AchievementSpam::UnlockSingle()
{
    static uint i = 0;
    IAchievement* pAchievement = pIAchievementMgr->GetAchievementByIndex( i ); 

    while( pAchievement->IsAchieved())
    {
        if (i < numAchievements - 1){
            i++;
            pAchievement = pIAchievementMgr->GetAchievementByIndex(i);
        }
        else{
            i = 0;
            //pAchievement = pIAchievementMgr->GetAchievementByIndex(0);
            Msg("%s Out of achievements to unlock", PLUGIN_NAME);
            return;
        }
    }
    UnlockAchievement(pAchievement, true);
}

//================================================
//==========Loads and unloads this hack===========
//================================================
//Will never access these concommands except when 
//we delete these, that's why they're here
ConCommand *c1, *c2, *c3, *c4, *c5, *c6;
void AchievementSpam::Init()
{
    int i = 0;
    while (true)
    {
        try
        {
            numAchievements = pIAchievementMgr->GetAchievementCount();
            if(!numAchievements)
                continue;
            break;
        }
        catch(...)
        {
            if(i == 10){
                Msg(PLUGIN_NAME " Failed to get achievement manager\n");
                return;
            }
            DevMsg(PLUGIN_NAME " Could not get achievement manager, attempt %i\n", ++i);
            //May have gotten a bad pointer during load, so let's initialize it again
            pIAchievementMgr = pEngine->GetAchievementMgr();
            pCAchievementMgr = dynamic_cast<CAchievementMgr*>(pIAchievementMgr);
            Sleep(1000); 
        }
    }
    c1 = new ConCommand( PREFIX "ach_unlockall", AchUnlockAll, "Unlocks just all of the achievements");
    c2 = new ConCommand( PREFIX "ach_unlocksingle", AchUnlockSingle, "Unlock a single, random achievement");
    c3 = new ConCommand( PREFIX "ach_relockall", AchRelockAll, "Relocks all the achievements");
    c4 = new ConCommand( PREFIX "ach_unlock", AchUnlockNamed, "Will unlock the achievement you entered");
    c5 = new ConCommand( PREFIX "ach_list", AchList, "Lists the name of the achievements for this game");
    c6 = new ConCommand( PREFIX "ach_relock", AchRelockNamed, "Locks an achievement by its name");
}
void AchievementSpam::Shutdown()
{
    FreeConCMD(c1);
    FreeConCMD(c2);
    FreeConCMD(c3);
    FreeConCMD(c4);
    FreeConCMD(c5);
    FreeConCMD(c6);
}
};