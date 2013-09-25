#pragma once
#include "Required.h"
#include "VMTHook.cpp"
#include "IHack.h"

class CSpyCam : public IHack
{
public:
    CSpyCam();
    ~CSpyCam();

    static void HOOKED_RenderView( const CViewSetup &view, int nClearFlags, int whatToDraw );
    void Run(CUserCmd*);
    void Run_PaintTraverse();
    //void Initialize();
private:
    //CVMTHook *m_hRenderView;
    ConVar *m_pX, *m_pY, *m_pSizeX, *m_pSizeY, *m_pEnabled, *m_pSpeed, *m_pLookSensitivity;
    CUserCmd* m_pOldCmd;

    QAngle m_qaSpecAngs;
    Vector m_vecSpecSpeed;
    Vector m_vecPrevLocation;
    //static CSpyCam *pThisPtr;
    //static DWORD __CViewRender_ViewDrawScene;
    //static DWORD __CViewRender_DrawOneMonitor; tf2 dun have drawonemonitor
};