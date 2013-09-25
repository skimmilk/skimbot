#pragma once
#include "Required.h"
#include "MouseInteractive.h"

#define WINDOW_TOP_THICK 16
#define WINDOW_SIDES_THICK 8
#define WINDOW_SIZE_THICK 8

struct Dragtype {
    enum Enum { None = 0, Move = 1, SizeHorizontal = 2, SizeWidth = 4, SizeBoth = (SizeHorizontal | SizeWidth) };
};

class CSkimWindow
{
    friend class CSkimGui;
public:
    // Sliders modify a convar's value, convars need to have a min and max to work
    void AddSlider(ConVar* val, const char* name = 0, const char* parent = 0);
    // Buttons are commands that can be called
    void AddButton(ConCommand* func, const char* name = 0, const char* parent = 0);
    // Filters are like folders that hold attributes, functions, and other filters
    void AddFilter(const char* name, const char* parent = 0);
    // Creates a new enumerator button that you can add to
    SkimEnumButton* AddEnumeration(ConVar* var, const char* name = 0, const char* parent = 0);

    ~CSkimWindow();
protected:
    // Called by GUI only
    CSkimWindow(ConVar* enabled, ConVar* x, ConVar*y, ConVar* width, ConVar* height, const char* windowName, bool sizeable);

    // Done adding stuff
    void Finalize();

    // Mouse no longer in this window, tell all interactives to reset and stuff
    void FocusOut();

    void Render();
    bool IsInWindow( const Drawing::CartPt& loc );
    // Called whenever the mouse is clicking in this
    void MouseClicking( const Drawing::CartPt& loc );
    // Called when the mouse is inside this not clicking
    void MouseBrowsing( const Drawing::CartPt& loc );
    // Function assumes mouse is inside window and not on a button
    int MovetypeAtMousepos( const Drawing::CartPt& loc );

    // This is set to false only if this is the main window gui window thing
    bool m_bMainWindow;
    // Not used in window, viewed by gui, set in window
    bool m_bSelected;
    // Are we clicking?
    bool m_bClicked;

    // This will be accessed by GUI to set up the close button and stuff
    // Needs to be sent over by pointer, the virtual tables keep being overwritten to point to the base classes's
    std::vector<IMouseInteractive*>* m_vInteractives;
private:
    void GetWindowBounds(int& otop, int& obot, int& olef, int& orig, int& top, int& bott, int& left, int& righ, bool pointsOnScreen);
    // Move Interactives by this much
    void MoveInteractives(const Drawing::CartPt& difference );
    // returns array index of pointed
    int SelectedButton( const Drawing::CartPt& loc );

    // pointer to button in array
    int m_iSelectedButton;
    // Previous mouse position
    Drawing::CartPt m_cpPrevious;
    // How we are moving, if we are
    int m_iDragtype;

    ConVar *m_pEnabled, *m_pX, *m_pY, *m_pW, *m_pH;
    const char* m_sName;
}; 