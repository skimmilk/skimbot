#include "Required.h"
#include "Interfaces.h"
#include "TF2Entity.h"
#include "IHack.h"

#define ARRAY_LENGH 33
// These tools are used to figure out players' velocities, how long they have been dormant, etc..
namespace Skim {

    class CTF2PlayerTools : public IHack{
    public:
        CTF2PlayerTools();
        void Run(CUserCmd *cmd);
        void Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active);

        //Velocities are per-frame
        static Vector PlayerVelocity(TF2Player*);
        static Vector PlayerVelocity(int index);

        static int PlayerDormantPeriod(TF2Player*);
        static int PlayerDormantPeriod(int index);

        //Don't use, try to use one with int frames, faster
        static Vector PredictPlayerLoc(TF2Player*, float seconds);
        static Vector PredictPlayerLoc(TF2Player*, int frames);

        static bool PlayerOnGround(TF2Player*);

        static CTF2PlayerTools* g_pTF2PlayerTools;
    private:
        float m_flFrameTime;
        int m_iDormantPeriods[ARRAY_LENGH];
        Vector vecCurrent[ARRAY_LENGH];
        Vector vecPast1[ARRAY_LENGH];
        Vector vecPast2[ARRAY_LENGH];
        bool m_bCanPredict;
    };
};