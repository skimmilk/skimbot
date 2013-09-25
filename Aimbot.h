#pragma once
#include "Required.h"
#include "IHack.h"

class CAimbot : public IHack
{
public:
    CAimbot();
    ~CAimbot();

    //void Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active);
    void Run(CUserCmd *cmd);
    //void Run_PaintTraverse();

    bool m_bRunning;

    static int m_iIndexAimed;

private:
    ConVar* m_pRunAttack;
    ConVar* m_pRunAttack2;
    ConVar* m_pSilentAim;
    ConVar* m_pAltKnifeAim;
    ConVar* m_pAutoshoot;
    ConVar* m_pEspAimed;
    ConVar* m_pAimBodypart;
    ConVar* m_pAttackAimed;
    ConCommand* m_pStartAim;
    ConCommand* m_pEndAim;
};
extern CAimbot *g_pAimbot;