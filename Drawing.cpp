#include "Drawing.h"
#include "TF2Entity.h"
#include "Aimbot.h"
#include "SteamFriendTools.h"

//stupid Microsoft stuff
#define CreateFont CreateFont

namespace Skim{
namespace Drawing{
    using namespace ValveInterfaces;


    CartPt::CartPt(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    CartPt::CartPt(){ x = 0; y = 0; }

    vgui::HFont font;

    void Init()
    {
        font = pSurface->CreateFont();
        //HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0
        pSurface->SetFontGlyphSet( font, "Default", 12, 450, 0, 0, 0x200); //ISurface::FONTFLAG_OUTLINE );//| ISurface::FONTFLAG_ANTIALIAS );
    }
    
    void DrawString(int x, int y, const Color& color, const char* text)
    {
        wchar_t wtext[32];
        swprintf(wtext, 32, L"%hs", text);

        pSurface->DrawSetTextFont( font );
        pSurface->DrawSetTextColor( color );
        pSurface->DrawSetTextPos( x, y );
        pSurface->DrawPrintText( wtext, std::char_traits<wchar_t>::length(wtext) );
    }

    void DrawStringCenter(int x, int y, const Color& color, const char* text)
    {
        //int len = strlen(text);
        wchar_t wtext[32];
        swprintf(wtext, 32, L"%hs", text);

        int w, h;
        pSurface->GetTextSize(font, wtext, w, h);
        DrawString( x - (w / 2), y - ( h / 2 ) , color, text);
    }

    inline void DrawLine(int x1, int y1, int x2, int y2, const Color& color)
    {
        pSurface->DrawSetColor(color);
        pSurface->DrawLine(x1, y1, x2, y2);
    }
    void DrawLine(const CartPt& p1, const CartPt& p2, const Color& color)
    {
        DrawLine(p1.x, p1.y, p2.x, p2.y, color);
    }

    bool DrawLineFromWorld(const Vector& worldloc1, const Vector& worldloc2, const Color& color)
    {
        CartPt loc1, loc2;
        if(!WorldVectorToScreen(worldloc1, loc1) || !WorldVectorToScreen(worldloc2, loc2))
            return false;
        DrawLine(loc1.x, loc1.y, loc2.x, loc2.y, color);
        return true;
    }
    
    //----------------------------------------
    //-----------WorldVectorToScreen----------
    //----------------------------------------
    //    Notes:
    //        Returns where the point is on the
    //          screen
    //        Mostly copypaste from 
    //          view_scene.cpp
    //----------------------------------------
    bool WorldVectorToScreen(const Vector& worldpt, CartPt& screenpt)
    {
        //// Assume 90 degree fov
        //// Y - angle -45 == left, angle 0 == middle, angle 45 == right
        //// X - angle -45 * scrh / scrw == up etc.

        //Vector me = TF2Entity::Me()->vecEyeOrigin();
        //Vector vecdist = worldpt - me;

        //QAngle look;
        //ValveInterfaces::pEngine->GetViewAngles(look);
        //float yAng = (atan2(vecdist.y, vecdist.x) * 180.0f / M_PI - look.y) * -1;
        //float xAng = (atan2(vecdist.z, hypotf(vecdist.x, vecdist.y)) * 180.0f / M_PI + look.x) * -1;

        //int h = ValveInterfaces::pClient->GetScreenHeight(), w = ValveInterfaces::pClient->GetScreenWidth();

        //float scrRatio = ((float)h) / ((float)w);
        //if (yAng < -45.0f || yAng > 45.0f || xAng * scrRatio > 45.0f || xAng * scrRatio < -45.0f)
        //    return false;

        ////get angles to screen
        ////center them
        //screenpt.x = (float)w / 2.0f + (yAng * w / 90.0f);
        //screenpt.y = (float)h / 2.0f + (xAng * h * scrRatio / 90.0f);
        //return true;
        float z,x,y;

        const VMatrix &worldToScreen = ValveInterfaces::pEngine->WorldToScreenMatrix();
        x = worldToScreen[0][0] * worldpt[0] + worldToScreen[0][1] * worldpt[1] + worldToScreen[0][2] * worldpt[2] + worldToScreen[0][3];
        y = worldToScreen[1][0] * worldpt[0] + worldToScreen[1][1] * worldpt[1] + worldToScreen[1][2] * worldpt[2] + worldToScreen[1][3];
        z = worldToScreen[3][0] * worldpt[0] + worldToScreen[3][1] * worldpt[1] + worldToScreen[3][2] * worldpt[2] + worldToScreen[3][3];

        if( z < 0.001f )
            return false;

        x /= z;
        y /= z;

        int scrw, scrh;
        ValveInterfaces::pEngine->GetScreenSize( scrw, scrh );
        float cx = scrw / 2;
        float cy = scrh / 2;
        //Assuming the + 0.5f is for rounding
        cx += x * 0.5f * scrw + 0.5f;
        cy -= y * 0.5f * scrh + 0.5f;
        screenpt.x = cx;
        screenpt.y = cy;
        return true;
    }
    
    void DrawBox(int x1, int y1, int x2, int y2, const Color& color)
    {
        pSurface->DrawSetColor(color);
        pSurface->DrawOutlinedRect(x1, y1, x2, y2);
    }
    void DrawFilledBox(int x1, int y1, int x2, int y2, const Color& color)
    {
        pSurface->DrawSetColor(color);
        pSurface->DrawFilledRect(x1, y1, x2, y2);
    }
    bool PlayerToScreen(TF2Player* plyr, CartPt& head, CartPt& feet)
    {
        return (WorldVectorToScreen(plyr->vecOrigin(), feet) && WorldVectorToScreen(plyr->vecEyeOrigin(), head));
    };

    int HealthbarColorAndFill(int max, int current, int fullLen, Color& color)
    {
        //low health = (255,0,0); mid = (127,0,127); max = (0,0,255)
        //y = x - 129; current == 0 -> x == 0; current == max -> x == 255
        int normal = ((float)current * 256.0f / (float)max);
        //r == 0, g == 1
        int r = normal * -1 + 256;
        int g = normal;
        if (r > 255) r = 255;
        if( g > 255) g = 255;
        color[0] = r;
        color[1] = g;
        return fullLen * current / max;
    }
    Color PlayerTeamColor(bool altFriendColor, bool showAimbotted, TF2Player* plyr)
    {
        Color plyrTeamClr = Color(0,0,0,255);
        if ( altFriendColor && SteamFriendTools::isFriend( plyr ) )
            plyrTeamClr = Color(0,255,0,255);
        else if (plyr->iTeamNum() == 3)
            plyrTeamClr = Color(0,0,255,255);
        else plyrTeamClr = Color(255,0,0,255);
        if (showAimbotted && plyr->index() == CAimbot::m_iIndexAimed ) {
            //plrTeamClr[0] = 255;
            plyrTeamClr[1] = 255;
            //plrTeamClr[2] = 0;
        }
        return plyrTeamClr;
    }
};
};