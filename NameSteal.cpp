#include "NameSteal.h"
#include "Interfaces.h"
#include "Namer.h"
#include "TF2Entity.h"
#include "SteamFriendTools.h"

using namespace Skim;
using namespace Skim::ValveInterfaces;

CNameSteal *g_pNameSteal;
int t_prevtime;

CNameSteal::~CNameSteal()
{
    FreeConCMD(m_pStealDelay);
    FreeConCMD(m_pRunning);
    FreeConCMD(m_pRandNum);
    FreeConCMD(m_pStalk);
    //FreeConCMD(m_pRandMult);
}

void CNameSteal::Run(CUserCmd* pCmd)
{
    //m_itickcount++;
    int t_now = time( 0 );

    if(!(m_pRunning->GetBool()))// && !m_pStealDelay->GetInt())
        return;

    if(m_bStalking && m_istalkedindex){
        static char* oldname = "";
        char* newname = TF2Entity::GetInfo(TF2Entity::GetPlayer(m_istalkedindex))->friendsName;
        if(oldname && strcmp(oldname,newname))
        {
            Namer::NameToEncrypted(newname);
            oldname = newname;
        }
        return;
    }

    if(!m_pStealDelay->GetInt())
        return;

    if( m_pRandNum->GetBool() )
    {
        if(!(pCmd->tick_count % m_pStealDelay->GetInt()))
            return;
        float randumb = rand();
        char* a = new char[32];
        sprintf(a, "%f", randumb);

        Namer::NameToEncrypted(a);
        delete a;
    }

    //I feel like UpdateTable might be a bit too slow to run every frame
    //I would like to have it update when a player connects/disconnects or when they change their name
    //if(!(m_itickcount % 60)|| m_vNames.size())
    if( t_now != t_prevtime || m_vNames.size() == 0 )
        UpdateTable();

    //if(pCmd->tick_count % m_pStealDelay->GetInt() == 0 && m_vNames.size())//we go now
    if( t_now % m_pStealDelay->GetInt() == 0 && m_vNames.size())//we go now
    {
        Namer::NameToEncrypted( m_vNames[ std::rand() % m_vNames.size() ]->name );
    }

    t_prevtime = t_now;
}

void CNameSteal::UpdateTable()
{
    m_vNames.clear();

    int len = pEngine->GetMaxClients() + 1;
    for (int i = 0; i < len; i++)
    {
        if(pEngine->GetLocalPlayer() == i)
            continue;

        player_info_t *ply = new player_info_t();
        pEngine->GetPlayerInfo(i, ply);
        if(ply && strlen(ply->name) > 1)
            m_vNames.push_back(ply);
        else
            delete ply;
    }
}

void CNameSteal::Stalk(const CCommand &args)
{
    if(args.ArgC() == 0 || !strlen(args.Arg(1)))
        return;
    SteamFriendTools::GetPlayerIndexFromName(Namer::CondenseToString(args), g_pNameSteal->m_istalkedindex);
}

CNameSteal::CNameSteal()
{
    // VVVVAAAAALLLLLLLVVVVVEEEEEE.....
    // Delay for changing names is default 20 seconds because Valve made that the max one can change their name
    m_pStealDelay = new ConVar(PREFIX "sname_delay", "20", 0, "A positive non-zero delay in /* frames */ seconds");
    m_pRunning = new ConVar(PREFIX "sname_running", "0", 0, "Namen stealen");
    m_pRandNum = new ConVar(PREFIX "sname_randnums", "0", 0, "Instead of stealing names, use random numbers");
    m_pStalk = new ConCommand(PREFIX "name_stalk",Stalk);
    m_istalkedindex = 0;
    t_prevtime = 0;

    g_pNameSteal = this;
}
