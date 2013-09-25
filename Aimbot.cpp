#include "Aimbot.h"
#include "TF2Entity.h"
#include "TraceHelper.h"
#include "AimAndMovementHelper.h"

using namespace Skim;
using namespace AimAndMovementHelper;

CAimbot *g_pAimbot;
int CAimbot::m_iIndexAimed;

void AltKnifeAim(CUserCmd* pCmd, TF2Player* prey)
{
    //if we are behind, offset = 0
    //if we are in front, offset = 60 or something
    AimAndMovementHelper::AimAt(prey, pCmd, HITBOX_SPINE_3, false);

    bool aimLeft = true;
    float aimDiff = AimAndMovementHelper::AngleDifference(prey->angEyeAngles().y, pCmd->viewangles.y);
    if (aimDiff < 0.0f){
        aimDiff *= -1.0f;
        aimLeft = false;
    }

    //Nice smooth aim
    //static ConVar* multout = 0;
    //if (!multout) multout = new ConVar( PREFIX "dev_aim_multout", "6" );
    //static ConVar* multin = 0;
    //if (!multin) multin = new ConVar( PREFIX "dev_aim_multin", "6" );
    //static ConVar* multpow = 0;
    //if (!multpow) multpow = new ConVar( PREFIX "dev_aim_multpow", "0.25" );

    //large offset while in front and side, smaller only when in facing back
    //**game goes crazy and freezes (but not crashes which makes it weird) if offset is messed up, probably 
    //        because it is too small and becomes NAN when directly behind
    //**could crash servers by sending messed up viewangles?
    //found through testing that:
    //the power is best at .5,
    //the multiplier inside is best at .25,
    //the multiplier outside is best at 13
    float offset = FastSqrt(aimDiff * 0.25f) * 13.0f;
    
    //static ConVar* maxDist = 0;
    //if (!maxDist)
    //    maxDist = new ConVar( PREFIX "dev_aim_stabdist", "80");
    //static ConVar* maxAngle = 0;
    //if (!maxAngle)
    //    maxAngle = new ConVar( PREFIX "dev_aim_stabangle", "40");

    float dist = TF2Entity::DistanceFromMe(prey);

    if (offset < 40.0f && dist < 80.0f)
        pCmd->buttons |= IN_ATTACK;
    if (_isnan(offset)) { //the offset is very small and errored up, means I am definitely behind them...
        if (dist < 80.0f)
            pCmd->buttons |= IN_ATTACK;
        return;
    }

    float yAngle = atan2(offset,dist) * 180.0f / M_PI;
    if (aimLeft)
        pCmd->viewangles.y -= yAngle;
    else
        pCmd->viewangles.y += yAngle;

    ValveInterfaces::pEngine->SetViewAngles(pCmd->viewangles);
}

void CAimbot::Run(CUserCmd* pcmd)
{
    m_iIndexAimed = -1;
    //check if we're running
    if (!(m_bRunning || m_pAutoshoot->GetBool() || ((m_pRunAttack->GetBool()) && (pcmd->buttons & IN_ATTACK))))
        return;

    float smallestdist = 8192.0f;//max trace distance, we cannot hit players if they're farther than this ;[
    TF2Player* closestplayer = 0;
    int maxClients = Skim::ValveInterfaces::pEngine->GetMaxClients() + 1;
    for(int i = 0; i < maxClients; i++)
    {
        // If we're in spy knife mode, then we want to make sure enemies are not above or below us
        // TODO: add priorities to enemies
        //    e.g. medics are higher, people far away with melee weapons are lower, spies close to us are maximum
        TF2Player *plyr = TF2Entity::GetPlayer(i);
        float dist = TF2Entity::DistanceFromMe( plyr );
        //check for dormancy because we won't be able to see it
        if ( plyr && !plyr->isDormant() && plyr->isValid() && ( ! m_pAltKnifeAim->GetBool() || abs(plyr->vecOrigin().z - TF2Entity::Me()->vecOrigin().z) < 100) )
        {
            if ( plyr->iTeamNum() != TF2Entity::Me()->iTeamNum() && dist < smallestdist &&  TraceHelper::CanSee(TF2Entity::Me(), plyr)){
                smallestdist = dist;
                closestplayer = plyr;
            }
        }
    }

    if (closestplayer) 
    {
        //int i = pcmd->weaponsubtype;
        //bool knifeOut = (i == 2 && TF2Entity::Me()->playerClass() == Spy);
        bool knifeMode = m_pAltKnifeAim->GetBool();//knifeOut && m_pAltKnifeAim->GetBool();

        m_iIndexAimed = closestplayer->index();
        // to be not silent, cannot have silent aim but can if we have altknifeaim
        // to be silent, must have silent on and altknifeaim off
        bool silent = m_pSilentAim->GetBool() && !knifeMode;

        if(knifeMode)
        {
            AltKnifeAim(pcmd, closestplayer);
        }
        else //not in knife aim mode
        {
            int hitbox = 0;
            switch (m_pAimBodypart->GetInt()) {
            case 1:
                hitbox = ::HITBOX_SPINE_2;
                break;
            case 2:
                hitbox = ::HITBOX_PELVIS;
                break;
            case 3:
                hitbox = HITBOX_ORIGIN;
            default:
                break;
            }
            AimAndMovementHelper::AimAt(closestplayer, pcmd, hitbox, silent);
        }

        if(!silent)
            pEngine->SetViewAngles(pcmd->viewangles);
        if (m_pAttackAimed->GetBool() && !(knifeMode))
            pcmd->buttons |= IN_ATTACK;
    }
}

void StartAim() {
    g_pAimbot->m_bRunning = true;
}
void EndAim() {
    g_pAimbot->m_bRunning = false;
}

CAimbot::CAimbot()
{
    m_pRunAttack = new ConVar(PREFIX "aim_runonattack", "0", 0, "Runs automatically when you fire");
    m_pRunAttack2 = new ConVar(PREFIX "aim_rononattack2", "0");
    m_pSilentAim = new ConVar(PREFIX "aim_silentaim", "0", 0);
    m_pAltKnifeAim = new ConVar(PREFIX "aim_knifer", "0");
    m_pAutoshoot = new ConVar(PREFIX "aim_auto", "0");
    m_pEspAimed = new ConVar(PREFIX "esp_showaimbotted", "1");
    m_pAimBodypart = new ConVar(PREFIX "aim_bodypart", "0", 0, "0 = head; 1 = chest; 2 = pelvis; 3 = feet");
    m_pAttackAimed = new ConVar(PREFIX "aim_attackaimed", "1");
    m_pStartAim = new ConCommand("+" PREFIX "aim", StartAim, "Start autoaiming", 0);
    m_pEndAim = new ConCommand("-" PREFIX "aim", EndAim, "Stop autoaiming", 0);
    m_bRunning = false;
    g_pAimbot = this;
}
CAimbot::~CAimbot(){
    FreeConCMD(m_pRunAttack);
    FreeConCMD(m_pSilentAim);
    FreeConCMD(m_pAltKnifeAim);
    FreeConCMD(m_pAutoshoot);
    FreeConCMD(m_pEspAimed);
    FreeConCMD(m_pAimBodypart);
    FreeConCMD(m_pAttackAimed);
    FreeConCMD(m_pStartAim);
    FreeConCMD(m_pEndAim);
}