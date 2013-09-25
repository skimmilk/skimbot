#include "Radar.h"
#include "Interfaces.h"
#include "AimAndMovementHelper.h"
#include "SkimGUI.h"
#include "TF2PlayerTools.h"

CRadar::CRadar()
{
    m_pEnabled = new ConVar(PREFIX "radar", "1");
    m_pDrawDist = new ConVar(PREFIX "radar_drawdistance", "30", 0, "Distance in inches per pixel", true, 1,true,100);
    m_pPosX = new ConVar(PREFIX "radar_pos_x", "32");
    m_pPosY = new ConVar(PREFIX "radar_pos_y", "32");
    m_pSizeX = new ConVar(PREFIX "radar_size_x", "128", 0, "", true, 16, true, 256);
    m_pSizeY = new ConVar(PREFIX "radar_size_y", "128", 0, "", true, 16, true, 256);
    m_pAltFriendColor = new ConVar(PREFIX "radar_altfriendcolor", "1", 0, "", true, 0, true, 1);
    m_pShowAimbotted = new ConVar(PREFIX "radar_showaimbotted", "1", 0, "", true, 0, true, 1);
    m_pBgMap = new ConVar(PREFIX "radar_backgroundmap", "0");
    m_pDrawSameTeam = new ConVar(PREFIX "radar_showteam", "1", 0, "", true, 0, true, 1);
    m_pLookDir = new ConVar( PREFIX "radar_lookdirdist", "10", 0, "", true, 0, true, 25);
    m_iBgImageID = -1;

    CSkimWindow* radarGui = CSkimGui::NewWindow(m_pEnabled,m_pPosX,m_pPosY,m_pSizeX, m_pSizeY, "Radar");
    radarGui->AddSlider(m_pDrawDist, "Draw distance");
    radarGui->AddSlider(m_pLookDir, "Look line distance");
}
CRadar::~CRadar()
{
    FreeConCMD(m_pEnabled);
    FreeConCMD(m_pDrawDist);
    FreeConCMD(m_pPosX);
    FreeConCMD(m_pPosY);
    FreeConCMD(m_pSizeX);
    FreeConCMD(m_pSizeY);
    FreeConCMD(m_pAltFriendColor);
    FreeConCMD(m_pShowAimbotted);
    FreeConCMD(m_pBgMap);
    FreeConCMD(m_pDrawSameTeam);
    FreeConCMD(m_pLookDir);
}

void CRadar::DrawRadarBG(int posx, int posy, int sizex, int sizey)
{
    if (!m_pBgMap->GetBool())
    {
        static Color black = Color(0,0,0,255);
        static Color bg = Color(69,69,69,128);
        Drawing::DrawFilledBox(posx, posy, posx + sizex, posy + sizey, bg);
        Drawing::DrawBox(posx, posy, posx + sizex, posy + sizey, black);
        Drawing::DrawLine(posx + (sizex / 2), posy, posx + (sizex / 2), posy + sizey, black);
        Drawing::DrawLine(posx, posy + (sizey / 2), posx + sizex, posy + (sizey / 2), black);
    }
    /*
    else 
    {
        m_iBgImageID = pSurface->DrawGetTextureId( "topview\\ctf_2fort" );

        if (m_iBgImageID == -1)
        {
            m_iBgImageID = surface()->CreateNewTextureID();
            pSurface->DrawSetTextureFile( m_iBgImageID, "topview\\ctf_2fort", false, true );
        }

        Vector myLoc = TF2Entity::Me()->vecOrigin();
        QAngle myAngles = TF2Entity::Me()->angEyeAngles();
        pSurface->DrawSetTexture( m_iBgImageID );
        pSurface->DrawSetColor( 255, 255, 255, 256 );
        //do math stuff
        //pSurface->DrawTexturedRect( posx, posy, sizex, sizey );
        //pSurface->DrawTexturedSubRect(posx, posy, sizex, sizey,);

        //pSurface->DrawSetTexture( m_iBgImageID );
        //pSurface->DrawSetColor( 255, 255, 255, 255 );
    }
    */
}
bool CRadar::WorldVecToRadar(const Vector& vec, Drawing::CartPt& screen)
{    
    int minDim = m_pSizeX->GetInt();
    if (minDim > m_pSizeY->GetInt()) minDim = m_pSizeY->GetInt();
    int sx = m_pSizeX->GetInt(), sy = m_pSizeY->GetInt();
    int x0 = m_pPosX->GetInt(), y0 = m_pPosY->GetInt(), x1 = sx + x0, y1 = sy + y0;

    Vector vecDist = vec - TF2Entity::Me()->vecOrigin();
    float xdiff = vecDist.x, ydiff = vecDist.y;
    float yAng = TF2Entity::Me()->angEyeAngles().y;

    AimAndMovementHelper::SetAngles(xdiff,ydiff,yAng * -1.0f,90.0f);

    float mult = minDim / m_pDrawDist->GetFloat() / minDim;
    int x = xdiff * mult * -1, y = ydiff * mult;

    x += x0 + (sx / 2); y += y0 + (sy / 2);
    if (x < x0 || x > x1 || y < y0 || y > y1) return false;
    screen.x = x; screen.y = y;
    return true;
}
void CRadar::DrawPlayerViewline(TF2Player* plyr, float dist, const Color& clr)
{
    Drawing::CartPt screenorig;
    if (!WorldVecToRadar(plyr->vecOrigin(), screenorig)) return;

    Color plrTeamClr = Drawing::PlayerTeamColor(m_pAltFriendColor->GetBool(), m_pShowAimbotted->GetBool(), plyr);

    Vector lookan;
    AngleVectors(plyr->angEyeAngles(), &lookan);
    // 1 pixel = drawdist inches
    lookan = plyr->vecOrigin() + (lookan * m_pDrawDist->GetFloat() * dist);

    Drawing::CartPt screenlook;
    if (!WorldVecToRadar(lookan, screenlook)) return;

    Drawing::DrawLine(screenorig.x, screenorig.y, screenlook.x, screenlook.y, plrTeamClr);
}
void CRadar::DrawPlayerDot(TF2Player* plyr, const Color& clr)
{
    Color plrTeamClr = Drawing::PlayerTeamColor(m_pAltFriendColor->GetBool(), m_pShowAimbotted->GetBool(), plyr);
    Drawing::CartPt screen;
    if (!WorldVecToRadar(plyr->vecOrigin(), screen)) return;
    Drawing::DrawBox(screen.x - 2, screen.y - 2, screen.x + 2, screen.y + 2, plrTeamClr);
}
void CRadar::Run_PaintTraverse()
{
    if (!(m_pEnabled->GetBool()))
        return;
    DrawRadarBG(m_pPosX->GetInt(), m_pPosY->GetInt(), m_pSizeX->GetInt(), m_pSizeY->GetInt());

    int maxClients = ValveInterfaces::pEngine->GetMaxClients() + 1;
    for (int i = 1; i < maxClients; i++)
    {
        TF2Player* plyr = TF2Entity::GetPlayer(i);
        if (plyr && i != pEngine->GetLocalPlayer() && plyr->isValid() && 
            (m_pDrawSameTeam->GetBool() || plyr->iTeamNum() != TF2Entity::Me()->iTeamNum()) && CTF2PlayerTools::PlayerDormantPeriod(i) < 90)
        {
            Color plrTeamClr = Drawing::PlayerTeamColor(m_pAltFriendColor->GetBool(), m_pShowAimbotted->GetBool(), plyr);
            DrawPlayerDot(plyr, plrTeamClr);
            float dist = m_pLookDir->GetFloat();
            if (!(dist < 0.0f))
                DrawPlayerViewline(plyr, dist, plrTeamClr);
        }
    }
}