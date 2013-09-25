#include "Required.h"
//#include "Interfaces.h"
#include "IHack.h"

class CSpinBot : public IHack
{
public:
    CSpinBot();
    ~CSpinBot();

    void Run_BeforeCM(int& sequence_number, float& input_sample_frametime, bool& active);
    void Run(CUserCmd *cmd);
    void Run_PaintTraverse();
    void PutAng(const CCommand &args);
    static CSpinBot* g_pInstance;
private:
    void Spin(CUserCmd*);
    void Reverse(CUserCmd*);
    void SetAng(CUserCmd*);
    inline bool CanGo(CUserCmd*);
    ConVar* m_enabled;
    ConVar* m_spinAmount;
    ConVar* m_stopOnA1;
    ConVar* m_stopOnA2;
    QAngle m_aOldAngle;
    QAngle m_aSetAngle;
    CUserCmd* m_pOldCmd;
    bool m_bRunning;
    ConCommand* c1;
};