#pragma once
#include "Required.h"
#include "Interfaces.h"
#include "TF2Entity.h"

namespace Skim{
namespace TraceHelper{
    //----------------------------------------
    //-Returns what the shooter is looking at-
    //----------------------------------------
    //    Notes:
    //        Used vector and qangle instead of
    //        tf2player, so its portable
    //----------------------------------------
    int GetAimIndex ( Vector vecAbsStart, QAngle shooterangs, int indexIgnore );
    int GetAimIndex( TF2Player* shooter, int indexIgnore);
    bool CanSee(TF2Player*, TF2Player*);
    inline bool CanSee(CBaseEntity*, CBaseEntity);

    // Distance from where the player is aiming from them
    float DistFromPlrAim(TF2Player* plr);

    // No copy constructors are allowed for CGameTrace, cannot return an existing CGameTrace, must pass through reference or pointer
    void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, trace_t &tr, int mask = MASK_SHOT);
};
};