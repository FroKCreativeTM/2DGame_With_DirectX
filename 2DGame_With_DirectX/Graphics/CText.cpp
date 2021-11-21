// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// text.cpp v1.0
// Sprite based font.

#include "CText.h"

//=============================================================================
// default constructor
//=============================================================================
CText::CText() : CImage()
{
    m_file = NULL;                        // font texture
    m_graphics = NULL;                    // pointer to graphics
    m_color = NSGraphics::WHITE;          // default to white font
    m_backColor = NSGraphics::TRANSCOLOR;             // default to transparent (no) fill
    m_align = NSText::LEFT;
    m_width = NSText::GRID_WIDTH - 3;     // -2 for transparent border and -1 for divider line
    m_height = NSText::GRID_HEIGHT - 3;
    m_fontHeight = NSText::FONT_HEIGHT;
    m_spriteData.width = 0;
    m_spriteData.height = 0;
    m_spriteData.rect.bottom = 1;         // rectangle to select parts of an image
    m_spriteData.rect.right = 1;

    for(int row=0; row< NSText::ROWS; row++)         // for each row of characters in font
    {
        for(int col=0; col < NSText::COLUMNS; col++)  // for each col of characters in font
        {
            m_fontData[row][col].left = MAXINT;
            m_fontData[row][col].right = 0;
        }
    }
    m_proportional = false;
    m_proportionalSpacing = NSText::PROPORTIONAL_SPACING;
    m_underline = false;
    m_bold = false;
    m_tabSize = NSText::TAB_SIZE;
}

//=============================================================================
// destructor
//=============================================================================
CText::~CText()
{
    CImage::~CImage();
}

//=============================================================================
// Initialize the Text.
// Find the left and right edge of each character in the font image 
// Post: returns true if successful, false if failed
//       fontData array contains left and right edge of each character
//=============================================================================
bool CText::Initialize(CGraphics *g, const char *file)
{
    try {
        m_pGraphics = g;           // pointer to graphics object

        //-------------------------------------------------------------
        // Load the font texture and examine it pixel by pixel to find
        // the exact position of each character.
        //-------------------------------------------------------------

        // Load font texture into system memory so it may be locked
        UINT w,h;
        HRESULT result = m_pGraphics->LoadTextureSystemMem(file, NSGraphics::TRANSCOLOR, w, h, m_textureData);
        if (FAILED(result))
        {
            SAFE_RELEASE(m_textureData);
            return false;
        }

        // textureData.width & textureData.height contain size of entire font texture
        // Each character has a 1 pixel wide border
        // There are ROWS * COLS characters

        // Lock the font texture, required to access the pixel data
        D3DLOCKED_RECT rect;
        result = m_textureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
        if(FAILED(result))                          // if lock failed
        {
            SAFE_RELEASE(m_textureData);
            return false;
        }

        for(DWORD row = 0; row < NSText::ROWS; row++)   // for each row of characters in font
        {
            for(DWORD col = 0; col < NSText::COLUMNS; col++)    // for each col of characters in font
            {
                m_fontData[row][col].left = MAXINT;    // initialize fontData
                m_fontData[row][col].right = 0;

                // Process each character pixel by pixel    
                // for y = top pixel; y <= bottom pixel; y++
                for(DWORD y = row * NSText::GRID_HEIGHT + 1; y < (row + 1) * NSText::GRID_HEIGHT - 1; y++)
                {
                    // Get a pointer to the start of this scanline in the texture
                    DWORD* pBits = (DWORD*)((BYTE*)rect.pBits + y*rect.Pitch);
                    // Process this line of pixels
                    for(DWORD x = col * NSText::GRID_WIDTH + 1; x < (col + 1)* NSText::GRID_WIDTH - 1; x++)
                    {
                        // Get this pixel
                        DWORD dwPixel = pBits[x];

                        // If the alpha is not transparent
                        if((dwPixel & 0xff000000) != 0x00)
                        {
                            if(x < m_fontData[row][col].left)     // if this pixel is more left
                                m_fontData[row][col].left = x;    // save as left edge of character
                            if(x > m_fontData[row][col].right)    // if this pixel is more right
                                m_fontData[row][col].right = x;   // save right edge of character
                        }
                    }
                }
            }
        }

        // Done with the texture, so unlock it
        m_textureData->UnlockRect(0);
    
        // release this font texture, we just needed it to get font spacing
        SAFE_RELEASE(m_textureData);

        //-------------------------------------------------------------
        // load the font image into a texture manager for use
        //-------------------------------------------------------------
        if (!m_fontTexture.Initialize(m_pGraphics, file)) {
            return false;                   // if error loading font texture
        }

        // prepare the font image
        if (!CImage::Initialize(m_pGraphics, NSText::FONT_WIDTH, NSText::FONT_HEIGHT, 0, &m_fontTexture)) {
            return false;                   // if failed
        }
    }
    catch(...)
    {
        return false;
    }
    return true;                    // successful
}

//=============================================================================
// setXY
// set x,y for text output
//=============================================================================
void CText::SetXY(int x2, int y2)
{
    if (x2 < 0 || y2 < 0)
        return;
    m_spriteData.x = (float)x2;
    m_spriteData.y = (float)y2;
}

//=============================================================================
// print string at current x,y
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void CText::Print(const std::string &str)
{
    Print(str, (int)m_spriteData.x, (int)m_spriteData.y);
}

//=============================================================================
// print with align at x,y
// text is aligned as specified from:
// LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void CText::Print(const std::string &str, int x, int y, NSText::Alignment al)
{
    m_align = al;             // save alignment
    Print(str, x, y);
    m_align = NSText::LEFT;
}

//=============================================================================
// Print string at x,y
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void CText::Print(const std::string &str, int x, int y)
{
    UCHAR ch = 0, chN = 0;
    std::string str2;
    m_width = NSText::FONT_WIDTH;
    int scaledWidth = static_cast<int>(NSText::FONT_WIDTH * m_spriteData.scale);
    float saveY=0;
    int tabX=0, tabW=0;
    
    m_spriteData.x = (float)x;
    m_spriteData.y = (float)y;
    DoAlign(str);

    for(UINT i=0; i<str.length(); i++)
    {
        ch = str.at(i);
        if (ch > NSText::MIN_CHAR && ch <= NSText::MAX_CHAR)    // if displayable character
        {
            chN = ch - NSText::MIN_CHAR;                // make min_char index 0
            m_spriteData.rect.top = chN / NSText::COLUMNS * NSText::GRID_HEIGHT + 1;
            m_spriteData.rect.bottom = m_spriteData.rect.top + NSText::FONT_HEIGHT;
            if(m_proportional)
            {
                m_spriteData.rect.left = m_fontData[chN / NSText::COLUMNS][chN % NSText::COLUMNS].left;
                // DirectX wants right + 1
                m_spriteData.rect.right = m_fontData[chN / NSText::COLUMNS][chN % NSText::COLUMNS].right + 1;
                m_width = m_spriteData.rect.right - m_spriteData.rect.left;
                if(m_width >= NSText::FONT_WIDTH)         // if full width character do not add spacing
                {
                    m_width = NSText::FONT_WIDTH;         // limit width
                    m_spriteData.rect.left = chN % NSText::COLUMNS * NSText::GRID_WIDTH + 1;
                    m_spriteData.rect.right = m_spriteData.rect.left + NSText::FONT_WIDTH;
                }
                else {// not full width so add spacing between characters 
                    m_width += m_proportionalSpacing;
                }
                scaledWidth = static_cast<int>(m_width * m_spriteData.scale);
                DrawChar(ch);
            }
            else    // fixed pitch
            {
                m_width = NSText::FONT_WIDTH;
                m_spriteData.rect.left = chN % NSText::COLUMNS * NSText::GRID_WIDTH + 1;
                m_spriteData.rect.right = m_spriteData.rect.left + NSText::FONT_WIDTH;
                DrawChar(ch);
            }
            m_spriteData.x += scaledWidth;
        } 
        else    // else, non displayable character
        {
            switch(ch)
            {
            case ' ':                            // space
                if(m_proportional)
                {
                    m_width = NSText::FONT_WIDTH/2;
                    scaledWidth = static_cast<int>(m_width * m_spriteData.scale);
                }
                DrawChar(' ');
                m_spriteData.x += scaledWidth;
                break;
            // newline advances 1 line down and sets left edge to starting x screen position,
            // not left edge of screen
            case '\n':                            // newline
                m_spriteData.x = (float)x;
                m_spriteData.y += static_cast<int>(m_height * m_spriteData.scale);
                saveY = m_spriteData.y;
                str2 = str.substr(i,str.length());
                DoAlign(str2);
                m_spriteData.y = saveY;
                break;
            case '\r':                            // return to starting x position
                m_spriteData.x = (float)x;
                str2 = str.substr(i,str.length());
                DoAlign(str2);
                break;
            case '\t':                            // tab
                m_width = NSText::FONT_WIDTH;
                scaledWidth = static_cast<int>(m_width * m_spriteData.scale);
                tabX = static_cast<int>(m_spriteData.x) / (scaledWidth * m_tabSize);
                tabX = (tabX+1) * scaledWidth * m_tabSize;
                tabW = tabX - static_cast<int>(m_spriteData.x);
                while(tabW > 0)
                {
                    if(tabW >= scaledWidth)
                    {
                        DrawChar(' ');
                        m_spriteData.x += scaledWidth;
                    }
                    else
                    {
                        m_width = tabW;        // fractional part of character to align with tab stop
                        DrawChar(' ');
                        m_spriteData.x += tabW;
                    }
                    tabW -= scaledWidth;
                }
                break;
            case '\b':                            // backspace
                m_spriteData.x -= scaledWidth;
                if(m_spriteData.x < 0)
                    m_spriteData.x = 0;
                break;
            case '\v':                            // vertical tab
                m_spriteData.y += static_cast<int>(m_height * m_spriteData.scale);
                break;
            case 0x01:                            // font signature character
                m_spriteData.rect.top = 1;
                m_spriteData.rect.bottom = 1 + NSText::FONT_HEIGHT;
                m_spriteData.rect.left = 1;
                m_spriteData.rect.right = 1 + NSText::FONT_WIDTH;
                Draw(m_spriteData);
                m_spriteData.x += scaledWidth;
                break;
            }
        }
    }
    return;
}

//=============================================================================
// Set m_spriteData.x,m_spriteData.y for current string and alignment.
// The default alignment is LEFT.
//=============================================================================
void CText::DoAlign(const std::string &str)
{
    if(m_spriteData.texture == NULL)  // if no texture
        return;

    UINT w, h;
    switch (m_align) {
    case NSText::CENTER:            // center at x and align top to y
        GetWidthHeight(str, w, h);
        m_spriteData.x -= w / 2;
        break;
    case NSText::RIGHT:             // right justify at x,y
        GetWidthHeight(str, w, h);
        m_spriteData.x -= w;
        break;
    case NSText::CENTER_MIDDLE:     // center at x and vertical middle to y
        GetWidthHeight(str, w, h);
        m_spriteData.x -= w / 2;
        m_spriteData.y -= h / 2;
        break;
    case NSText::CENTER_BOTTOM:     // center at x and align bottom to y
        GetWidthHeight(str, w, h);
        m_spriteData.x -= w / 2;
        m_spriteData.y -= h;
        break;
    case NSText::LEFT_BOTTOM:       // left justify at x and align bottom to y
        GetWidthHeight(str, w, h);
        m_spriteData.y -= h;
        break;
    case NSText::RIGHT_BOTTOM:      // right justify at x and align bottom to y
        GetWidthHeight(str, w, h);
        m_spriteData.x -= w;
        m_spriteData.y -= h;
        break;
    }
}

//=============================================================================
// getWidthHeight
// Determines width and height of string in pixels for current font size.
// Does not display the string
//=============================================================================
void CText::GetWidthHeight(const std::string& str, UINT& w, UINT& h)
{
    if (m_spriteData.texture == NULL)         // if no texture
        return;

    UCHAR ch = 0, chN = 0;
    m_width = NSText::FONT_WIDTH;
    int scaledWidth = static_cast<int>(m_width * m_spriteData.scale);
    int strW = 0;
    h = 0;
    int stringWidth = 0;

    for (UINT i = 0; i < str.length(); i++)
    {
        ch = str.at(i);
        // if displayable character
        if (ch > NSText::MIN_CHAR && ch <= NSText::MAX_CHAR)
        {
            chN = ch - NSText::MIN_CHAR;    // make min_char index 0
            if (m_proportional)
            {
                m_spriteData.rect.left = m_fontData[chN / NSText::COLUMNS][chN % NSText::COLUMNS].left;
                // +1 for DirectX sprite width
                m_spriteData.rect.right = m_fontData[chN / NSText::COLUMNS][chN % NSText::COLUMNS].right + 1;
                m_width = m_spriteData.rect.right - m_spriteData.rect.left + m_proportionalSpacing;
                scaledWidth = static_cast<int>(m_width * m_spriteData.scale);
            }
            else    // fixed pitch
            {
                m_width = NSText::FONT_WIDTH;
                m_spriteData.rect.left = chN % NSText::COLUMNS * NSText::GRID_WIDTH + 1;
                m_spriteData.rect.right = m_spriteData.rect.left + NSText::FONT_WIDTH;
            }
            stringWidth += scaledWidth;
        }
        else    // else, non displayable character
        {
            switch (ch)
            {
            case ' ':   // space
                if (m_proportional)
                {
                    m_width = (NSText::FONT_WIDTH) / 2;
                    scaledWidth = static_cast<int>(m_width * m_spriteData.scale);
                }
                stringWidth += scaledWidth;
                break;
            case '\n':  // newline
                if (strW == 0)
                    strW = stringWidth;
                stringWidth = 0;
                h += static_cast<int>(m_height * m_spriteData.scale);
                break;
            case '\r':  // return
                if (strW == 0)
                    strW = stringWidth;
                stringWidth = 0;
                break;
            case '\t':  // tab
            {
                m_width = NSText::FONT_WIDTH;
                scaledWidth = static_cast<int>(m_width * m_spriteData.scale);
                int tabX = static_cast<int>(m_spriteData.x) / (scaledWidth * m_tabSize);
                tabX = (tabX + 1) * scaledWidth * m_tabSize;
                int tabW = tabX - static_cast<int>(m_spriteData.x);
                while (tabW > 0)
                {
                    if (tabW >= scaledWidth)
                        stringWidth += scaledWidth;
                    else
                    {
                        // fractional part of character to align with tab stop
                        m_tabSize = tabW;
                        stringWidth += tabW;
                    }
                    tabW -= scaledWidth;
                }
            }
            break;
            case '\b':      // backspace
                stringWidth -= scaledWidth;
                if (stringWidth < 0)
                    stringWidth = 0;
                break;
            case 0x01:      // special
                stringWidth += scaledWidth;
                break;
            }
        }
    }
    if (strW == 0)
        strW = stringWidth;
    w = strW;
    return;
}

//=============================================================================
// drawChar
// Display character sprite described by m_spriteData using color and fill
// Does underline and bold
//=============================================================================
void CText::DrawChar(UCHAR ch)
{
    SPRITE_DATA sd2 = m_spriteData;    // copy sprite data

    // display backColor color
    if(m_backColor != NSGraphics::TRANSCOLOR)     // if backColor is not transparent
    {
        m_spriteData.rect.top = (NSText::SOLID-NSText::MIN_CHAR) / NSText::COLUMNS * NSText::GRID_HEIGHT + 1;
        m_spriteData.rect.bottom = m_spriteData.rect.top + NSText::GRID_HEIGHT - 2;
        m_spriteData.rect.left = (NSText::SOLID-NSText::MIN_CHAR) % NSText::COLUMNS * NSText::GRID_WIDTH + 1;
        m_spriteData.rect.right = m_spriteData.rect.left + m_width;
        Draw(m_tabSize);        // draw backColor
        m_spriteData.rect = sd2.rect;     // restore character rect
    }

    // display underline
    if(m_underline)
    {
        m_spriteData.rect.top = (NSText::UNDERLINE-NSText::MIN_CHAR) / NSText::COLUMNS * NSText::GRID_HEIGHT + 1;
        m_spriteData.rect.bottom = m_spriteData.rect.top + NSText::GRID_HEIGHT - 2;
        m_spriteData.rect.left = (NSText::UNDERLINE-NSText::MIN_CHAR) % NSText::COLUMNS * NSText::GRID_WIDTH + 1;
        m_spriteData.rect.right = m_spriteData.rect.left + m_width;
        Draw(m_color);
        m_spriteData.rect = sd2.rect;     // restore character rect
    }

    // display character
    if(ch > NSText::MIN_CHAR && ch <= NSText::MAX_CHAR) // if displayable character
    {
        Draw(m_spriteData, m_color);
        if (m_bold)   // bold is done by displaying the character twice with offset x
        {
            m_spriteData.x += NSText::BOLD_SIZE*m_spriteData.scale; 
            Draw(m_spriteData, m_color);
            m_spriteData.x = sd2.x;
        }
    }
}


//=============================================================================
// called when graphics device is lost
//=============================================================================
void CText::OnLostDevice()
{
    try{
        if (!m_initialized)
            return;
        m_fontTexture.OnLostDevice();
    } catch(...){}
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void CText::OnResetDevice()
{
    try{
        if (!m_initialized)
            return;
        m_fontTexture.OnResetDevice();
    } catch(...){}
}

