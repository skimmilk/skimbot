#pragma once
#include "Required.h"
#include "IHack.h"
#include <stdstring.h>
#include <vector>

class CNameSteal : public IHack
{
public:
    CNameSteal();
    ~CNameSteal();
    void Run(CUserCmd*);
    void FireGameEvent( IGameEvent *event );
    int m_istalkedindex;
private:
    void UpdateTable();
    static void Stalk(const CCommand& args);
    ConVar *m_pStealDelay;
    ConVar *m_pRunning;
    ConVar *m_pRandNum;
    //ConVar *m_pRandMult;
    ConCommand *m_pStalk;
    bool m_bStalking;
    //int m_itickcount;
    std::vector<player_info_t*> m_vNames;
};

extern CNameSteal *g_pNameSteal;
