#include "SkimWindow.h"
using namespace Drawing;
//----------------------------------------
//--------Constructor & destructor--------
//----------------------------------------
CSkimWindow::CSkimWindow(ConVar* enabled, ConVar* x, ConVar*y, ConVar* width, ConVar* height, const char* windowName, bool sizeable){
    m_pEnabled = enabled;
    m_pX = x;
    m_pY = y;
    m_pW = width;
    m_pH = height;
    m_sName = windowName;
    m_bMainWindow = sizeable;

    // Initialize stuff
    this->m_bClicked = false;
    this->m_bSelected = false;
    this->m_cpPrevious = CartPt(0,0);
    this->m_iDragtype = Dragtype::None;
    this->m_iSelectedButton = -1;
    this->m_vInteractives = new std::vector<IMouseInteractive*>();
}
CSkimWindow::~CSkimWindow() { m_vInteractives->clear(); }

//----------------------------------------
//--------------Add functions-------------
//----------------------------------------
void CSkimWindow::AddButton( ConCommand* func, const char* name, const char* parent )
{
    SkimButton* dat = new SkimButton( 0, 0, STD_BUTTON_WIDTH, STD_BUTTON_HEIGHT, name? name : func->GetName(), parent, func );
    m_vInteractives->push_back( dat );
}
void CSkimWindow::AddFilter( const char* name, const char* parent)
{
    SkimFilter* dat = new SkimFilter( 0, 0, STD_BUTTON_WIDTH, STD_BUTTON_HEIGHT, name, parent );
    m_vInteractives->push_back( dat );
}
void CSkimWindow::AddSlider(ConVar* val, const char* name, const char* parent)
{
    SkimSlider* dat = new SkimSlider( 0, 0, STD_BUTTON_WIDTH, STD_BUTTON_HEIGHT, name? name : val->GetName(), parent, val );
    m_vInteractives->push_back( dat );
}
SkimEnumButton* CSkimWindow::AddEnumeration(ConVar* val, const char* name, const char* parent)
{
    SkimEnumButton* dat = new SkimEnumButton( 0, 0, STD_BUTTON_WIDTH, STD_BUTTON_HEIGHT, name? name : val->GetName(), parent, val );
    m_vInteractives->push_back(dat);
    return dat;
}

//----------------------------------------
//-----------Misc. helper stuff-----------
//----------------------------------------
void CSkimWindow::GetWindowBounds(int& otop, int& obot, int& olef, int& orig, int& top, int& bott, int& left, int& righ, bool pointsOnScreen)
{
    // Draw the stuff that goes around the panel we cover, size specified in convars
    // Points inside, original points
    otop = pointsOnScreen? m_pY->GetInt() - 1 : 0;
    olef = pointsOnScreen? m_pX->GetInt() - 1 : 0;
    orig = olef + m_pW->GetInt() + 2;

    if ( m_pEnabled->GetBool() )
    {
        obot = otop + m_pH->GetInt() + 2;//vroom vroom
    }
    else 
    {
        obot = otop;
    }

    top  = otop;
    bott = obot;
    left = olef;
    righ = orig;
    top  -= WINDOW_TOP_THICK;
    bott += WINDOW_SIDES_THICK;
    left -= WINDOW_SIDES_THICK;
    righ += WINDOW_SIDES_THICK;
}
int CSkimWindow::MovetypeAtMousepos( const Drawing::CartPt& loc )
{
    if (!m_bMainWindow)
        return Dragtype::None;
    int result = 0;
    // point inside window
    CartPt inWin = loc - CartPt( m_pX->GetInt(), m_pY->GetInt() );
    int otop = 0, obottom = 0, oleft = 0, oright = 0;
    int top = 0, bottom = 0, left = 0, right = 0;
    GetWindowBounds(otop,obottom,oleft,oright,top,bottom,left,right,false);
    // We're sizing it vertically
    if ((inWin.x >= left && inWin.x <= right && inWin.y >= top && inWin.y < top + WINDOW_SIZE_THICK ) ||
        (inWin.x >= left && inWin.x <= right && inWin.y <= bottom && inWin.y > bottom - WINDOW_SIZE_THICK ))
    {
        result = Dragtype::SizeWidth;
    }
    // We're sizing it horizontally
    if ((inWin.y >= top && inWin.y <= bottom && inWin.x >= left && inWin.x < left + WINDOW_SIZE_THICK) ||
        (inWin.y >= top && inWin.y <= bottom && inWin.x <= right && inWin.x > right - WINDOW_SIZE_THICK))
    {
        result |= Dragtype::SizeHorizontal;
    }
    if (result)
        return result;
    return Dragtype::Move;
}
bool CSkimWindow::IsInWindow( const Drawing::CartPt& loc )
{
    int t=0,b=0,l=0,r=0,ot=0,ob=0,ol=0,or=0;
    GetWindowBounds(ot,ob,ol,or,t,b,l,r,true);
    bool inWindow = (loc.y >= t && loc.y <= b && loc.x <= r && loc.x >= l);
    if (inWindow)
        return true;
    int len = m_vInteractives->size();
    for (int i = 0; i < len; i++)
    {
        if (m_vInteractives->at(i)->Inside( loc.x, loc.y ) )
            return true;
    }
    return false;
}

//----------------------------------------
//--------------Mouse events--------------
//----------------------------------------
int CSkimWindow::SelectedButton( const Drawing::CartPt& loc )
{
    int len = m_vInteractives->size();
    // we were on a button before
    // see if we're still on it or moved up or down on the list it's parented to or moved back to its parent or moved to its children
    // m_bShown will be set if either
    if (m_iSelectedButton != -1)
    {
        if (m_vInteractives->at(m_iSelectedButton)->Inside( loc.x, loc.y ))
            return m_iSelectedButton;
    }
    for (int i = 0; i < len; i++)
    {
        IMouseInteractive* that = m_vInteractives->at(i);
        if (that->Inside( loc.x, loc.y ) && that->m_bShown)
            //( i == m_iSelectedButton || that->m_iParent == btnsParent || that->m_iParent == m_iSelectedButton))
            return i;
    }
    return -1;
}
void CSkimWindow::MouseBrowsing( const Drawing::CartPt& loc )
{
    m_bSelected = true;
    m_bClicked = false;
    // Update buttons, determine if they're visible
    m_iSelectedButton = SelectedButton( loc );
    int len = m_vInteractives->size();

    // The cursor is inside the window
    if ( m_iSelectedButton == -1 ) 
    {
        m_iDragtype = MovetypeAtMousepos(loc);
        for (int i = 0; i < len; i++)
        {
            m_vInteractives->at(i)->m_bHighlighted = false;
            if (m_vInteractives->at(i)->m_iParent != -1)
                m_vInteractives->at(i)->m_bShown = false;
        }
        return;
    }
    else
        m_iDragtype = Dragtype::None;

    IMouseInteractive* selected = m_vInteractives->at(m_iSelectedButton);
    int selectedParentParent = 0;
    if (selected->m_iParent != -1)
        selectedParentParent = m_vInteractives->at(selected->m_iParent)->m_iParent;

    for( int i = 0; i < len; i++ )
    {
        IMouseInteractive* that = m_vInteractives->at(i);
        that->m_bHighlighted = false;
        // is selected button a part of the list that selected is parented to,
        // or a child of the selected button,
        // or the selected button
        // also reset their highlighted status while we're here
        if (that->m_iParent == -1 || i == m_iSelectedButton || that->m_iParent == m_iSelectedButton || selectedParentParent == that->m_iParent || that->m_iParent == selected->m_iParent)
            that->m_bShown = true;
        else
            that->m_bShown = false;
    }
    selected->m_bHighlighted = true;
}
void CSkimWindow::MouseClicking( const Drawing::CartPt& loc )
{
    // Was clicking on previous frame
    // Figure out if we need to click a button, move window, or resize
    CartPt difference = loc - m_cpPrevious;
    if (m_bClicked)
    {
        // Tell button to drag
        if (m_iSelectedButton != -1)
        {
            IMouseInteractive* asdf = m_vInteractives->at(m_iSelectedButton);
            CartPt locInsideBtn = loc - CartPt(asdf->x,asdf->y);
            asdf->Drag(locInsideBtn);
        }
        // We're not on a button
        // Move this window
        else if (m_iDragtype == Dragtype::Move)
        {
            m_pX->SetValue( m_pX->GetInt() + difference.x );
            m_pY->SetValue( m_pY->GetInt() + difference.y );
            MoveInteractives(difference);
        }
        // resize appropriately
        else if (m_bMainWindow)
        {
            if ( m_iDragtype & Dragtype::SizeHorizontal )
            {
                m_pW->SetValue( m_pW->GetInt() + difference.x );
            }
            if ( m_iDragtype & Dragtype::SizeWidth && m_pEnabled->GetBool())
            {
                m_pH->SetValue( m_pH->GetInt() + difference.y );
            }
        }
    }
    // Just clicked
    // Gotta set up buttons by calling mousebrowsing, it decides what button we're on
    else
    {
        MouseBrowsing( loc );
        m_bClicked = true;
        m_iDragtype = 0;
        if ( m_iSelectedButton != -1 )
        {
            // Since we just clicked, send a click event
            IMouseInteractive* asdf = m_vInteractives->at(m_iSelectedButton);
            CartPt locInsideBtn = loc - CartPt(asdf->x,asdf->y);
            asdf->OnClick(locInsideBtn);
        }
        // Not clicking a button, figure out our movetype
        else
        {
            m_iDragtype = MovetypeAtMousepos( loc );
        }
    }
    m_cpPrevious = loc;
}
void CSkimWindow::FocusOut()
{
    m_bSelected = false;
    int len = m_vInteractives->size();
    for (int i = 0; i < len; i++)
    {
        if (m_vInteractives->at(i)->m_iParent != -1)
            m_vInteractives->at(i)->m_bShown = false;
        m_vInteractives->at(i)->m_bHighlighted = false;
    }
}

//----------------------------------------
//-----------------Render-----------------
//----------------------------------------
void CSkimWindow::Render()
{
    if (m_bMainWindow)
    {
        int otop = 0, obottom = 0, oleft = 0, oright = 0;
        int top = otop, bottom = obottom, left = oleft, right = oright;
        GetWindowBounds(otop,obottom,oleft,oright,top,bottom,left,right,true);

        const static Color blue (0, 116, 210, 255);
        const static Color white (255, 255, 255, 255);
        // Top, left, right, bottom
        // No overdraw
        Drawing::DrawFilledBox(left, top, right, otop, blue);
        Drawing::DrawFilledBox(left, otop, oleft, obottom, blue);
        Drawing::DrawFilledBox(oright, otop, right, obottom, blue);
        Drawing::DrawFilledBox(left, obottom, right, bottom, blue);

        Drawing::DrawBox(left, top, right, bottom, white);
        Drawing::DrawBox(oleft, otop, oright, obottom, white);

        //Drawing::DrawStringCenter(oleft - ((oleft - oright)/2), top - ((top - otop) / 2), white, m_sName);
    }
    // Loop through interactive list and render them if they should be shown
    int len = m_vInteractives->size();
    for (int i = 0; i < len; i++)
    {
        if (m_vInteractives->at(i)->m_bShown)// || m_vInteractives->at(i)->m_bHighlighted) Should be shown if highlighted
            m_vInteractives->at(i)->Render();
    }
}

//----------------------------------------
//----------------Finalize----------------
//----------------------------------------
//  Notes:
//    Sets all the interactives' positions
//----------------------------------------
int FindParent(const std::vector<IMouseInteractive*>* parentList, const char* parentName) {
    int len = parentList->size();
    for (int i = 2; i < len; i++) 
    {
        //all strings are constant & made at runtime, all point to same thing in string pool
        if (parentList->at(i)->name == parentName)
            return i;
    }
    return 0;
}
// Takes in the list and the button's parent's name
//   e.g. asdf is parented to ghjk is parented to qwert = 2 depth
// I hate recursion but Comp Sci II has gotten me to just accept that it has uses
int FindParentDepth(const std::vector<IMouseInteractive*>* butts, const char* parentName)
{
    int parentIndex = FindParent(butts, parentName);
    if (parentIndex)
        return FindParentDepth(butts, butts->at(parentIndex)->parent) + 1;
    return 0;
}
void CSkimWindow::Finalize()
{
    int len = m_vInteractives->size();
    IMouseInteractive* mainBtn = 0;
    if (len)
    {
        mainBtn = m_vInteractives->at(0);
    }
    
    for (int i = 0; i < len; i++) 
    {
        IMouseInteractive* that = m_vInteractives->at(i);
        if (that->m_iParent != -1) 
        {
            that->m_iParent = FindParent(m_vInteractives, that->parent);
            that->x = mainBtn->x + (FindParentDepth(m_vInteractives, that->parent) * STD_BUTTON_WIDTH);
        }
        else if (that->m_iParent != -1)
            that->m_iParent = 0;
    }

    // All parent indexes are set
    // We assume that children were added to the list after the parent was added
    // Loop through everything, see what is parented to it, set its y position accordingly
    for (int parentIndex = 0; parentIndex < len; parentIndex++) 
    {
        // if the interactive at this index is parented to the main menu, move it down so it doesn't cover it
        IMouseInteractive* parent = m_vInteractives->at(parentIndex);
        int parentCount = 0;
        for (int childIndex = parentIndex + 1; childIndex < len; childIndex++)
        {
            IMouseInteractive* child = m_vInteractives->at(childIndex);
            if (child->m_iParent == parentIndex)
            {
                child->y += parent->y + (STD_BUTTON_HEIGHT * parentCount);// + mainBtn->height;
                parentCount++;
            }
        }
    }

    // Gotta move errything down so it will not overlap the main button
    for (int i = 0; i < len; i++)
    {
        IMouseInteractive* that = m_vInteractives->at(i);
        //if (that->m_iParent == 0)
        //    that->x = m_pX->GetInt() + 17;
        if (that->m_iParent != -1)
        {
            that->y += mainBtn->height;
            that->m_bShown = true;
        }
        //if (that->m_iParent > 0)
    }
}
// Move Interactives by this much
void CSkimWindow::MoveInteractives( const Drawing::CartPt& difference ){
    int len = m_vInteractives->size();
    for (int i = 0; i < len; i++)
    {
        m_vInteractives->at(i)->x += difference.x;
        m_vInteractives->at(i)->y += difference.y;
    }
}
