#include "Required.h"
#include "Interfaces.h"
#include "BaseHook.h"

using namespace Skim;
CTF2Hook *g_pTF2Hook;

//----------------------------------------
//--------Functions called by hook--------
//----------------------------------------
void __stdcall Hooked_CreateMove(int sequence_number, float input_sample_frametime, bool active)
{
    static bool first = true;
    if(first){
        DevMsg("%s Hooked CreateMove...\n", PLUGIN_NAME);
        first = false;

        g_pTF2Hook->PostInit();
    }

    if (ValveInterfaces::pEngine->IsInGame())
        g_pTF2Hook->RunBeforeCM(sequence_number, input_sample_frametime, active);

    g_pTF2Hook->pCMHook->UnHook();
    ValveInterfaces::pClient->CreateMove(sequence_number, input_sample_frametime, active);
    g_pTF2Hook->pCMHook->ReHook();

    CUserCmd* pCmd = ValveInterfaces::pIInput->GetUserCmd(sequence_number);
    CInput::CVerifiedUserCmd* pVerified = &(*MakePointer<CInput::CVerifiedUserCmd*>( ValveInterfaces::pIInput, 0xB8 ))[ sequence_number % MULTIPLAYER_BACKUP ];
    
    if(!pCmd || !pVerified)
    {
        // big deal here
        Error("%s CreateHook failed ;[\n",PLUGIN_ERROR);
        Error("%s pCmd: 0x%p, pVerified: 0x%p\n",PLUGIN_ERROR,pCmd,pVerified);
        ValveInterfaces::pEngine->ClientCmd_Unrestricted("showconsole");
        g_pTF2Hook->pCMHook->UnHook();
        return;
    }

    if (ValveInterfaces::pEngine->IsInGame())
        g_pTF2Hook->RunHacks(pCmd);

    pVerified->m_cmd = *pCmd;
    pVerified->m_crc = pCmd->GetChecksum();
}
void __stdcall Hooked_PaintTraverse(vgui::VPANEL vpanel, bool forceRepaint, bool allowForce)
{
    g_pTF2Hook->pPTHook->UnHook();
    ValveInterfaces::pPanel->PaintTraverse(vpanel, forceRepaint, allowForce);
    g_pTF2Hook->pPTHook->ReHook();
    
    static bool first = true;
    static bool render = false;

    const char* panelName = ValveInterfaces::pPanel->GetName(vpanel);
    //Relies on panels to render
    //More than one panel, all stack up on top of eachother
    //staticClientDLLPanel is rendered, then gamemenu, then matsys...

    if ( !render && !strcmp(panelName, "staticClientDLLPanel") )
        render = true;

    if ( !strcmp(panelName, "GameMenu") )
        render = false;

    if ( render && !strcmp(panelName, "MatSystemTopPanel") && ! ValveInterfaces::pEngine->IsPaused() && ! ValveInterfaces::pEngine->IsTakingScreenshot() ) {
        if(first){
            DevMsg("%s Hooked PaintTraverse\n", PLUGIN_NAME);
            first = false;
        }
        g_pTF2Hook->RunPaintTraverse();
    }
}

//----------------------------------------
//-------Destructor and initializer-------
//----------------------------------------
CTF2Hook::~CTF2Hook()
{
    while (m_vHacksToRun.size())
    {
        //.pop_back() and .clear() are SUPPOSED to call the destructors
        //is Microsoft retarded or am I missing something
        delete m_vHacksToRun.back();
        m_vHacksToRun.pop_back();
    }
}
void CTF2Hook::Shutdown()
{
    pCMHook->UnHook();
    pPTHook->UnHook();
}
void CTF2Hook::Init()
{
    pCMHook = new CVMTHook();
    DevMsg( PLUGIN_NAME "Hooking CreateMove\n");
    pCMHook->Hook( Hooked_CreateMove, (PDWORD)ValveInterfaces::pClient, CREATEMOVE_INDEX );
    pPTHook = new CVMTHook();
    DevMsg( PLUGIN_NAME " Hooking PaintTraverse\n");
    pPTHook->Hook( Hooked_PaintTraverse, (PDWORD)ValveInterfaces::pPanel, 40 ); 
}
void CTF2Hook::PostInit()
{
    for (uint i = 0; i < m_vHacksToRun.size(); i++)
        m_vHacksToRun[i]->Initialize();
}

//----------------------------------------
//----Functions that add hacks to hook----
//----------------------------------------
void CTF2Hook::AddClassName(const char* name)
{
    m_vClassNames.push_back(name);
}
void CTF2Hook::AddToHook(IHack *hack)
{
    m_vHacksToRun.push_back(hack);
}


// Ugly stuff to make it easy to disable and re-enable try/catch stuff for speed or debugging
// Tries are very slow, should disable them when we are sure it will not crash
#define _TRYCATCH

#ifdef _TRYCATCH
#define _TRY try{
#define _CLOSE }
#define _CATCH }catch(...){
#define _REPORT_ERROR(className,method) ReportError(className,method);
#endif

#ifndef _TRYCATCH
#define _TRY
#define _CLOSE 
#define _CATCH 
#define _REPORT_ERROR(className,method)
#endif

void CTF2Hook::ReportError(int classIndex, const char* method)
{
    static int bitchCount = 0;
    if (++bitchCount > 10)
        return;
    ValveInterfaces::pEngine->ClientCmd_Unrestricted("showconsole");
    Msg(PLUGIN_ERROR "Hack crashed in class %s at %s.\n", m_vClassNames[classIndex], method);
}


//----------------------------------------
//----Functions called by global hooks----
//----------------------------------------

// called by Hooked_CreateMove when ingame
void CTF2Hook::RunHacks(CUserCmd *pCmd)
{
    int index = 0;
    _TRY
        int length = m_vHacksToRun.size();
        for(; index < length; index++) {
            m_vHacksToRun[index]->Run(pCmd);
        }
    _CATCH
        _REPORT_ERROR(index, "CTF2Hook::RunHacks(CUserCmd)");
    _CLOSE
}
// called by Hooked_CreateMove when ingame
void CTF2Hook::RunBeforeCM(int& sequence_number, float& input_sample_frametime, bool& active)
{
    int index = 0;
    _TRY
        int length = m_vHacksToRun.size();
        for(; index < length; index++) {
            m_vHacksToRun[index]->Run_BeforeCM(sequence_number, input_sample_frametime, active);
        }
    _CATCH
        _REPORT_ERROR(index, "CTF2Hook::CTF2Hook::RunBeforeCM(int&, float&, bool&)");
    _CLOSE
}

// called by Hooked_PaintTraverse when on the panel we want and ingame
void CTF2Hook::RunPaintTraverse()
{
    int index = 0;
    _TRY
        int length = m_vHacksToRun.size();
        for(; index < length; index++) {
            m_vHacksToRun[index]->Run_PaintTraverse();
        }
    _CATCH
        _REPORT_ERROR(index, "CTF2Hook::RunPaintTraverse()");
    _CLOSE
}

