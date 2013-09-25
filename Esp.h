#pragma once
#include "Required.h"
#include "IHack.h"
#include "TF2Entity.h"

class CEsp : public IHack
{
public:
    CEsp();
    ~CEsp();

    void Run_PaintTraverse();

    void DrawPlayerEsp(TF2Player*);
private:
    ConVar* m_pBones;
    ConVar* m_pBBox;
    ConVar* m_pName;
    ConVar* m_pHealth;
    ConVar* m_pHealthBar;
    ConVar* m_pEsp;
    ConVar* m_pDrawDist;
    ConVar* m_pMaxDist;
    ConVar* m_pDrawDistCrosshair;
    ConVar* m_pAltFriendColor;
    ConVar* m_pWpnName;
    ConVar* m_pShowLocal;
    ConVar* m_pShowAimbotted;
    ConVar* m_pDrawSameTeam;
    ConVar* m_pLookDir;
    ConVar* m_pDetailDist;
    //ConVar* m_pSpecShow;
    //ConVar* m_pSpecX;
    //ConVar* m_pSpecY;

    ConCommand* m_pReinitFont;
}; 