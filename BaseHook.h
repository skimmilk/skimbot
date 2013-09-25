#pragma once
#include "Required.h"
#include "VMTHook.cpp"
#include "IHack.h"
#include <stdstring.h>
#include <vector>

class CTF2Hook
{
public:
    ~CTF2Hook();
    void Init();
    void Shutdown();
    void AddToHook(IHack*);
    void RunBeforeCM(int& sequence_number, float& input_sample_frametime, bool& active);
    void PostInit();
    void RunHacks(CUserCmd*);
    void RunPaintTraverse();
    void AddClassName(const char*);
    //CreateMmove
    CVMTHook* pCMHook;
    //PaintTraverse
    CVMTHook* pPTHook;
private:
    std::vector<IHack*> m_vHacksToRun;
    std::vector<const char*> m_vClassNames;
    void ReportError(int index, const char* method);
};

extern CTF2Hook *g_pTF2Hook;