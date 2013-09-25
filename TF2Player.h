//Created: Nov. 19, 2011
//Modified: Aprl. 13, 2012

#pragma once
#include "Required.h"
#include "TF2Weapon.h"
#include "TF2Offsets.h"
#include "Interfaces.h"
#include "TF2.h"

enum TF2Class { Scout = 1, Sniper, Soldier, Demoman, Medic, Heavy, Pyro, Spy, Engineer };
enum hitboxes_t
{
    HITBOX_ENGINEER_HIP_R = 14,
    HITBOX_ENGINEER_KNEE_R = 15,
    HITBOX_ENGINEER_SPINE_0 = 1,
    HITBOX_ENGINEER_SPINE_1 = 2,
    HITBOX_ENGINEER_SPINE_2 = 3,
    HITBOX_ENGINEER_SPINE_3 = 4,
    HITBOX_ENGINEER_HEAD = 0,
    HITBOX_ENGINEER_HIP_L = 11,
    HITBOX_ENGINEER_KNEE_L = 12,
    HITBOX_ENGINEER_UPPERARM_L = 5,
    HITBOX_ENGINEER_LOWERARM_L = 6,
    HITBOX_ENGINEER_UPPERARM_R = 8,
    HITBOX_ENGINEER_LOWERARM_R = 9,
    HITBOX_ENGINEER_FOOT_L = 13,
    HITBOX_ENGINEER_FOOT_R = 16,
    HITBOX_ENGINEER_HAND_R = 10,
    HITBOX_ENGINEER_HAND_L = 7,

    HITBOX_PELVIS = 1,
    HITBOX_SPINE_0 = 2,
    HITBOX_SPINE_1 = 3,
    HITBOX_SPINE_2 = 4,
    HITBOX_SPINE_3 = 5,
    HITBOX_HEAD = 0,
    HITBOX_HIP_R = 15,
    HITBOX_KNEE_R = 16,
    HITBOX_FOOT_R = 17,
    HITBOX_UPPERARM_L = 6,
    HITBOX_LOWERARM_L = 7,
    HITBOX_HAND_L = 8,
    HITBOX_HIP_L = 12,
    HITBOX_KNEE_L = 13,
    HITBOX_FOOT_L = 14,
    HITBOX_SCOUT_PACKMIDDLE = 18,
    HITBOX_UPPERARM_R = 9,
    HITBOX_LOWERARM_R = 10,
    HITBOX_HAND_R = 11,

    HITBOX_ORIGIN = -1
};
struct BonePos {
    enum Enum { MAX, MIN, MIDDLE };
};

using namespace Skim;
using namespace Skim::ValveInterfaces;
using namespace Skim::Weapons;
using namespace Skim::TF2Offsets;

struct TF2Player
{
    inline bool isDormant()                    { return ((CBaseEntity*)this)->IsDormant(); }
    inline bool isAlive()                    { return lifeState() > 256; }
    inline int index()                        { return ((CBaseEntity*)this)->index; }
    inline float flPlayerHeight()            { return *(float*)(this + BaseEnt::flPlayerHeight); }
    inline int iHealth()                    { return *(int*)(this + BaseEnt::iHealth); }
    inline Vector vecOrigin()                { return *(Vector*)(this + BaseEnt::vecOrgin ); }
    inline Vector vecEyeOrigin()            { return Vector(vecOrigin().x, vecOrigin().y, vecOrigin().z + flPlayerHeight()); }
    inline QAngle angEyeAngles()            { if(pEngine->GetLocalPlayer()==index())return*(QAngle*)(this+BaseEnt::angRotationLocal);return*(QAngle*)(this+BaseEnt::angRotation); }
    inline float flCloakMeter()                { return *(float*)(this + BaseEnt::shared + TFPlayerShared::flCloakMeter); }
    inline float flChargeMeter()            { return *(float*)(this + BaseEnt::shared + TFPlayerShared::flChargeMeter); }
    inline int iRevengeCrits()                { return *(int*)(this + BaseEnt::shared + TFPlayerShared::iRevengeCrits); }
    inline float flEnergyDrinkMeter()        { return *(float*)(this + BaseEnt::shared + TFPlayerShared::flEnergyDrinkMeter); }
    inline int iStunFlags()                    { return *(int*)(this + BaseEnt::shared + TFPlayerShared::iStunFlags); }
    inline int nDisguiseTeam()                { return *(int*)(this + BaseEnt::shared + TFPlayerShared::nDisguiseTeam); }
    inline int nDisguiseClass()                { return *(int*)(this + BaseEnt::shared + TFPlayerShared::nDisguiseClass); }
    inline int nPlayerCond()                { return *(int*)(this + BaseEnt::shared + TFPlayerShared::nPlayerCond); }
    inline int nPlayerState()                { return *(int*)(this + BaseEnt::shared + TFPlayerShared::nPlayerState); }
    inline int playerClass()                { return *(int*)(this + BaseEnt::playerClass); }
    inline EHANDLE* hItem()                    { return (EHANDLE*)(this + BaseEnt::tfPlayer + TFPlayer::hItem); }
    inline int iDisguiseTargetIndex()        { return *(int*)(this + BaseEnt::shared + + TFPlayerShared::iDisguiseTargetIndex); }
    inline bool isDisguised()                { return (((CBaseEntity*)(this))->index == iDisguiseTargetIndex());    }
    inline int iTeamNum()                    { return *(int*)(this + BaseEnt::iTeamNum); }
    inline bool bValidTeam()                { return iTeamNum() == 3 || iTeamNum() == 2; }
    inline bool isValid()                    { return bValidTeam() && isAlive(); }
    inline Vector vecChestOrigin()            { Vector v; GetHitboxPos(v, HITBOX_SPINE_2); return v; }
    inline Vector vecHeadOrigin()            { Vector v; GetHitboxPos(v, HITBOX_HEAD); return v; }
    inline int fFlags()                        { return *(int*)(this + BaseEnt::hBasePlayer + BasePlayer::fFlags); }
    inline int lifeState()                    { return *(int*)(this + BaseEnt::hBasePlayer + BasePlayer::lifeState); }
    
    
    inline int observerTargetIndex() {
        EHANDLE plyr = *(EHANDLE*)(CBaseEntity*)( this + BaseEnt::hBasePlayer + BasePlayer::hObserverTarget );
        return pClientEntList->GetClientEntity(plyr.GetEntryIndex())->GetBaseEntity()->index;
    }

    inline TF2Weapon* ActiveWeapon() {
        //if(!isValid()) return 0;
        EHANDLE wpn = *(EHANDLE*)( this + BaseEnt::baseCombatCharacter + BaseCombatCharacter::hActiveWeapon );
        return (TF2Weapon*)pClientEntList->GetClientEntity(wpn.GetEntryIndex())->GetBaseEntity();
    }
    inline int CorrectedHitbox(int normalHitbox) {
        if (playerClass() != Scout && normalHitbox == HITBOX_SCOUT_PACKMIDDLE ) return HITBOX_SPINE_2;
        if (playerClass() != Engineer) return normalHitbox;
        if (normalHitbox == HITBOX_HEAD) return normalHitbox;
        return normalHitbox - 1;
    }
    //This will get the correct hitbox before doing stuff
    bool GetHitboxPos(Vector&, int hitbox, BonePos::Enum pos = BonePos::MIDDLE);
};