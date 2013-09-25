#include "AimAndMovementHelper.h"
#include "TF2Entity.h"
#include "Nospread.h"

namespace Skim{
    namespace AimAndMovementHelper{
        //ang y is zero -> forward move == x vec add


        //Fix how you move, so you always move where you are currently looking, not moving by how your CUserCmd looks
        void SetAngles(float& forwardMove, float& sideMove, const float& angleFromView, const float& viewY)
        {
            //fixen
            sideMove *= -1;
            //results of trigonometry stuff is in rads, converting to deg where needed
            float angle_move = atan2(sideMove, forwardMove) * 180.0f / M_PI;
            //total movement
            float movetotal = hypotf(forwardMove, sideMove);

            //Don't know what is going on here anymore
            float angle_newmove = angleFromView - angle_move - viewY;

            //newx = r cos angle etc..
            //float is bad and innacurate, force these trigonometric identities or else
            if ((int)(angle_newmove + 0.5f) == 90 || ((int)(angle_newmove - 0.5f)== -90))
                forwardMove = 0.0f;
            else
                forwardMove = movetotal * cos(angle_newmove / 180.0f * M_PI);

            if (((int)(angleFromView + .5f) == 180) && ((int)(sideMove) == 0))
                sideMove = 0.0f;
            else
                sideMove = movetotal * sin(angle_newmove / 180.0f * M_PI);
        }

        inline void SetAngles(CUserCmd* me, const float& yangle)
        {
            SetAngles(me->forwardmove, me->sidemove, yangle, me->viewangles.y);
        }

        void AimAt(TF2Player* prey, CUserCmd* predator, int hitbox, bool silent)
        {
            //How many degrees do you have to look on y-axis and x
            //Adjust movement based on y look axis
            //Aim at him
            Vector aimAt;
            
            prey->GetHitboxPos(aimAt, hitbox);

            Vector me = TF2Entity::Me()->vecEyeOrigin();
            Vector vecdist = aimAt - me;


            float yAng = atan2(vecdist.y, vecdist.x) * 180.0f / M_PI;
            float xAng = atan2(vecdist.z, hypotf(vecdist.x, vecdist.y)) * -180.0f / M_PI;

            if (silent)
                SetAngles(predator,yAng);

            predator->viewangles.x = xAng;
            predator->viewangles.y = yAng;
        }

        float AngleDifference(float a1, float a2)
        {
            float aimDiff = a1 - a2;
            //if ang > 180 then -360 else if ang < -180 then add 360 else add 0
            return (aimDiff >180.0f) ? aimDiff -360 : (aimDiff<-180.0f) ? aimDiff + 360 : aimDiff;
        }
    };
};