#include "Required.h"
#include "IHack.h"
#include "TF2Entity.h"
#include "Drawing.h"

class CRadar : public IHack
{
public:
    CRadar();
    ~CRadar();

    void Run_PaintTraverse();

private:
    void DrawRadarBG(int posx, int posy, int sizex, int sizey);
    void DrawPlayerDot(TF2Player*, const Color&);
    void DrawPlayerViewline(TF2Player* plyr, float dist, const Color&);
    bool WorldVecToRadar(const Vector& vec, Drawing::CartPt&);

    ConVar* m_pEnabled;
    ConVar* m_pDrawDist;
    ConVar* m_pPosX;
    ConVar* m_pPosY;
    ConVar* m_pSizeX;
    ConVar* m_pSizeY;
    ConVar* m_pAltFriendColor;
    ConVar* m_pShowAimbotted;
    ConVar* m_pBgMap;
    ConVar* m_pDrawSameTeam;
    ConVar* m_pLookDir;
    char* m_sMapName;
    int m_iBgImageID;
};