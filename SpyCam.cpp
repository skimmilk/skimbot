#include "SpyCam.h"
#include "Interfaces.h"
#include "TF2Entity.h"
//#include "Memory.h"
//#include "..\game\client\c_point_camera.h"
#include "SkimGUI.h"
#include <Windows.h>

using namespace Skim;
using namespace ValveInterfaces;

CSpyCam::CSpyCam()
{
    m_pX = new ConVar( PREFIX "spycam_x", "1000");
    m_pY = new ConVar( PREFIX "spycam_y", "100");
    m_pSizeX = new ConVar( PREFIX "spycam_xsize", "320", 0, "", true, 16, true, 640);
    m_pSizeY = new ConVar( PREFIX "spycam_ysize", "180", 0, "", true, 16, true, 640);
    m_pEnabled = new ConVar( PREFIX "spycam", "0", 0, "", true, 0, true, 1);
    m_pSpeed = new ConVar( PREFIX "spycam_spec_speed", "10", 0, "", true, 1, true, 30);
    m_pLookSensitivity = new ConVar( PREFIX "spycam_spec_looksensitivity", "8", 0, "", true, 1, true, 10);
    m_qaSpecAngs = QAngle(0,0,0);
    m_vecSpecSpeed = Vector(0,0,0);

    CSkimWindow* asdf = CSkimGui::NewWindow(m_pEnabled,m_pX,m_pY,m_pSizeX,m_pSizeY, "Spy Cam");
    asdf->AddFilter( "Spectator" );
    asdf->AddSlider(m_pSpeed, "Flying Speed", "Spectator");
    asdf->AddSlider(m_pLookSensitivity, "Look sensitivity", "Spectator");
}
CSpyCam::~CSpyCam()
{
    FreeConCMD(m_pX);
    FreeConCMD(m_pY);
    FreeConCMD(m_pSizeX);
    FreeConCMD(m_pSizeY);
    FreeConCMD(m_pEnabled);
}

// update old cmd to be accesed on render func for its viewangles
// Again, the pointers change every frame
void CSpyCam::Run(CUserCmd* pCmd)
{
    m_pOldCmd = pCmd;
    if ( ! TF2Entity::Me()->isAlive() )
    {
        m_qaSpecAngs.x += ((float)pCmd->mousedy * (m_pLookSensitivity->GetFloat() / 100.f));
        m_qaSpecAngs.y -= ((float)pCmd->mousedx * (m_pLookSensitivity->GetFloat() / 100.f));
        //m_qaSpecAngs = 
        //pEngine->GetViewAngles(m_qaSpecAngs);
        QAngle tmp = m_qaSpecAngs;

        bool going = false;
        if (GetAsyncKeyState(0x57))
            going = true;
        else if (GetAsyncKeyState(0x53))//S
        {
            tmp.y += 180;
            going = true;
        }
        else if (GetAsyncKeyState(0x41))//A
        {
            tmp.y -= 90.f;
            going = true;
        }
        else if (GetAsyncKeyState(0x44))//D
        {
            tmp.y += 90.f;
            going = true;
        }

        if ( going )
            AngleVectors(tmp, &m_vecSpecSpeed);
        else
            m_vecSpecSpeed.Zero();
        m_vecSpecSpeed *= m_pSpeed->GetFloat();
        m_vecPrevLocation += m_vecSpecSpeed;
    }
    else
    {
        m_vecPrevLocation = TF2Entity::Me()->vecEyeOrigin();
        m_qaSpecAngs = pCmd->viewangles;
    }
}
void CSpyCam::Run_PaintTraverse()
{
    if ( m_pEnabled->GetBool() && pEngine->IsInGame() && ! pEngine->IsTakingScreenshot()) 
    {
        int x = m_pX->GetInt();
        int y = m_pY->GetInt();
        int xs = m_pSizeX->GetInt();
        int ys = m_pSizeY->GetInt();

        //Make sure everything fits on screen
        //if ( x<0 || y<0 || xs<0 || ys<0 || x+xs > pClient->GetScreenWidth() || y+ys > pClient->GetScreenHeight())
        //    return;

        //will call copy constructor
        CViewSetup spycam(*pViewRender->GetViewSetup());// = view;
        spycam.x = x;
        spycam.y = y;
        spycam.width = xs;
        spycam.height = ys;
        spycam.m_flAspectRatio = ((float)xs) / ((float)ys);

        spycam.fov = 90.0f;

        if (TF2Entity::Me()->isAlive())
        {
            QAngle spyViewAngles = m_qaSpecAngs;
            //if angles is greater than 180 then subtract 180 else add 180
            spyViewAngles.y += spyViewAngles.y > 180.0f ? -180.0f : 180.0f;
            spyViewAngles.x *= -1;
            spycam.angles = spyViewAngles;
            spycam.origin = TF2Entity::Me()->vecEyeOrigin();
        }
        else
        {
            spycam.angles = m_qaSpecAngs;
            spycam.origin = m_vecPrevLocation;
            //spycam.origin = Vector(-22,-380,-100);
        }
        Frustum frustum;
         pRenderView->Push3DView( spycam, VIEW_CLEAR_DEPTH | VIEW_CLEAR_COLOR, 0, (VPlane *)frustum );
        //((CViewRender*)pViewRender)->ViewDrawScene( false, SKYBOX_2DSKYBOX_VISIBLE, spycam, 0, VIEW_MONITOR );
        pViewRender->RenderView(spycam, VIEW_CLEAR_DEPTH | VIEW_CLEAR_COLOR, 0);
         pRenderView->PopView( frustum );
    }
}