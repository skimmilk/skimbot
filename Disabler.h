#pragma once
#include "Required.h"
#include "Interfaces.h"
#include "IHack.h"
#include "VMTHook.cpp"



class CDisabler : public IHack
{
public:
    CDisabler();
    ~CDisabler();
    void Run(CUserCmd *cmd);
    //20
    //static ConVar *m_disableZRotation;
    static bool m_bDisableSetAngles;
    static CVMTHook *m_hSetViewAngles;
    static CUserCmd* m_pOldCmd;
private:
    ConCommand *m_pDisableSetang;
    ConCommand *m_pEnableSetang;
};