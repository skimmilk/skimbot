#include "SkimGUI.h"

using namespace Drawing;

//----------------------------------------
//----------Static declarations-----------
//----------------------------------------
bool CSkimGUI::m_bShowMenu;
CSkimGUI* CSkimGUI::thisptr;
void CSkimGui::In_Showmenu(){thisptr->m_bShowMenu = true;}
void CSkimGui::Out_Showmenu(){thisptr->m_bShowMenu = false;}

void CSkimGui::ToggleSelectedWindow()
{
    int select = thisptr->m_iLastInWindow;
    if (select != -1){
        ConVar* enabled = thisptr->m_vWindows[select]->m_pEnabled;
        enabled->SetValue( ! enabled->GetBool() );
    }
}
ConCommand* CSkimGui::m_pCloseSelectedWindow;

//----------------------------------------
//--------Constructor & Destructor--------
//----------------------------------------
CSkimGUI::CSkimGUI()
{
    thisptr = this;
    m_bShowMenu = false;
    m_cpMouseAccum = CartPt(0,0);
    m_pInShowmenu = new ConCommand("+st_showmenu", CSkimGui::In_Showmenu);
    m_pOutShowmenu = new ConCommand("-st_showmenu", CSkimGui::Out_Showmenu);

    // stupid hack to get window working
    // a hack that hacks hacks
    m_pEnabled = new ConVar(PREFIX "gui", "1", 0, "", true, 1, true, 1);
    m_pX = new ConVar(PREFIX"gui_x", "0", 0, "", true, 0.f, true, 0.f);
    m_pY = new ConVar(PREFIX"gui_y", "20", 0, "", true, 20.f, true, 20.f);
    m_pWidth = new ConVar(PREFIX"gui_width", "128");
    m_pHeight = new ConVar(PREFIX"gui_height", "0", 0, "", true, 0.0f, true, 0.0f);
    m_pSensitivity = new ConVar(PREFIX"gui_mousesensitivity", "5", 0, "", true, 1.f, true, 15.f);

    m_pCloseSelectedWindow = new ConCommand(PREFIX"gui_togglewindow", CSkimGui::ToggleSelectedWindow);

    //CSkimWindow* datwindow = NewWindow(m_pEnabled, m_pX, m_pY, m_pWidth, m_pHeight, "Skimbot settings");
    CSkimWindow* datwindow = new CSkimWindow(m_pEnabled, m_pX, m_pY, m_pWidth, m_pHeight, "Skimbot settings", false);
    m_vWindows.push_back(datwindow);

    IMouseInteractive* dropdown = new SkimFilter( m_pX->GetInt() + 17, m_pY->GetInt() -15, 100, 15, "Skimbot settings", 0);
    dropdown->m_iParent = -1;
    dropdown->m_bShown = true;
    datwindow->m_vInteractives->push_back(dropdown);

    datwindow->m_bMainWindow = false;
    const char* naem = "GUI";
    const char* colors = "Colors";
    const char* high = "Btn Highlighted color";
    const char* reg = "Btn regular color";
    datwindow->AddFilter( naem );
    datwindow->AddFilter( colors );
    datwindow->AddFilter( high, colors );
    datwindow->AddFilter( reg, colors );
    datwindow->AddSlider(m_pSensitivity, "Mouse sensitivity", naem);
    //datwindow->Finalize();

    // Initialize stuff
    this->m_bShowMenu = false;
    this->m_cpMouseAccum = CartPt(0,0);
    this->m_cpMousepos = CartPt( pClient->GetScreenWidth() / 2, pClient->GetScreenHeight() / 2 );
    this->m_iLastInWindow = -1;
}
CSkimGUI::~CSkimGUI()
{
    FreeConCMD(m_pEnabled, false);
    FreeConCMD(m_pOutShowmenu);
    FreeConCMD(m_pInShowmenu);
    FreeConCMD(m_pX, false);
    FreeConCMD(m_pY, false);
    FreeConCMD(m_pWidth, false);
    FreeConCMD(m_pHeight, false);
}
void CSkimGUI::Initialize()
{
    int len = m_vWindows.size();
    for (int i = 0; i < len; i++)
    {
        m_vWindows[i]->Finalize();
    }
}

//----------------------------------------
//--------------Add functions-------------
//----------------------------------------
CSkimWindow* CSkimGui::NewWindow(ConVar* enabled, ConVar* x, ConVar*y, ConVar* width, ConVar* height, const char* windowName)
{
    CSkimWindow* toAdd = new CSkimWindow(enabled,x,y,width,height,windowName,true);

    IMouseInteractive* dropdown = new SkimFilter( x->GetInt() + 17, y->GetInt() -15, 100, 15, windowName, 0);
    dropdown->m_iParent = -1;
    dropdown->m_bShown = true;
    toAdd->m_vInteractives->push_back(dropdown);

    IMouseInteractive* toggle = new SkimButton( x->GetInt() + 1, y->GetInt() -15, 15, 15, "X", 0, m_pCloseSelectedWindow);
    toggle->m_iParent = -1;
    toggle->m_bShown = true;
    toAdd->m_vInteractives->push_back(toggle);
    thisptr->m_vWindows.push_back(toAdd);
    return toAdd;
}
void CSkimGui::NewHackGroup(const char* name)
{
    CSkimWindow* datmain = thisptr->m_vWindows[0];
    datmain->AddFilter(name);
}
void CSkimGui::AddButton(const char* name, ConCommand* func, const char* parent)
{
    CSkimWindow* datmain = thisptr->m_vWindows[0];
    datmain->AddButton(func,name,parent);
}
void CSkimGui::AddFilter(const char* name, const char* parent)
{
    CSkimWindow* datmain = thisptr->m_vWindows[0];
    datmain->AddFilter(name,parent);
}
void CSkimGui::AddSlider(const char* name, ConVar* var, const char* parent)
{
    CSkimWindow* datmain = thisptr->m_vWindows[0];
    datmain->AddSlider(var,name,parent);
}
SkimEnumButton* CSkimGui::AddEnumerator(const char* name, ConVar* var, const char* parent)
{
    CSkimWindow* datmain = thisptr->m_vWindows[0];
    SkimEnumButton* yep = datmain->AddEnumeration(var,name,parent);
    return yep;
}

//----------------------------------------
//-------------On tick events-------------
//----------------------------------------
int CSkimGui::GetClickedWindow()
{
    int len = m_vWindows.size();
    for(int i = len - 1; i >= 0; i--)
    {
        // wanna select the topmost window
        if (m_vWindows[i]->IsInWindow(m_cpMousepos)){
            return i;
        }
    }
    return -1;
}
void CSkimGui::Run(CUserCmd* pCmd)
{
    if (m_bShowMenu){

    // Get mouse accum
    // Check what window the mouse is over
    m_cpMouseAccum.x = ((float)pCmd->mousedx * (m_pSensitivity->GetFloat() / 15.f));
    m_cpMouseAccum.y = ((float)pCmd->mousedy * (m_pSensitivity->GetFloat() / 15.f));
    //if ( abs(m_cpMouseAccum.x) < 15)
    //    m_cpMouseAccum.x /= 2;
    //if ( abs(m_cpMouseAccum.y) < 15)
    //    m_cpMouseAccum.y /= 2;
    pCmd->mousedx = 0;
    pCmd->mousedy = 0;

    m_cpMousepos += m_cpMouseAccum;
    if (m_cpMousepos.x < 0)
        m_cpMousepos.x = 0;
    else if (m_cpMousepos.x > pClient->GetScreenWidth())
        m_cpMousepos.x = pClient->GetScreenWidth();
    if (m_cpMousepos.y < 0)
        m_cpMousepos.y = 0;
    else if (m_cpMousepos.y > pClient->GetScreenHeight())
        m_cpMousepos.y = pClient->GetScreenHeight();
    // we're clicking
    if (pCmd->buttons & IN_ATTACK) 
    {
        // we're moving stuff around or clicking
        if (m_iLastInWindow != -1)
        {
            m_vWindows[m_iLastInWindow]->MouseClicking(m_cpMousepos);
        }
        // We just clicked
        // Find the window we're in and fire its events
        else
        {
            int windowIndex = GetClickedWindow();
            if (windowIndex != -1){
                m_iLastInWindow = windowIndex;
                m_vWindows[windowIndex]->MouseClicking(m_cpMousepos);
            }
        }
    }
    // could still be browsing around
    else 
    {
        int windowIndex = GetClickedWindow();
        if (windowIndex != m_iLastInWindow && m_iLastInWindow != -1)
            // Changed windows, focus changed so tell old window
            m_vWindows[m_iLastInWindow]->FocusOut();
        else if (windowIndex != -1)
            m_vWindows[windowIndex]->MouseBrowsing(m_cpMousepos);
        m_iLastInWindow = windowIndex;

    }

    // Clean up
    pCmd->buttons &= ~IN_ATTACK;
    pCmd->viewangles = m_pOldCmd->viewangles;
    pEngine->SetViewAngles(pCmd->viewangles);
    //pointers change on every tick
    }
    m_pOldCmd = pCmd;
}
void DrawStandardMouse(int x, int y,int size)
{
    Color white(255,255,255,255);
    for (int i = 0; i < size; i++)
    {
        Drawing::DrawLine( x + i, y, x + i, y + size - i, white );
    }
    Drawing::DrawLine(x,y,x+size,y+size,white);
    Drawing::DrawLine(x+1,y,x+size+1,y+size,white);
    Drawing::DrawLine(x,y+1,x+size,y+size+1,white);
}
void DrawSizeMouse(int x, int y, int d)
{
    Color white(255,255,255,255);
    if (d == Dragtype::SizeHorizontal)
        Drawing::DrawFilledBox(x-8,y-2,x+8,y+2,white);
    else if (d == Dragtype::SizeWidth)
        Drawing::DrawFilledBox(x-2,y-8,x+2,y+8,white);
    else if (d == Dragtype::SizeBoth)
    {
        Drawing::DrawLine(x-4,y-4,x+4,y+4,white);
        Drawing::DrawLine(x-4,y+4,x+4,y-4,white);
        x++;y++;
        Drawing::DrawLine(x-4,y-4,x+4,y+4,white);
        Drawing::DrawLine(x-4,y+4,x+4,y-4,white);
        x -= 2; y-= 2;
        Drawing::DrawLine(x-4,y-4,x+4,y+4,white);
        Drawing::DrawLine(x-4,y+4,x+4,y-4,white);
    }
    else if (d == Dragtype::Move)
    {
        Drawing::DrawFilledBox(x-8,y-2,x+8,y+2,white);
        Drawing::DrawFilledBox(x-2,y-8,x+2,y+8,white);
    }
}
void CSkimGui::Run_PaintTraverse()
{
    if (!m_bShowMenu)
        return;
    int len = m_vWindows.size();
    for (int i = 0; i < len; i++) 
    {
        m_vWindows[i]->Render();
    }
    // draw tha mouse
    Color white(255,255,255,255);
    int x = m_cpMousepos.x, y = m_cpMousepos.y;

    if (m_iLastInWindow == -1) {
        DrawStandardMouse(x,y,8);
    }
    else
    {
        CSkimWindow* that = m_vWindows[m_iLastInWindow];
        if (that->m_iDragtype == Dragtype::None)
            DrawStandardMouse(x,y,8);
        else
            DrawSizeMouse(x,y,that->m_iDragtype);
    }
}