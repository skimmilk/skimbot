#pragma once
#include "Required.h"
#include "CvarTools.h"
#include "IHack.h"

class FastCharger : public IHack
{
public:
    FastCharger();
    ~FastCharger();
    void Run(CUserCmd *pCmd);
    bool m_bRun;
    static FastCharger *g_pInstance;
private:
    static void TurnOff();
    static void TurnOn();
    inline void SetSpeedhack(bool);
    bool isSpeeding;
    ConVar *ShouldRun;
    ConVar *TargetTimescale;
    ConVar *CurrentClass;
    ConCommand *c1, *c2;
};