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
    COLOR_ARGB          m_color;               // ��Ʈ�÷�(ARGB)
    COLOR_ARGB          m_backColor;           // ����÷�(ARGB)
    UINT                m_fontHeight;               // �ȼ� �󿡼� height
    UINT                m_tabSize;                  // �� ������
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

    // ������
    virtual bool Initialize(CGraphics *g, const char *file);
    // Update�� �������� �ʴ´�
    virtual void Update(float frameTime) {};
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    //=============================================================
    // new member functions
    //=============================================================

    // XY����
    virtual void SetXY(int x, int y);

    // spriteBegin()/spriteEnd() ���̿��� ȣ���ϸ�, X, Y�࿡ ���缭 ���
    virtual void Print(const std::string &str);

    // spriteBegin()/spriteEnd() ���̿��� ȣ���ϸ�, �Էµ� X, Y�࿡ ���缭 ���
    virtual void Print(const std::string &str, int x, int y);

    // spriteBegin()/spriteEnd() ���̿��� ȣ���ϸ�, X, Y�� �׸��� Alignment�� ���缭 ���
    virtual void Print(const std::string &str, int x, int y, NSText::Alignment align);

    // WIDTH�� HEIGHT�� ��ȯ
    virtual void GetWidthHeight(const std::string &str, UINT &width, UINT &height);

    // ��Ʈ �÷� ����
    virtual void SetFontColor(COLOR_ARGB c) {m_color = c;}

    // ��Ʈ �÷� ��ȯ
    virtual COLOR_ARGB GetFontColor() {return m_color;}

    // ��� �÷� ����
    virtual void SetBackColor(COLOR_ARGB bc) {m_backColor = bc;}

    // ��� �÷� ��ȯ
    virtual COLOR_ARGB GetBackColor() {return m_backColor;}

    // �ȼ� �󿡼� ��Ʈ Height ����
    virtual void SetFontHeight(UINT height)
    {
        if(height == 0 || height > NSText::MAX_FONT_HEIGHT)  // if invalid size
            return;
        m_fontHeight = height;
        m_spriteData.scale = (float)height/(float)NSText::FONT_HEIGHT;
    }

    // Height ��ȯ
    virtual UINT GetFontHeight() {return m_fontHeight;}

    // ��ʰ� ����
    virtual void SetProportional(bool p) {m_proportional = p;}

    // ��ʰ� ��ȯ ����
    virtual bool GetProportional() {return m_proportional;}

    // ��� ���� ����
    virtual void SetProportionalSpacing(UINT s) {m_proportionalSpacing = s;}

    // ��� ���� ��ȯ
    virtual UINT GetProportionalSpacing() {return m_proportionalSpacing;}

    // ���� ����
    virtual void SetUnderline(bool u) {m_underline = u;}

    // ���� ������ ��ȯ
    virtual bool GetUnderline() {return m_underline;}

    // ���� ����
    virtual void SetBold(bool b) {m_bold = b;}

    // ���� ������ ��ȯ
    virtual bool GetBold() {return m_bold;}

    // �� ������ ����
    virtual void SetTabSize(UINT size)  
    {
        if(size == 0)
            return;
        m_tabSize = size;
    }

    // �� ������ ��ȯ
    virtual UINT GetTabSize() {return m_tabSize;}

    // ���� string�� ���İ��� spriteData�� x,y���� �����Ѵ�.
    // �⺻ ���� LEFT�̴�.
    virtual void DoAlign(const std::string &str);

    // Display character sprite described by spriteData using color and fill
    // Does underline and bold
    virtual void DrawChar(UCHAR ch);
};


