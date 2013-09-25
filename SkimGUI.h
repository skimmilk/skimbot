#pragma once
#include "Required.h"
#include "IHack.h" 
#include "SkimWindow.h"
#include "Drawing.h"

#define CSkimGUI CSkimGui

//Window manager, controls where windows are and fires their events
class CSkimGui : public IHack
{
public:
    CSkimGui();
    ~CSkimGui();

    void Initialize();

    // Windows draw under whatever little panel that might be there
    static CSkimWindow* NewWindow(ConVar* enabled, ConVar* x, ConVar*y, ConVar* width, ConVar* height, const char* windowName);

    // Simple windows do not go over panels, they only are there for easy access to the hack's variables
    //static CSkimWindow* SimpleWindow(const char* windowName);

    // Add a hack group thing to the big window
    static void NewHackGroup(const char* name);
    // Add a filter to the big hax window
    static void AddFilter(const char* name, const char* hackNameOrParent);
    // Add a button to the big hax window
    static void AddButton(const char* name, ConCommand* func, const char* hackNameOrParent);
    // Add a slider to the big hax window
    static void AddSlider(const char* name, ConVar* var, const char* hackNameOrParent);
    // Add an enumerator
    static SkimEnumButton* AddEnumerator(const char* name, ConVar* var, const char* hackNameOrParent);
    // Add to the enumerator group
    //static void AddToEnumGroup(const char* name, int value, 
protected:
    void Run_PaintTraverse();
    void Run(CUserCmd*);
private:
    int GetClickedWindow();

    static CSkimGui* thisptr;

    CUserCmd* m_pOldCmd;
    // Index pointer to last window selected in list
    int m_iLastInWindow;
    std::vector<CSkimWindow*> m_vWindows;

    // Static stuff modified by static function concommands fire
    static bool m_bShowMenu;
    ConCommand* m_pInShowmenu;
    ConCommand* m_pOutShowmenu;
    static void In_Showmenu();
    static void Out_Showmenu();

    static void ToggleSelectedWindow();
    static ConCommand* m_pCloseSelectedWindow;

    ConVar *m_pEnabled, *m_pX, *m_pY, *m_pWidth, *m_pHeight, *m_pSensitivity;

    Drawing::CartPt m_cpMouseAccum;
    Drawing::CartPt m_cpMousepos;
};