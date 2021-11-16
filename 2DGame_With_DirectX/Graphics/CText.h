// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// text.h v1.0
// Sprite based font

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "CImage.h"
#include "../Const.h"

namespace NSText {
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
    COLOR_ARGB          m_color;               // 폰트컬러(ARGB)
    COLOR_ARGB          m_backColor;           // 배경컬러(ARGB)
    UINT                m_fontHeight;               // 픽셀 상에서 height
    UINT                m_tabSize;                  // 탭 사이즈
    UINT                m_proportionalSpacing;      
    bool                m_proportional;              
    bool                m_underline;
    bool                m_bold;
    NSText::Alignment   m_align;        // how is text aligned (center, left, etc)

public:
    CText();
    virtual ~CText();

    //=============================================================
    // inherited member functions
    //=============================================================

    // 생성자
    virtual bool Initialize(CGraphics *g, const char *file);
    // Update를 진행하지 않는다
    virtual void Update(float frameTime) {};
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    //=============================================================
    // new member functions
    //=============================================================

    // XY설정
    virtual void SetXY(int x, int y);

    // spriteBegin()/spriteEnd() 사이에서 호출하며, X, Y축에 맞춰서 출력
    virtual void Print(const std::string &str);

    // spriteBegin()/spriteEnd() 사이에서 호출하며, 입력된 X, Y축에 맞춰서 출력
    virtual void Print(const std::string &str, int x, int y);

    // spriteBegin()/spriteEnd() 사이에서 호출하며, X, Y축 그리고 Alignment에 맞춰서 출력
    virtual void Print(const std::string &str, int x, int y, NSText::Alignment align);

    // WIDTH와 HEIGHT를 반환
    virtual void GetWidthHeight(const std::string &str, UINT &width, UINT &height);

    // 폰트 컬러 설정
    virtual void SetFontColor(COLOR_ARGB c) {m_color = c;}

    // 폰트 컬러 반환
    virtual COLOR_ARGB GetFontColor() {return m_color;}

    // 배경 컬러 설정
    virtual void SetBackColor(COLOR_ARGB bc) {m_backColor = bc;}

    // 배경 컬러 반환
    virtual COLOR_ARGB GetBackColor() {return m_backColor;}

    // 픽셀 상에서 폰트 Height 설정
    virtual void SetFontHeight(UINT height)
    {
        if(height == 0 || height > NSText::MAX_FONT_HEIGHT)  // if invalid size
            return;
        m_fontHeight = height;
        m_spriteData.scale = (float)height/(float)NSText::FONT_HEIGHT;
    }

    // Height 반환
    virtual UINT GetFontHeight() {return m_fontHeight;}

    // 비례값 설정
    virtual void SetProportional(bool p) {m_proportional = p;}

    // 비례값 반환 설정
    virtual bool GetProportional() {return m_proportional;}

    // 비례 간격 설정
    virtual void SetProportionalSpacing(UINT s) {m_proportionalSpacing = s;}

    // 비례 간격 반환
    virtual UINT GetProportionalSpacing() {return m_proportionalSpacing;}

    // 밑줄 설정
    virtual void SetUnderline(bool u) {m_underline = u;}

    // 밑줄 설정값 반환
    virtual bool GetUnderline() {return m_underline;}

    // 볼드 설정
    virtual void SetBold(bool b) {m_bold = b;}

    // 볼드 설정값 반환
    virtual bool GetBold() {return m_bold;}

    // 탭 사이즈 설정
    virtual void SetTabSize(UINT size)  
    {
        if(size == 0)
            return;
        m_tabSize = size;
    }

    // 탭 사이즈 반환
    virtual UINT GetTabSize() {return m_tabSize;}

    // 현재 string과 정렬값에 spriteData의 x,y값을 설정한다.
    // 기본 값은 LEFT이다.
    virtual void DoAlign(const std::string &str);

    // Display character sprite described by spriteData using color and fill
    // Does underline and bold
    virtual void DrawChar(UCHAR ch);
};


