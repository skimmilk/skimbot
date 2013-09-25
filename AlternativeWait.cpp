#include "AlternativeWait.h"
#include "Interfaces.h"

void CmdWait(const CCommand &args)
{
    if(!strlen(args.Arg(1)) || args.ArgC() == 0)
        return;
    //need to get cmd buffer

    //Msg(args.GetCommandString());
    int i = atoi(args.Arg(1));

    CWaiter::g_pInstance->AddWait(args.GetCommandString(), i);
}
void CmdLoop(const CCommand &args)
{
    if(!strlen(args.Arg(1)) || args.ArgC() == 0)
        return;

}

CWaiter *CWaiter::g_pInstance;

CWaiter::CWaiter()
{
    m_command = new ConCommand(PREFIX "wait", CmdWait, "Acts just like the standard wait command");
    m_loopcmd = new ConCommand(PREFIX "loop", CmdLoop, "Runs the specified command after every amount of frames specified, can only be one instance");
    g_pInstance = this;
}
CWaiter::~CWaiter()
{
    Skim::ValveInterfaces::FreeConCMD(m_command);
    Skim::ValveInterfaces::FreeConCMD(m_loopcmd);
}

void CWaiter::Run(CUserCmd* pCmd)
{
    int i = m_vCommands.size();
    if(!i)
        return;

    std::vector<WaitCmd_t> vtmp;
    //Fire commands if wait is 0, otherwize decrement the command and add to temp
    for (int j = 0; j < i; j++)
    {
        if(!m_vCommands[j].m_iWait)
        {
            Skim::ValveInterfaces::pEngine->ClientCmd_Unrestricted(m_vCommands[j].m_sCmd);
        }
        else{
            m_vCommands[j].m_iWait--;
            vtmp.push_back(m_vCommands[j]);
        }
    }
    m_vCommands = vtmp;
}

void CWaiter::AddWait(const char* scmd, int iframes)
{
    WaitCmd_t noe;
    noe.m_iWait = iframes;
    strcpy(noe.m_sCmd, scmd);
    m_vCommands.push_back(noe);
}