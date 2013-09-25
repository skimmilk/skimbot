#include "Esp.h"
#include "Drawing.h"
#include "TraceHelper.h"
#include "TF2PlayerTools.h"

using namespace Skim;
using namespace Drawing;

void ReinitFont()
{
    Drawing::Init();
}
CEsp::CEsp()
{
    m_pName = new ConVar(PREFIX "esp_name", "1");
    m_pHealth = new ConVar(PREFIX "esp_health", "1");
    m_pBBox = new ConVar(PREFIX "esp_box", "1");
    m_pBones = new ConVar(PREFIX "esp_bones", "1");
    m_pEsp = new ConVar(PREFIX "esp", "0");
    m_pHealthBar = new ConVar(PREFIX "esp_hbar", "1");
    m_pDrawDist = new ConVar(PREFIX "esp_drawdist", "0");
    m_pMaxDist = new ConVar(PREFIX "esp_maxdist", "8192");
    m_pDrawDistCrosshair = new ConVar(PREFIX "esp_drawshotdist", "0");
    m_pAltFriendColor = new ConVar(PREFIX "esp_altfriendcolor", "1");
    m_pWpnName = new ConVar(PREFIX "esp_wpnname", "0");
    m_pShowLocal = new ConVar(PREFIX "esp_devshowlocalplayer", "0");
    m_pShowAimbotted = new ConVar(PREFIX "esp_showaimbotted", "1");
    m_pDrawSameTeam = new ConVar(PREFIX "esp_sameteam", "1", 0, "Draws esp on both enemies and friendlies");
    m_pLookDir = new ConVar( PREFIX "esp_lookdirdist", "0", 0, "Draws a line showing where people are looking with a line with the specified length");
    m_pDetailDist = new ConVar( PREFIX "esp_detaildist", "3000", 0, "If a player is farther than this from us, will only draw basic ESP");
    m_pReinitFont = new ConCommand( PREFIX "esp_reinitfont", ReinitFont);
    //m_pSpecShow = new ConVar( PREFIX "esp_spectators_show", "1", 0, "Shows if anyone is spectating you and who");
    //m_pSpecX = new ConVar( PREFIX "esp_spectators_xpos", "30");
    //m_pSpecY = new ConVar( PREFIX "esp_spectators_ypos", "200"); 
}
CEsp::~CEsp()
{
    FreeConCMD(m_pName);
    FreeConCMD(m_pHealth);
    FreeConCMD(m_pBBox);
    FreeConCMD(m_pBones);
    FreeConCMD(m_pEsp);
    FreeConCMD(m_pHealthBar);
    FreeConCMD(m_pDrawDist);
    FreeConCMD(m_pMaxDist);
    FreeConCMD(m_pDrawDistCrosshair);
    FreeConCMD(m_pAltFriendColor);
    FreeConCMD(m_pWpnName);
    FreeConCMD(m_pShowLocal);
    FreeConCMD(m_pShowAimbotted);
    FreeConCMD(m_pDrawSameTeam);
    FreeConCMD(m_pLookDir);
    FreeConCMD(m_pDetailDist);
    FreeConCMD(m_pReinitFont);
}

void CEsp::Run_PaintTraverse()
{
    if (!m_pEsp->GetBool()) //|| !m_pSpecShow->GetBool())
        return;
    int maxclients = ValveInterfaces::pEngine->GetMaxClients() + 1;
    for( int i = 1; i < maxclients; i++)
    {
        TF2Player* pl = TF2Entity::GetPlayer(i);
        if ((i != ValveInterfaces::pEngine->GetLocalPlayer() || m_pShowLocal->GetBool()) && 
            pl && pl->isValid() && 
            (m_pDrawSameTeam->GetBool() || pl->iTeamNum() != TF2Entity::Me()->iTeamNum()) && 
            CTF2PlayerTools::PlayerDormantPeriod(i) < 90 &&
            (m_pMaxDist->GetInt() == 0 || TF2Entity::DistanceFromMe(pl) < m_pMaxDist->GetFloat()))

            DrawPlayerEsp(pl);
    }

    if ( m_pDrawDistCrosshair->GetBool() ) {
        int cx = pClient->GetScreenWidth() / 2, cy = pClient->GetScreenHeight() / 2 + 15;
        int distance = TraceHelper::DistFromPlrAim( TF2Entity::Me() );
        char sDist[5];
        itoa(distance, sDist, 10);
        Drawing::DrawStringCenter(cx, cy, Color(255,255,255,255), sDist);
    }
}
//draws health bar
void DrawHealthBar(TF2Player* plr, const Color& plrTeamClr, const CartPt& feet, int& drawTextPos, bool drawingHealthString)
{
    int maxHealth = 0;
    switch (plr->playerClass()) {
    case Heavy:
        maxHealth = 300; break;
    case Pyro:
        maxHealth = 150; break;
    case Soldier:
        maxHealth = 200; break;
    case Demoman:
        maxHealth = 175; break;
    case Medic:
        maxHealth = 150; break;
    default:
        maxHealth = 125;
    }
    int currentHealth = plr->iHealth();
    if (currentHealth > maxHealth) maxHealth = currentHealth;
    const int boxLength = 66;
    const int boxHalf = 33;
    const int boxHeight = 13;

    drawTextPos -= 6;
    Color hlthClr = Color(0,0,0, 127);
    int fill = Drawing::HealthbarColorAndFill(maxHealth, currentHealth, boxLength, hlthClr);
    //draws a box from upper left + fill to lower right with transparent black
    Drawing::DrawFilledBox(feet.x + fill - boxHalf, feet.y + drawTextPos, feet.x + boxLength - boxHalf, feet.y + drawTextPos + boxHeight, Color(0,0,0,85));
    //draws a box from upper left to lower right + fill with transparent health color
    Drawing::DrawFilledBox(feet.x - boxHalf, feet.y + drawTextPos, feet.x + fill - boxHalf, feet.y + drawTextPos + boxHeight, hlthClr);
    Drawing::DrawBox(feet.x - boxHalf, feet.y + drawTextPos, feet.x + boxLength - boxHalf, feet.y + drawTextPos + boxHeight, Color(0,0,0,255));
    if (!drawingHealthString) drawTextPos += 15;
    drawTextPos += 6;
}
//draws the bones for this player
void DrawBones(TF2Player* plr, const Color& plrTeamClr)
{
    Vector head, shoulder, shoulderL, shoulderR, pelvis, kneeR, kneeL, ankleR, ankleL, elbowR, elbowL, handL, handR;
    if (!plr->GetHitboxPos(head, HITBOX_HEAD)) 
        return;//assuming if we can't get this we ain't getting the others
    plr->GetHitboxPos(shoulder, ::HITBOX_SPINE_3);
    plr->GetHitboxPos(shoulderL, ::HITBOX_SPINE_3, BonePos::MAX);
    plr->GetHitboxPos(shoulderR, ::HITBOX_SPINE_3, BonePos::MIN);
    plr->GetHitboxPos(pelvis, ::HITBOX_PELVIS);
    //max in this case is closer to ankle
    plr->GetHitboxPos(ankleL, ::HITBOX_FOOT_L, BonePos::MAX);
    plr->GetHitboxPos(ankleR, ::HITBOX_FOOT_R, BonePos::MIN);
    plr->GetHitboxPos(handL, ::HITBOX_HAND_L, BonePos::MIN);
    plr->GetHitboxPos(handR, ::HITBOX_HAND_R, BonePos::MIN);
    plr->GetHitboxPos(kneeL, ::HITBOX_KNEE_L, BonePos::MIN);
    plr->GetHitboxPos(kneeR, ::HITBOX_KNEE_R, BonePos::MIN);
    plr->GetHitboxPos(elbowR, ::HITBOX_UPPERARM_R, BonePos::MAX);
    plr->GetHitboxPos(elbowL, ::HITBOX_UPPERARM_L, BonePos::MAX);

    CartPt scrhead, scrshoulder, scrshoulderL, scrshoulderR, scrpelvis, scrkneeR, scrkneeL, scrankleR, scrankleL, screlbowR, screlbowL, scrhandL, scrhandR;

    // Its worth taking the time to check all of these so we do not have
    //     take the time and draw everything out if the person is obviously not on screen
    //     also will glitch out if one cartpt is set but the other errored and draws lines errywhere

    if (!(
        Drawing::WorldVectorToScreen(head, scrhead) &&
        Drawing::WorldVectorToScreen(head, scrhead) &&
        Drawing::WorldVectorToScreen(shoulder, scrshoulder) &&
        Drawing::WorldVectorToScreen(shoulderL, scrshoulderL) &&
        Drawing::WorldVectorToScreen(shoulderR, scrshoulderR) &&
        Drawing::WorldVectorToScreen(pelvis, scrpelvis) &&
        Drawing::WorldVectorToScreen(kneeR, scrkneeR) &&
        Drawing::WorldVectorToScreen(kneeL, scrkneeL) &&
        Drawing::WorldVectorToScreen(ankleR, scrankleR) &&
        Drawing::WorldVectorToScreen(ankleL, scrankleL) &&
        Drawing::WorldVectorToScreen(elbowR, screlbowR) &&
        Drawing::WorldVectorToScreen(elbowL, screlbowL) &&
        Drawing::WorldVectorToScreen(handL, scrhandL) &&
        Drawing::WorldVectorToScreen(handR, scrhandR)))
    return;


    Drawing::DrawLine(scrpelvis, scrkneeR, plrTeamClr);
    Drawing::DrawLine(scrhead, scrshoulder, plrTeamClr);
    Drawing::DrawLine(scrshoulder, scrpelvis, plrTeamClr);
    Drawing::DrawLine(scrshoulderL, scrshoulder, plrTeamClr);
    Drawing::DrawLine(scrshoulderR, scrshoulder, plrTeamClr);
    Drawing::DrawLine(scrshoulderL, screlbowL, plrTeamClr);
    Drawing::DrawLine(scrshoulderR, screlbowR, plrTeamClr);
    Drawing::DrawLine(screlbowL, scrhandL, plrTeamClr);
    Drawing::DrawLine(screlbowR, scrhandR, plrTeamClr);
    Drawing::DrawLine(scrpelvis, scrkneeL, plrTeamClr);
    Drawing::DrawLine(scrkneeR, scrankleR, plrTeamClr);
    Drawing::DrawLine(scrkneeL, scrankleL, plrTeamClr);
}

void CEsp::DrawPlayerEsp(TF2Player* plr)
{
    static const Color white = Color(255,255,255,255);

    Color plrTeamClr = Drawing::PlayerTeamColor(m_pAltFriendColor->GetBool(), m_pShowAimbotted->GetBool(), plr);

    Drawing::CartPt head, feet;
    if( !Drawing::PlayerToScreen(plr, head, feet) )
        return;
    
    //// player must be not dormant? and be on spectator or dead
    //if ( m_pSpecShow->GetBool() && !plr->isDormant() && ( !plr->isAlive() || plr->iTeamNum() == 4 ) && plr->index() != pEngine->GetLocalPlayer())
    //{
    //    static int drawTextPos = 0;
    //    static int lastPlrIndex = 0;
    //    static bool someonesWatching = false;

    //    // we looped back, started drawing a new frame
    //    if (plr->index() < lastPlrIndex){
    //        drawTextPos = 0;
    //        someonesWatching = false;
    //    }
    //    lastPlrIndex = plr->index();

    //    int index = 0;
    //    try { index = plr->observerTargetIndex(); }
    //    catch (...) { return; }

    //    if (index == pEngine->GetLocalPlayer())
    //    {
    //        // First time
    //        if (!someonesWatching)
    //            Drawing::DrawString(
    //                            m_pSpecX->GetInt(),
    //                            m_pSpecY->GetInt(),
    //                            white,
    //                            "Players watching you:");
    //        someonesWatching = true;
    //        drawTextPos += 12;

    //        Drawing::DrawString(
    //            m_pSpecX->GetInt(),
    //            m_pSpecY->GetInt() + drawTextPos,
    //            white,
    //            TF2Entity::GetInfo(plr)->name);
    //    }
    //}

    // player must be on a valid team and be alive
    //if (!m_pEsp->GetBool() || !plr->isValid()) return;
    if (m_pBBox->GetBool()) {
        int divisor = 4; if (plr->playerClass() == Heavy) divisor = 3;//hue
        int fatness = (head.y - feet.y) / divisor;
        int midline = (head.x + feet.x) / 2;
        Drawing::DrawBox(midline + fatness, head.y, midline - fatness, feet.y, plrTeamClr);
    }
    if (m_pBones->GetBool()) {
        DrawBones(plr, plrTeamClr);
    }
    float dist;
    if ((dist = m_pLookDir->GetFloat()) > 0.0f) {
        Vector head, end;
        plr->GetHitboxPos(head, 0);
        AngleVectors(plr->angEyeAngles(), &end);
        end = head + (end * dist);
        Drawing::DrawLineFromWorld(head, end, plrTeamClr);
    }

    // Won't draw out the text and stuff below if too far away
    if (m_pDetailDist->GetFloat() < TF2Entity::DistanceFromMe(plr))
        return;

    // 1/3rd the way from feet to head
    CartPt espStart = CartPt(feet.x, feet.y - ((feet.y - head.y) / 3));
    int drawTextPos = 0;

    if (m_pName->GetBool()) {
        Drawing::DrawStringCenter(espStart.x, espStart.y + drawTextPos, white, TF2Entity::GetInfo(plr)->name);
        drawTextPos += 12;
    }
    if (m_pWpnName->GetBool())
    {
        TF2Weapon* wpn = plr->ActiveWeapon();
        Drawing::DrawStringCenter(espStart.x, espStart.y + drawTextPos, white, wpn->sName());
        drawTextPos += 12;
    }
    if (m_pDrawDist->GetBool()) {
        char sDist[5];
        itoa(TF2Entity::DistanceFromMe(plr), sDist, 10);
        Drawing::DrawStringCenter(espStart.x, espStart.y + drawTextPos, white, sDist);
        drawTextPos += 12;
    }
    if (m_pHealthBar->GetBool()) {
        DrawHealthBar(plr, plrTeamClr, espStart, drawTextPos, m_pHealth->GetBool());
    }
    if (m_pHealth->GetBool()) {
        char sHealth[8];
        // Avoid overflows when health is too high, like in saxton hale mode
        ::itoa(plr->iHealth() % 10000000, sHealth, 10);
        Drawing::DrawStringCenter(espStart.x, espStart.y + drawTextPos, white, sHealth);
        drawTextPos += 12;
    }
}