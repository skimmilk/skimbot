#include "IHack.h"

class CMiscHacks : public IHack {
public:
    CMiscHacks();
    ~CMiscHacks();

    void Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active);
    void Run(CUserCmd*);

    void UnPatchAttacks();
    void UnPatchMoveKeys();

    bool m_bAttack1;
    bool m_bAttack2;
    bool m_bforward;
    bool m_bback;
    bool m_bleft;
    bool m_bright;
    bool m_bjump;

private:
    bool m_bPatchedAttacks;
    bool m_bPatchedMovement;

    ConVar* m_pSlidingTaunt;
    ConVar* m_pForceAttacks;
    ConVar* m_pBhop;
    ConVar* m_pForceAngles;

    //original attack functions
    FnCommandCallbackV1_t m_fnInAttackOrig;
    FnCommandCallbackV1_t m_fnOutAttackOrig;

    FnCommandCallbackV1_t m_fnInAttack2Orig;
    FnCommandCallbackV1_t m_fnOutAttack2Orig;

    //attack concommands
    ConCommand* m_pInAttack;
    ConCommand* m_pOutAttack;

    ConCommand* m_pInAttack2;
    ConCommand* m_pOutAttack2;

    //original movement functions
    FnCommandCallbackV1_t m_fnIForw;
    FnCommandCallbackV1_t m_fnOForw;

    FnCommandCallbackV1_t m_fnIBack;
    FnCommandCallbackV1_t m_fnOBack;

    FnCommandCallbackV1_t m_fnIRigh;
    FnCommandCallbackV1_t m_fnORigh;

    FnCommandCallbackV1_t m_fnILeft;
    FnCommandCallbackV1_t m_fnOLeft;

    FnCommandCallbackV1_t m_fnIJump;
    FnCommandCallbackV1_t m_fnOJump;

    //movement commands
    ConCommand* m_pIForw;
    ConCommand* m_pOForw;

    ConCommand* m_pIBack;
    ConCommand* m_pOBack;

    ConCommand* m_pILeft;
    ConCommand* m_pOLeft;

    ConCommand* m_pIRigh;
    ConCommand* m_pORigh;

    ConCommand* m_pIJump;
    ConCommand* m_pOJump;

    //Speedhags
    ConCommand* m_pInSpeed;
    ConCommand* m_pOutSpeed;
    ConVar* m_pSpeedAmt;
    static ConVar* host_framerate;
    static ConVar* host_timescale;
    static void InSpeed();
    static void OutSpeed();
};

extern CMiscHacks* g_pMiscHacks;