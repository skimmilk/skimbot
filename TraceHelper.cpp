#include "TraceHelper.h"


namespace Skim{
namespace TraceHelper{
    //TraceFilter to hit only players
    class CPlayerFilter : public ITraceFilter
    {
    public: 
        CPlayerFilter(int indexIgnore){ ignore = indexIgnore;}
        bool ShouldHitEntity( IHandleEntity *pServerEntity, int contentsMask )
        {
            TF2Player *ent = (TF2Player*)pClientEntList->GetClientEntityFromHandle(pServerEntity->GetRefEHandle())->GetBaseEntity();
            return ent->bValidTeam() && ent->index() != ignore;
        }
        virtual TraceType_t    GetTraceType() const
        {
            return TRACE_EVERYTHING;
        }

    private:
        int ignore;
    };

    //----------------------------------------
    //-Returns what the shooter is looking at-
    //----------------------------------------
    //    Notes:
    //----------------------------------------
    int GetAimIndex ( Vector vecAbsStart, QAngle shooterangs, int indexIgnore )
    {
        //TODO: fix player filter, or fix whatever is making indexIgnore ignore wrong stuff
        CPlayerFilter* filter = new CPlayerFilter(0);
        trace_t tr;
        Vector vecDir;
        AngleVectors( shooterangs, &vecDir );

        Ray_t ray;
        ray.Init( vecAbsStart, vecAbsStart + (vecDir * 8192) );

        Skim::ValveInterfaces::pEngineTrace->TraceRay( ray, MASK_SHOT , filter, &tr );
        delete filter;

        if (tr.m_pEnt)
            return tr.m_pEnt->entindex();
        return 0;
    }
    int GetAimIndex( TF2Player* shooter, int indexIgnore)
    {
        return GetAimIndex(shooter->vecEyeOrigin(), shooter->angEyeAngles(), indexIgnore);
    }

    //----------------------------------------
    //-Determins if players can see eachother-
    //----------------------------------------
    bool CanSee( TF2Player* a, TF2Player* b)
    {
        CPlayerFilter* filter = new CPlayerFilter(a->index());
        trace_t tr;
        Ray_t ray;
        Vector start;
        Vector end; 

        a->GetHitboxPos(start,HITBOX_HEAD);
        b->GetHitboxPos(end, HITBOX_HEAD);

        ray.Init(start, end);

        Skim::ValveInterfaces::pEngineTrace->TraceRay( ray, MASK_SHOT, filter , &tr );
        delete filter;

        return tr.m_pEnt == (CBaseEntity*)b;
    }
    inline bool CanSee( CBaseEntity* a, CBaseEntity* b)
    {
        return CanSee( (TF2Player*)a, (TF2Player*)b );
    }

    // Distance from where the player is aiming from them
    float DistFromPlrAim(TF2Player* plr) {
        CPlayerFilter* filter = new CPlayerFilter(0);
        trace_t tr;
        Vector vecDir;
        AngleVectors( plr->angEyeAngles(), &vecDir );

        Ray_t ray;
        ray.Init( plr->vecOrigin(), plr->vecOrigin() + (vecDir * 8192) );

        Skim::ValveInterfaces::pEngineTrace->TraceRay( ray, MASK_SHOT , filter, &tr );
        delete filter;

        return tr.endpos.Length();
    }

    // No copy constructors are allowed for CGameTrace, cannot return an existing trace_t, must pass through reference or pointer
    void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, trace_t &tr, int mask)
    {
        Vector vecDir;

        Ray_t ray;
        ray.Init( vecAbsStart, vecAbsEnd );

        Skim::ValveInterfaces::pEngineTrace->TraceRay( ray, mask , 0, &tr );
    }
};
};