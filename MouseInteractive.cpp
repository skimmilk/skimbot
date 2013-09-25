#include "MouseInteractive.h"

ConVar* IMouseInteractive::r, *IMouseInteractive::g, *IMouseInteractive::b, *IMouseInteractive::a;
ConVar* IMouseInteractive::hr, *IMouseInteractive::hg, *IMouseInteractive::hb, *IMouseInteractive::ha;
//----------------------------------------
//----Render functions for all classes----
//----------------------------------------
void SkimFilter::Render(){
    DrawBG();
    Drawing::DrawStringCenter(x+(width/2), y+(height/2), Color(255,255,255,255), name);
}
void SkimButton::Render(){
    DrawBG();
    Drawing::DrawStringCenter(x + 8, y + (height/2), Color(255,255,255,255), name);
}
void SkimSlider::Render(){
    DrawBG();
    static const Color white(255,255,255,255);
    Drawing::DrawStringCenter(x + (width/2), y + (height/2) - 6, white, name);

    int startx = x + 10, starty = y + (height/2) - 2, endx = x + width - 10;
    Drawing::DrawFilledBox(startx, starty, endx, starty + 4, white);

    float xOffset = ((float)width-10) / (m_pVar->m_fMaxVal - m_pVar->m_fMinVal) * m_pVar->GetFloat() - m_pVar->m_fMinVal;
    Drawing::DrawFilledBox( startx + xOffset - 8, starty - 4, startx + xOffset + 8, starty + 4, Color(77,109,243,255));

    char str[5];
    itoa(m_pVar->GetInt(),str,10);
    Drawing::DrawStringCenter( startx + xOffset, starty, white, str );
}
void SkimEnumButton::Render(){
    DrawBG();
    int x0=0,y0=0,x1=0,y1=0,x2=0,x3=0;
    x0+=x;x1+=x;x2+=x;x3+=x;
    y0+=y;y1+=y;
    GetBtnBounds(x0,y0,x1,y1,x2,x3);

    Drawing::DrawBox(x0,y0,x1,y1,Color(0,0,0,255));
    Drawing::DrawFilledBox(x0,y0,x1,y1,Color(127,127,127,255));

    Drawing::DrawBox(x2,y0,x3,y1,Color(0,0,0,255));
    Drawing::DrawFilledBox(x2,y0,x3,y1,Color(127,127,127,255));

    Drawing::DrawString(x+(width/2),y+(height/2),Color(255,255,255,255),m_vEnums[selected].name);
    Drawing::DrawString((x0+x1)/2,y+(height/2),Color(255,255,255,255),"<<");
    Drawing::DrawString((x2+x3)/2,y+(height/2),Color(255,255,255,255),">>");
}

//----------------------------------------
//-----OnClick functions for classes------
//----------------------------------------
void SkimButton::OnClick(const Drawing::CartPt& loc){
    //pEngine->ClientCmd_Unrestricted(m_pFunc->GetName());
    m_fnFunc();
}
void SkimSlider::OnClick(const Drawing::CartPt& loc){
    MakePointInside(loc);
    MoveSliderTo(loc.x);
}
void SkimEnumButton::OnClick( const Drawing::CartPt& loc) {
    // see if it is in button- or button+
    int x0=0,y0=0,x1=0,y1=0,x2=0,x3=0;
    GetBtnBounds(x0,y0,x1,y1,x2,x3);

    int val = 0;
    if (loc.y > y0 && loc.y < y1)
    {
        if (loc.x > x0 && loc.x < x1)
        {
            val = (m_pVar->GetInt() - 1) % m_vEnums.size();
            m_pVar->SetValue(val);
            selected = GetValueIndex(val);
        }
        if (loc.x > x2 && loc.x < x3)
        {
            val = (m_pVar->GetInt() + 1) % m_vEnums.size();
            m_pVar->SetValue(val);
            selected = GetValueIndex(val);
        }
    }
}

// Drag function
void SkimSlider::Drag(const Drawing::CartPt& loc){
    MakePointInside(loc);
    MoveSliderTo(loc.x);
}
void SkimSlider::MoveSliderTo(int _x) {
    //val = (fmaxval - fminval)*(x/xmaxval) + fminval
    if (_x >= 10 && _x < width - 10){
        float fm = m_pVar->m_fMaxVal - m_pVar->m_fMinVal;
        float fx = ((float)(_x - 10)) / ((float)(width - 10));
        float fb = m_pVar->m_fMinVal;
        m_pVar->SetValue( (int)(fm * fx + fb) );// cast to int?
    }
}

//----------------------------------------
//--------Constructor & Destructor--------
//----------------------------------------

IMouseInteractive::IMouseInteractive(int _x, int _y, int _width, int _height, const char* _name, const char* _parent)
{
    x = _x;
    y = _y;
    width = _width;
    height = _height;
    name = _name;
    parent = _parent;
    m_iParent = 0;
    m_bHighlighted = false;
    m_bShown = false;
}
SkimButton::SkimButton(int _x, int _y, int _width, int _height, const char* _name, const char* _parent, ConCommand* pFunc)
    : IMouseInteractive(_x, _y, _width, _height, _name, _parent) {
    m_fnFunc = pFunc->m_fnCommandCallbackV1;
}
SkimFilter::SkimFilter(int _x, int _y, int _width, int _height, const char* _name, const char* _parent)
    : IMouseInteractive(_x, _y, _width, _height, _name, _parent){
}
SkimSlider::SkimSlider(int _x, int _y, int _width, int _height, const char* _name, const char* _parent, ConVar* pVar)
    : IMouseInteractive(_x, _y, _width, _height, _name, _parent){
    m_pVar = pVar;
}
SkimEnumButton::SkimEnumButton(int _x, int _y, int _width, int _height, const char* _name, const char* _parent, ConVar* pVar)
    : IMouseInteractive(_x, _y, _width, _height, _name, _parent){
        m_pVar = pVar;
}
void SkimEnumButton::AddEnumeration(const char* set, int val)
{
    SkimEnum st;
    st.name = set;
    st.set = val;
    m_vEnums.push_back(st);
}