#include "TF2PlayerTools.h"
#include "TraceHelper.h"

namespace Skim {


//----------------------------------------
//----------Constructor and stuff---------
//----------------------------------------
CTF2PlayerTools* CTF2PlayerTools::g_pTF2PlayerTools;
CTF2PlayerTools::CTF2PlayerTools()
{
    ZeroMemory(m_iDormantPeriods, sizeof(int) * ARRAY_LENGH);
    g_pTF2PlayerTools = this;
    m_bCanPredict = false;
}
void CTF2PlayerTools::Run(CUserCmd*)
{
    static int runover = 0;
    int length = ValveInterfaces::pEngine->GetMaxClients() + 1;
    for (int i = 1; i < length; i++)
    {
        IClientEntity* clEnt = ValveInterfaces::pClientEntList->GetClientEntity(i);
        if (!clEnt)
            continue;
        if (clEnt->IsDormant())
            m_iDormantPeriods[i]++;
        else 
            m_iDormantPeriods[i] = 0;

        memcpy(vecPast2, vecPast1, sizeof vecPast1);
        memcpy(vecPast1, vecCurrent, sizeof vecCurrent);
        vecCurrent[i] = TF2Entity::GetPlayer(clEnt->entindex())->vecOrigin();
    }
    runover++;
    if (runover == 3)
        m_bCanPredict = true;
}
void CTF2PlayerTools::Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active)
{
    m_flFrameTime = input_sample_frametime;
}

//----------------------------------------
//-------Determining dormancy period------
//----------------------------------------
int CTF2PlayerTools::PlayerDormantPeriod(int index)
{
    return g_pTF2PlayerTools->m_iDormantPeriods[index];
}
int CTF2PlayerTools::PlayerDormantPeriod(TF2Player* a)
{
    return g_pTF2PlayerTools->m_iDormantPeriods[a->index()];
}

//----------------------------------------
//-Stuff dealing with a player's velocity-
//----------------------------------------
inline Vector CTF2PlayerTools::PlayerVelocity(TF2Player *w)
{
    return PlayerVelocity(w->index());
}
Vector CTF2PlayerTools::PlayerVelocity(int index)
{
    if (!g_pTF2PlayerTools->m_bCanPredict)
        return Vector(0,0,0);
    // average over 3 frames
    float x = (
        g_pTF2PlayerTools->vecCurrent[index].x +
        g_pTF2PlayerTools->vecPast1[index].x +
        g_pTF2PlayerTools->vecPast2[index].x 
        ) * 0.3333333f;
    float y = (
        g_pTF2PlayerTools->vecCurrent[index].y +
        g_pTF2PlayerTools->vecPast1[index].y +
        g_pTF2PlayerTools->vecPast2[index].y
        ) * 0.3333333f;
    float z = (
        g_pTF2PlayerTools->vecCurrent[index].z +
        g_pTF2PlayerTools->vecPast1[index].z +
        g_pTF2PlayerTools->vecPast2[index].z
        ) * 0.3333333f;
    return Vector(x,y,z);
}
inline Vector CTF2PlayerTools::PredictPlayerLoc(TF2Player* plyr, float seconds)
{
    return PredictPlayerLoc(plyr, seconds / g_pTF2PlayerTools->m_flFrameTime);
}
Vector CTF2PlayerTools::PredictPlayerLoc(TF2Player* jimmies, int frames)
{
    Vector velocity = PlayerVelocity(jimmies);
    Vector origin = jimmies->vecOrigin();
    return Vector(
        velocity.x * frames + origin.x, 
        velocity.y * frames + origin.y,
        velocity.z * frames + origin.z );
}

//----------------------------------------
//--Determining if a player is on ground--
//----------------------------------------
//Will only work if player is on even ground
bool CTF2PlayerTools::PlayerOnGround(TF2Player* p)
{
    //length will be 2 if not hit, 1 if on ground
    trace_t tr;
    const static Vector oneup(0,0,2);
    TraceHelper::TraceLine(p->vecOrigin() + oneup, p->vecOrigin() - oneup, tr, MASK_PLAYERSOLID);
    return ((tr.startpos - tr.endpos).z < 3.0f);
}
};