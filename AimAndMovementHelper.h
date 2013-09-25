#pragma once
#include "Required.h"
#include "TF2Entity.h"

namespace Skim{
    namespace AimAndMovementHelper{
        namespace Bodypart{
            enum Enum{
                Feet, Middle, Head
            };
        };

        void SetAngles(float& forwardMove, float& sideMove, const float& angle, const float& viewY);
        inline void SetAngles(CUserCmd* me, const float& yangle);
        void AimAt(TF2Player* prey, CUserCmd* predator, int hitbox, bool silent);
        float AngleDifference(float,float); 
    };
};