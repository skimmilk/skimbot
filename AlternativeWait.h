//Created Nov. 18, 2011
//Modified Nov. 18, 2011

#pragma once
#include "Required.h"
#include "Interfaces.h"

struct WaitCmd_t { char m_sCmd[512]; int m_iWait; };

class CWaiter : public IHack
{
public:
    CWaiter();
    ~CWaiter();
    //void Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active);
    void Run(CUserCmd *cmd);
    //void Run_PaintTraverse();
    //void Shutdown();
    void AddWait(const char*, int iframes);
    static CWaiter *g_pInstance;
private:
    std::vector<WaitCmd_t> m_vCommands;
    ConCommand *m_command;
    ConCommand *m_loopcmd;
};