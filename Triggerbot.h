#pragma once
#include "Required.h"
#include "Interfaces.h"
#include "TF2Player.h"

namespace Skim{
class CTriggerBot : public IHack
{
public:
    CTriggerBot();
    ~CTriggerBot();
    void Run(CUserCmd*);
    bool m_bManualEnabled;
    static void GetInfoFromIndex(int);
    static void LocalPlayerInfo();
    static void AimedPlayerInfo();
    static void GetInfoFromConCMD(const CCommand &args);
private:
    static bool ShouldIShoot(CUserCmd*);
    ConVar *IsEnabled;
    //ConVar *Nospread;
    //ConVar *HitFriends;
    ConCommand *c1,*c2,*c3,*c4,*c5;
};
extern CTriggerBot *g_pTBot;

}