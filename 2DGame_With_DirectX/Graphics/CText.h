// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// text.h v1.0
// Sprite based font

#ifndef _text_h                 // Prevent multiple definitions if this 
#define _text_h                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "CImage.h"
#include "../Const.h"


namespace NSText
{
    enum Alignment{LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM};
    struct FontData
    {
        UINT left;
        UINT right;
    };
    // Actual font width is 48, font height is 62
    const int FONT_BORDER = 3;      // 1 pixel wide transparent border + visible cell border on right and bottom
    const int FONT_WIDTH  = 48;
    const int FONT_HEIGHT = 62;     // font is 62 pixels high
    const int GRID_WIDTH  = FONT_WIDTH + FONT_BORDER;    
    const int GRID_HEIGHT = FONT_HEIGHT + FONT_BORDER;
    const int COLUMNS = 16;         // number of columns in font image
    const int ROWS = 14;            // number of rows in font image
    const int FRAMES = 1;           // how many frames of animation (1 = not animated)
    const double ANIM_DELAY = 0.0;  // delay between animation frames
    const int MAX_FONT_HEIGHT = 1000;
    const int MIN_CHAR = 0x0020;    // minimum character code
    const int MAX_CHAR = 0x00FF;    // maximum character code
    const int PROPORTIONAL_SPACING = 5; // pixels between 1:1 scaled characters
    const int TAB_SIZE = 8;
    const char UNDERLINE = '_';
    const char SOLID = 0x7F;        // solid block used for fill character ASCII $7F
    const int BOLD_SIZE = 4;        // pixel shift for bold display
}


class CText : public CImage
{
private:
    CTextureManager     m_fontTexture;     
    LP_TEXTURE          m_textureData;     
    char    *           m_file;            
    CGraphics *         m_pGraphics;       
    UINT                m_width, m_height; 
    NSText::FontData    m_fontData[NSText::ROWS][NSText::COLUMNS]; 
    COLOR_ARGB          m_color;               // font color (a,r,g,b)
    COLOR_ARGB          m_backColor;           // background color (a,r,g,b)
    UINT                m_fontHeight;               // font height in pixels
    UINT                m_tabSize;                  // character spacing for tab
    UINT                m_proportionalSpacing;      // spacing in pixels between proportional characters
    bool                m_proportional;              // true for proportional spacing
    bool                m_underline;
    bool                m_bold;
    NSText::Alignment   m_align;        // how is text aligned (center, left, etc)

public:
    // default constructor (sprite text)
    CText();

    // destructor
    virtual ~CText();

    //=============================================================
    // inherited member functions
    //=============================================================

    // Initialize font using file texture image.
    virtual bool Initialize(CGraphics *g, const char *file);
    // disable inherited update()
    virtual void Update(float frameTime) {};
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    //=============================================================
    // new member functions
    //=============================================================

    // Position display point at x,y
    virtual void SetXY(int x, int y);

    // Print at current x,y. Call between spriteBegin()/spriteEnd()
    virtual void Print(const std::string &str);

    // Print at x,y. Call between spriteBegin()/spriteEnd()
    virtual void Print(const std::string &str, int x, int y);

    // Print at x,y using align. Call between spriteBegin()/spriteEnd()
    virtual void Print(const std::string &str, int x, int y, NSText::Alignment align);

    // Determines width and height of string in pixels for current point size.
    virtual void GetWidthHeight(const std::string &str, UINT &width, UINT &height);

    // Set the font color. Use SETCOLOR_ARGB macro or colors from graphicsNS::
    virtual void SetFontColor(COLOR_ARGB c) {m_color = c;}

    // Returns font color
    virtual COLOR_ARGB GetFontColor() {return m_color;}

    // Set background color
    virtual void SetBackColor(COLOR_ARGB bc) {m_backColor = bc;}

    // Returns background color
    virtual COLOR_ARGB GetBackColor() {return m_backColor;}

    // Set height of font in pixels
    virtual void SetFontHeight(UINT height)
    {
        if(height == 0 || height > NSText::MAX_FONT_HEIGHT)  // if invalid size
            return;
        m_fontHeight = height;
        m_spriteData.scale = (float)height/(float)NSText::FONT_HEIGHT;
    }

    // Returns height of font
    virtual UINT GetFontHeight() {return m_fontHeight;}

    // Set proportional spacing true/false
    virtual void SetProportional(bool p) {m_proportional = p;}

    // Returns proportional spacing setting
    virtual bool GetProportional() {return m_proportional;}

    // Set the spacing used for proportional spacing
    virtual void SetProportionalSpacing(UINT s) {m_proportionalSpacing = s;}

    // Return proportional spacing size
    virtual UINT GetProportionalSpacing() {return m_proportionalSpacing;}

    // Set underline true/false
    virtual void SetUnderline(bool u) {m_underline = u;}

    // Returns underline setting
    virtual bool GetUnderline() {return m_underline;}

    // Set bold true/false
    virtual void SetBold(bool b) {m_bold = b;}

    // Returns bold setting
    virtual bool GetBold() {return m_bold;}

    // Set tab size
    virtual void SetTabSize(UINT size)  
    {
        if(size == 0)
            return;
        m_tabSize = size;
    }

    // Return tab size
    virtual UINT GetTabSize() {return m_tabSize;}

    // Set spriteData.x,spriteData.y for current string and alignment.
    // The default alignment is LEFT.
    virtual void DoAlign(const std::string &str);

    // Display character sprite described by spriteData using color and fill
    // Does underline and bold
    virtual void DrawChar(UCHAR ch);
};

#endif

