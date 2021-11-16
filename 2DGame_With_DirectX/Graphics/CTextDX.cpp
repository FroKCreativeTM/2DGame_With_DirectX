// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textDX.cpp v1.0
// DirectX font

#include "CTextDX.h"

//=============================================================================
// default constructor
//=============================================================================
CTextDX::CTextDX()
{
    m_color = SETCOLOR_ARGB(255,255,255,255); // default to white font

    // set font position
    m_fontRect.top = 0;
    m_fontRect.left = 0;
    m_fontRect.right = GAME_WIDTH;
    m_fontRect.bottom = GAME_HEIGHT;
    m_dxFont = nullptr;
    m_angle  = 0;
}

//=============================================================================
// destructor
//=============================================================================
CTextDX::~CTextDX()
{
    SAFE_RELEASE(m_dxFont);
}

//=============================================================================
// Create DirectX Font
//=============================================================================
bool CTextDX::Initialize(CGraphics *g, int height, bool bold, bool italic,
                        const std::string &fontName)
{
    m_pGraphics = g;                   // the graphics system

    UINT weight = FW_NORMAL;
    if(bold)
        weight = FW_BOLD;

    // create DirectX font
    if (FAILED(D3DXCreateFont(m_pGraphics->Get3Ddevice(), height, 0, weight, 1, italic,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(),
        &m_dxFont))) {
        return false;
    }

    // Create the tranformation matrix
    D3DXMatrixTransformation2D(&m_matrix, nullptr, 0.0f, nullptr, nullptr, 0.0f, nullptr);

    return true;
}

//=============================================================================
// Print text at x,y 
// Return 0 on fail, height of text on success
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
int CTextDX::Print(const std::string &str, int x, int y)
{
    if(m_dxFont == nullptr)
        return 0;
    // set font position
    m_fontRect.top = y;
    m_fontRect.left = x;

    // Rotation center
    D3DXVECTOR2 rCenter=D3DXVECTOR2((float)x,(float)y);
    // Setup matrix to rotate text by angle
    D3DXMatrixTransformation2D(&m_matrix, nullptr, 0.0f, nullptr, &rCenter, m_angle, nullptr);
    // Tell the sprite about the matrix "Hello Neo"
    m_pGraphics->GetSprite()->SetTransform(&m_matrix);
    return m_dxFont->DrawTextA(m_pGraphics->GetSprite(), str.c_str(), -1, &m_fontRect, DT_LEFT, m_color);
}

//=============================================================================
// Print text inside RECT using DirectX text format
// Return 0 on fail, height of text on success
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
int CTextDX::Print(const std::string &str, RECT &rect, UINT format)
{
    if(m_dxFont == nullptr)
        return 0;

    // Setup matrix to not rotate text
    D3DXMatrixTransformation2D(&m_matrix, nullptr, 0.0f, nullptr, nullptr, NULL, nullptr);
    // Tell the sprite about the matrix "Hello Neo"
    m_pGraphics->GetSprite()->SetTransform(&m_matrix);
    return m_dxFont->DrawTextA(m_pGraphics->GetSprite(), str.c_str(), -1, &rect, format, m_color);
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void CTextDX::OnLostDevice()
{
    if(m_dxFont == nullptr)
        return;
    m_dxFont->OnLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void CTextDX::OnResetDevice()
{
    if(m_dxFont == nullptr)
        return;
    m_dxFont->OnResetDevice();
}
