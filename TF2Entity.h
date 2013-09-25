#pragma once

#include "Required.h"
#include "TF2Player.h"

namespace Skim
{
    namespace TF2Entity
    {
        inline TF2Player* GetBaseEntity( int Index )
        {
            IClientEntity* pEnt = Skim::ValveInterfaces::pClientEntList->GetClientEntity( Index );

            // do not want to call the pointer to getbaseentity() if it is null
            if(! pEnt )//|| pEnt->IsDormant() )
                return 0;
            
            return (TF2Player*)pEnt->GetBaseEntity();
        }

        inline TF2Player* GetPlayer(int index)
        {
            return (TF2Player*)GetBaseEntity(index);
        }

        inline TF2Player* Me ( )
        {
            return (TF2Player*)GetBaseEntity( Skim::ValveInterfaces::pEngine->GetLocalPlayer() );
        }
        //TF2Weapon* ActiveWeapon(TF2Player* pl)
        //{
        //    EHANDLE hActiveWeapon = CHandle<C_BaseEntity>(((CBaseEntity*)pl) + TF2Offsets::BaseCombatCharacter::hActiveWeapon);

        //    return (TF2Weapon*)dynamic_cast< C_BaseCombatWeapon* >( ValveInterfaces::pClientEntList->GetClientEntityFromHandle( hActiveWeapon ) );
        //}
        player_info_t * GetInfo( TF2Player* player);
        
        inline float DistanceFrom( TF2Player* a, TF2Player* b)
        {
            if ( !a || !b )
                return 0.0f;
            Vector delta = a->vecOrigin() - b->vecOrigin();
            return delta.Length();
        }
        inline float DistanceFromMe ( TF2Player* it )
        {
            return DistanceFrom( Me(), it );
        }
    };
};