// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textDX.h v1.0
// DirectX font

#ifndef _TEXTDX_H               // Prevent multiple definitions if this 
#define _TEXTDX_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "../Const.h"
#include "../CGraphics.h"

class CTextDX
{
private:
    CGraphics    *      m_pGraphics;
    COLOR_ARGB          m_color;          // font color (a,r,g,b)
    LP_DXFONT           m_dxFont;
    RECT                m_fontRect;       // text rectangle
    // matrix to rotate the text
    D3DXMATRIX          m_matrix;
    float               m_angle;          // rotation angle of text in radians

public:
    // Constructor (sprite text)
    CTextDX();

    // Destructor
    virtual ~CTextDX();

    // Initialize font
    // Pre: *g points to Graphics object
    //      height = height in pixels
    //      bold = true/false
    //      italic = true/false
    //      &fontName = name of font to use
    virtual bool Initialize(CGraphics *g, int height, bool bold, bool italic, const std::string &fontName);

    // Print at x,y. Call between spriteBegin()/spriteEnd()
    // Return 0 on fail, height of text on success
    // Pre: &str contains text to display
    //      x, y = screen location
    virtual int Print(const std::string &str, int x, int y);

    // Print inside rect using format. Call between spriteBegin()/spriteEnd()
    // Return 0 on fail, height of text on success
    // Pre: &str = text to display
    //      &rect = rectangular region
    //      format = format specifier
    virtual int Print(const std::string &str, RECT &rect, UINT format);

    // Return rotation angle in degrees.
    virtual float GetDegrees()      {return angle*(180.0f/(float)PI);}

    // Return rotation angle in radians.
    virtual float GetRadians()      {return angle;}

    // Returns font color
    virtual COLOR_ARGB GetFontColor() {return color;}

    // Set rotation angle in degrees.
    // 0 degrees is up. Angles progress clockwise.
    virtual void SetDegrees(float deg)  {angle = deg*((float)PI/180.0f);}

    // Set rotation angle in radians.
    // 0 radians is up. Angles progress clockwise.
    virtual void SetRadians(float rad)  {angle = rad;}

    // Set the font color. Use SETCOLOR_ARGB macro or colors from graphicsNS::
    virtual void SetFontColor(COLOR_ARGB c) {color = c;}

    // Release resources
    virtual void OnLostDevice();

    // Restore resources
    virtual void OnResetDevice();
};

#endif

