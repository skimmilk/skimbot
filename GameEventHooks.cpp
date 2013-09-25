#include "GameEventHooks.h"
//"player" is ent index
//"userid" is ID, short (int16)
//"attacker" is ID, short
CGameEventHooks *g_pGEHooks;

void HookGameEvent(const CCommand &args)
{
    if ( args.ArgC() < 2 || strlen(args.Arg(2)) == 0)
    {
        Msg("%s Usage: skim_hooks_hook <event> <commands to run>\n", PLUGIN_NAME);
        return;
    }
    GEHook_t hok = GEHook_t();
    strcpy(hok.evnt, args.Arg(1));
    strcpy(hok.commands, args.Arg(1));
    g_pGEHooks->AddHook(hok,false);
}
void UnhookGameEvent(const CCommand &args)
{
    if ( args.ArgC() < 1 || strlen(args.Arg(1)) == 0)
    {
        Msg("%s Usage: skim_hooks_unhook <event>\n", PLUGIN_NAME);
        return;
    }
    Msg("%sNot implemented", PLUGIN_NAME);

}

void HookGameEventMeOnly(const CCommand &args)
{
    if ( args.ArgC() < 2 || strlen(args.Arg(2)) == 0)
    {
        Msg("%s Usage: skim_hook <event> <commands to run>\n", PLUGIN_NAME);
        return;
    }

    GEHook_t hok = GEHook_t();
    strcpy(hok.evnt, args.Arg(1));
    strcpy(hok.commands, args.Arg(1));
    g_pGEHooks->AddHook(hok,true);

}
void UnhookGameEventMeOnly(const CCommand &args)
{
    if ( args.ArgC() < 1 || strlen(args.Arg(1)) == 0)
    {
        Msg("%s Usage: skim_hooks_unhook <event>\n", PLUGIN_NAME);
        return;
    }
    Msg("%sNot implemented", PLUGIN_NAME);

}

void HookKillEvent(const CCommand &args)
{
    if ( args.ArgC() < 1 || strlen(args.Arg(1)) == 0)
    {
        Msg("%s Usage: skim_hook_kill <command>\n", PLUGIN_NAME);
        return;
    }
    g_pGEHooks->AddDeathHook(args.Arg(1), EVENTS_KILL);
}
void HookDeathEvent(const CCommand &args)
{
    if ( args.ArgC() < 1 || strlen(args.Arg(1)) == 0)
    {
        Msg("%s Usage: skim_hook_death <command>\n", PLUGIN_NAME);
        return;
    }
    g_pGEHooks->AddDeathHook(args.Arg(1), EVENTS_DEATH);
}
void HookSuicideEvent(const CCommand &args)
{
    if ( args.ArgC() < 1 || strlen(args.Arg(1)) == 0)
    {
        Msg("%s Usage: skim_hook_death <command>\n", PLUGIN_NAME);
        return;
    }
    g_pGEHooks->AddDeathHook(args.Arg(1), EVENTS_SUICIDE);
}

void UnhookKillEvent(){
    g_pGEHooks->RemoveDeathHook(EVENTS_KILL);
}
void UnhookDeathEvent(){
    g_pGEHooks->RemoveDeathHook(EVENTS_DEATH);
}
void UnhookSuicideEvent(){
    g_pGEHooks->RemoveDeathHook(EVENTS_SUICIDE);
}

using namespace Skim;
using namespace Skim::ValveInterfaces;
inline bool IsSuicide(bool victimMe, int attackerid)
{
    //Attackerid can be 0(world) or attackers entindex can equal ours
    return ( victimMe && ( !attackerid || (pEngine->GetPlayerForUserID(attackerid) == pEngine->GetLocalPlayer() ) ) );
}
bool CGameEventHooks::RunDeathCommands(player_info_t *attack, player_info_t *victim)
{
    player_info_t *local = new player_info_t();
    pEngine->GetPlayerInfo(pEngine->GetLocalPlayer(), local);

    bool victimMe = local->userID == victim->userID;
    bool killerMe = local->userID == attack->userID;
    int attackerid = attack->userID;

    //if I'm the victim & not the killer, I got killed
    //if I'm the killer & not the victim, I killed
    //if the victim and killer id match ours, I suicided

    //(victim, not killer, hooked death, not world death) OR (not hooking suicide and is suicide)
    if((victimMe && !killerMe && hokdeath && !(attack->userID)) || (!hoksuicide && IsSuicide(victimMe, attackerid)))
        Yell( deathEventArg, attack->name);
    else if (killerMe && !victimMe && hokkill)
        Yell( killEventArg, victim->name);
    else if (hoksuicide && IsSuicide(victimMe, attackerid))
        Yell( suicideEventArg );
    
    delete local;
    return true;
}
void CGameEventHooks::Yell(const char* eventArgs, const char* nameToYell)
{
    char* full = new char[MAX_COMMAND_SIZE];
    sprintf(full, eventArgs, nameToYell);
    pEngine->ExecuteClientCmd(full);    
    delete full;
}
void CGameEventHooks::Yell(const char* eventArgs)
{
    pEngine->ExecuteClientCmd(eventArgs);
}

void CGameEventHooks::FireGameEvent(IGameEvent* vent)
{
    using namespace Skim::ValveInterfaces;

    const char* eventname = vent->GetName();

    if(!strcmp("player_death", eventname) 
    && !(vent->GetBool("feign_death")))
    {
        int attacker = vent->GetInt("attacker");
        player_info_t *att = new player_info_t();
        pEngine->GetPlayerInfo(pEngine->GetPlayerForUserID(attacker), att);

        int victim_entindex = vent->GetInt("victim_entindex");
        player_info_t *victim = new player_info_t();
        pEngine->GetPlayerInfo(victim_entindex, victim);

        RunDeathCommands(att, victim);

        delete att;
        delete victim;
        return;//assuming not hooking player_death outside of this
    }

    player_info_t *loc = new player_info_t();
    pEngine->GetPlayerInfo(pEngine->GetLocalPlayer(),loc);

    for(int i = m_vMeHooks.size(); i > 0; i--) {
        if(    !strcmp(eventname, m_vMeHooks[i].evnt) &&
            ((vent->GetInt("player") == pEngine->GetLocalPlayer()) || (vent->GetInt("userid") == loc->userID)))
        {
            pEngine->ExecuteClientCmd(m_vMeHooks[i].commands);
            delete loc;
            return;
        }
    }
    delete loc;

    for(int i = m_vHooks.size(); i > 0; i--) {
        if(    !strcmp(eventname, m_vHooks[i].evnt))
        {
            pEngine->ExecuteClientCmd(m_vHooks[i].commands);
            return;
        }
    }
}

CGameEventHooks::CGameEventHooks()
{
    c1 = new ConCommand(PREFIX "hook", HookGameEvent,
        "Makes the specified command run when the specified hook fires\nUsage: skim_hook <hook name> <command>\nExample: skim_hook tf_game_over \"say shit\"");
    c2 = new ConCommand(PREFIX "hooks_unhook", UnhookGameEvent, "Unregisters the hook");

    c3 = new ConCommand(PREFIX "hook_eventbyme", HookGameEventMeOnly, 
        "Hooks an event and fires the command if the event was caused by me\ni.e. skim_hooks_hook_eventbyme player_death \"say shit\"");
    c4 = new ConCommand(PREFIX "hooks_unhook_eventbyme", UnhookGameEventMeOnly);

    c5 = new ConCommand(PREFIX "hook_kill", HookKillEvent, 
        "Fires the specified command when you kill someone\nExample with printf: skim_hooks_hook_kill \"say I just owned % s\" (no space between % and s)\nReturns I just owned victim name");
    c6 = new ConCommand(PREFIX "hooks_unhook_kill", UnhookKillEvent);

    c7 = new ConCommand(PREFIX "hook_death", HookDeathEvent, 
        "Fires when you die\nExample with printf: skim_hooks_hook_death \"say % s pwnd me\" (no space between % and s)\nReturns killer name pwnd me");
    c8 = new ConCommand(PREFIX "hooks_unhook_death", UnhookDeathEvent);

    c9 = new ConCommand(PREFIX "hook_suicide", HookSuicideEvent, "By you or world");
    c0 = new ConCommand(PREFIX "hooks_unhook_suicide", UnhookSuicideEvent);

    g_pGEHooks = this;
    deathEventArg = 0;
    killEventArg = 0;
    suicideEventArg = 0;
}

CGameEventHooks::~CGameEventHooks(){
    pGameEventManager->RemoveListener(this);
    FreeConCMD(c1);
    FreeConCMD(c2);
    FreeConCMD(c3);
    FreeConCMD(c4);
    FreeConCMD(c5);
    FreeConCMD(c6);
    FreeConCMD(c7);
    FreeConCMD(c8);
    FreeConCMD(c9);
    FreeConCMD(c0);
}

inline void ReadyEventArg(char* argold, const char* arg)
{
    delete argold;
    argold = new char[MAX_COMMAND_SIZE];
    strcpy(argold, arg);
}
void CGameEventHooks::AddDeathHook(const char* command, int type)
{
    pGameEventManager->AddListener(this,"player_death",false);

    if(type == EVENTS_DEATH){
        ReadyEventArg(deathEventArg, command);
        hokdeath = true;
    }
    else if (type == EVENTS_KILL){
        ReadyEventArg(killEventArg, command);
        hokkill = true;
    }
    else if (type == EVENTS_SUICIDE){
        ReadyEventArg(suicideEventArg, command);
        hoksuicide = true;
    }
}

//There is no need to delete the arg, it will be deleted and reassigned at the AddDeathHook function
void CGameEventHooks::RemoveDeathHook(int type)
{
    if(type == EVENTS_DEATH)
        hokdeath = false;
    else if (type == EVENTS_KILL)
        hokkill = false;
    else if (type == EVENTS_SUICIDE)
        hoksuicide = false;
}

void AddHook_DuplicateCheck(std::vector<GEHook_t> &vec, GEHook_t hok)
{
    int size = vec.size();
    for (int i = 0; i < size; i++)
    {
        if(strcmp(vec[i].evnt, hok.evnt))
        {
            strcpy(vec[i].commands, hok.commands);
            return;
        }
    }
    vec.push_back(hok);
}

void CGameEventHooks::AddHook(const GEHook_t &hok, bool isMe)
{
    pGameEventManager->AddListener(this, hok.evnt, false);

    if(isMe)
    {
        AddHook_DuplicateCheck(m_vMeHooks, hok);
        m_vMeHooks.push_back(hok);
    }
    else
    {
        AddHook_DuplicateCheck(m_vHooks, hok);
        m_vHooks.push_back(hok);
    }
}

void RemoveHookToVec(std::vector<GEHook_t> &vec, const char* name)
{
    std::vector<GEHook_t> tmp;
    int size = vec.size();
    for(int i = 0; i < size; i++) {
        if(    strcmp(name, vec[i].evnt) != 0 )//theys different?
        {
            tmp.push_back(vec[i]);
        }
    }

    vec = tmp;
}
void CGameEventHooks::RemoveHook(const char* name)
{
    RemoveHookToVec(m_vMeHooks, name);
    RemoveHookToVec(m_vHooks, name);
}


//"player_death"        // a game event, name may be 32 charaters long
//{
//    // this extends the original player_death 
//    "userid"    "short"       // user ID who died                
//    "victim_entindex"    "long"
//    "inflictor_entindex"    "long"    // ent index of inflictor (a sentry, for example)
//    "attacker"    "short"         // user ID who killed
//    "weapon"    "string"     // weapon name killer used 
//    "weaponid"    "short"        // ID of weapon killed used
//    "damagebits"    "long"        // bits of type of damage
//    "customkill"    "short"        // type of custom kill
//    "assister"    "short"        // user ID of assister
//    "weapon_logclassname"    "string"     // weapon name that should be printed on the log
//    "stun_flags"    "short"    // victim's stun flags at the moment of death
//    "death_flags"    "short" //death flags.
//    "silent_kill"    "bool"
//    "playerpenetratecount"    "short"
//    
////    "dominated"    "short"        // did killer dominate victim with this kill
////    "assister_dominated" "short"    // did assister dominate victim with this kill
////    "revenge"    "short"        // did killer get revenge on victim with this kill
////    "assister_revenge" "short"    // did assister get revenge on victim with this kill
////    "first_blood"    "bool"        // was this a first blood kill
////    "feign_death"    "bool"    // the victim is feign death
//}