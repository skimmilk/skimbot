#pragma once
#include "Required.h"
#include "Interfaces.h"

#define MAX_COMMAND_SIZE 512
#define EVENTS_KILL 0
#define EVENTS_DEATH 1
#define EVENTS_SUICIDE 2
#define EVENTS_DOMINATION 3
#define EVENTS_REVENGE 4

struct GEHook_t { char evnt[MAX_EVENT_NAME_LENGTH]; char commands[MAX_COMMAND_SIZE]; };

class CGameEventHooks : public IGameEventListener2
{
public:
    CGameEventHooks();
    ~CGameEventHooks();
    virtual void FireGameEvent(IGameEvent*);
    void AddHook(const GEHook_t &a,bool isMe);
    void RemoveHook(const char* command);
    void AddDeathHook(const char* command, int type);
    void RemoveDeathHook(int type);
private:
    void Yell(const char* eventarg, const char* name);
    void Yell(const char* eventarg);
    bool RunDeathCommands(player_info_t *attack, player_info_t *victim);
    inline int GetEntIndexFromID(int);
    std::vector<GEHook_t>m_vHooks;
    //Things that were fired because due to me, fires when
    //"player" equals our ent index or
    //"userid" is our ID
    std::vector<GEHook_t>m_vMeHooks;
    char* killEventArg;
    char* deathEventArg;
    char* suicideEventArg;
    bool hokkill;
    bool hokdeath;
    bool hoksuicide;
    ConCommand *c1,*c2,*c3,*c4,*c5,*c6,*c7,*c8,*c9,*c0;
};
extern CGameEventHooks *g_pGEHooks;