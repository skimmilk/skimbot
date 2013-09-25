#pragma once
#include "Interfaces.h"
#include "TF2Player.h"

//stupid Microsoft stuff
#define CreateFont CreateFont
namespace Skim{
namespace Drawing{
    class CartPt
    {
    public:
        CartPt(int _x, int _y);
        CartPt(); 
        int x,y;

        inline CartPt CartPt::operator +(const CartPt& p) const {
            return CartPt(x + p.x, y + p.y);
        }
        inline CartPt CartPt::operator -(const CartPt& p) const {
            return CartPt(x - p.x, y - p.y);
        }
        inline CartPt CartPt::operator *(int a) const {
            return CartPt(x*a, y*a);
        }
        inline CartPt& CartPt::operator *=(int a) {
            x *= a;
            y *= a;
            return *this;
        }
        inline CartPt& CartPt::operator +=(const CartPt& p) {
            x += p.x;
            y += p.y;
            return *this;
        }
        inline CartPt& CartPt::operator -=(const CartPt& p) {
            x -= p.x;
            y -= p.y;
            return *this;
        }
    };

    void Init();
    
    void DrawString(int x, int y, const Color& color, const char* text);
    void DrawStringCenter(int x, int y, const Color& color, const char* text);
    bool WorldVectorToScreen(const Vector& worldpt, CartPt& screenpt);
    inline void DrawLine(int x1, int y1, int x2, int y2, const Color& color);
    void DrawLine(const CartPt& p1, const CartPt& p2, const Color& color);
    bool DrawLineFromWorld(const Vector&, const Vector&, const Color&);
    void DrawBox(int x1, int y1, int x2, int y2, const Color&);
    void DrawFilledBox(int x1, int y1, int x2, int y2, const Color&);
    bool PlayerToScreen(TF2Player* plyr, CartPt& head, CartPt& feet);

    //returns fill length, does not initialize color
    int HealthbarColorAndFill(int max, int current, int fullLen, Color& color);
    Color PlayerTeamColor(bool altFriendColor, bool showAimbotted, TF2Player* plyr);
};
};