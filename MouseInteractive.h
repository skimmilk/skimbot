#pragma once
#include "Required.h"
#include "Drawing.h"

#define STD_BUTTON_WIDTH 130
#define STD_BUTTON_HEIGHT 30

#pragma warning (disable : 4018 )

inline Color GetFillColor(bool highlighted)
{
    static const Color h = Color(255,127,0,150);
    static const Color n = Color(127,127,127,150);
    return highlighted?h:n;
}

class IMouseInteractive {
public:
    int x,y,width,height;

    // Pos in array located in SkimWindow
    int m_iParent;
    // The parent information stuff isn't going to be helpful for the interactive,
    //  it is only used for the window
    const char *name, *parent; 
    // Set by SkimWindow when the mouse is over this
    bool m_bHighlighted;
    // Set by window when this should be drawn
    bool m_bShown;

    // Called by SkimWindow only if this should be drawn
    virtual void Render(){DrawBG();}
    // OnClick is called only when loc is inside this
    // loc is a point between 0,0 and width,height
    virtual void OnClick( const Drawing::CartPt& loc ){}
    // Drag is called every frame after onClick
    virtual void Drag( const Drawing::CartPt& loc ){}

    inline bool Inside( int _x, int _y ) {
        return _x >= x && _y >= y && _x <= x+width && _y <= y+height; 
    }
    inline void Move(int _x, int _y){
        x += _x;
        y += _y;
    }
    static ConVar *r, *g, *b, *a, *hr, *hg, *hb, *ha;
private:
    IMouseInteractive();
protected:
    IMouseInteractive(int _x, int _y, int _width, int _height, const char* _name, const char* _parent);
    // This gets sorted out in the click and drag event
    inline Drawing::CartPt MakePointInside(Drawing::CartPt loc){
        loc.x -= x;
        loc.y -= y;
        return loc;
    } 
    inline void DrawBG() {
        Drawing::DrawFilledBox(x+1,y+1,x+width-1,y+height-1, GetFillColor(m_bHighlighted));
        Drawing::DrawBox(x,y,x+width,y+height, Color(200,200,200,255));
    }
};
class SkimFilter : public IMouseInteractive {
public:
    SkimFilter(int _x, int _y, int _width, int _height, const char* _name, const char* _parent);
    //~SkimFilter();

    void Render();
};
// Buttons; click to fire func 
class SkimButton : public IMouseInteractive {
public:
    SkimButton(int _x, int _y, int _width, int _height, const char* _name, const char* _parent, ConCommand* pFunc);
    //~SkimButton();

    void Render();
    void OnClick( const Drawing::CartPt& loc );
    //void Drag( Drawing::CartPt loc );

    //ConCommand* m_pFunc; 
    // Changed to just a function because it is hard to procedurally make concommands,
    //   I might add buttons in other interactives
    FnCommandCallbackV1_t m_fnFunc;
};
// Sliders; click and drag to modify var, min and max set in it
class SkimSlider : public IMouseInteractive {
public:
    SkimSlider(int _x, int _y, int _width, int _height, const char* _name, const char* _parent, ConVar* pVar);
    //~SkimSlider();

    void Render();
    void OnClick( const Drawing::CartPt& loc );
    void Drag( const Drawing::CartPt& loc );
    void MoveSliderTo(int _x);
    ConVar* m_pVar;
};

struct SkimEnum { const char* name; int set; };
class SkimEnumButton : public IMouseInteractive {
public:
    SkimEnumButton(int _x, int _y, int _width, int _height, const char* _name, const char* _parent, ConVar* pVar);

    void Render();
    void OnClick( const Drawing::CartPt& loc );
    void AddEnumeration( const char* set, int val );

    ConVar* m_pVar;
    std::vector<SkimEnum> m_vEnums;
    int selected;

    inline void GetBtnBounds(int& x0, int& y0, int& x1, int& y1, int& x2, int& x3)
    {
        x0 = 20; x1 = 30;
        y0 = 5; y1 = height-5;
        //y2 = 5; y3 = height-5;
        x2 = width - 30;
        x3 = width - 20;
    }
    int GetValueIndex(int val)
    {
        for (int i = 0; i < m_vEnums.size(); i++)
        {
            if (m_vEnums[i].set == val)
                return i;
        }
        return -1;
    }
};