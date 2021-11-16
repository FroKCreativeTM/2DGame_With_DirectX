// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textDX.h v1.0
// DirectX font

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "../Const.h"
#include "../CGraphics.h"

#define LP_DXFONT LPD3DXFONT 

class CTextDX
{
private:
    CGraphics* m_pGraphics;
    COLOR_ARGB          m_color;          // font color (a,r,g,b)
    LP_DXFONT           m_dxFont;
    RECT                m_fontRect;       // text rectangle
    // matrix to rotate the text
    D3DXMATRIX          m_matrix;
    float               m_angle;          // rotation angle of text in radians

public:
    // 생성&소멸자
    CTextDX();
    virtual ~CTextDX();

    // DX의 폰트를 초기화한다.
    virtual bool Initialize(CGraphics* g, int height, bool bold, bool italic, const std::string& fontName);

    // spriteBegin()/spriteEnd() 사이에서 x과 y축을 설정한다. (성공시 height 반환, 아니면 0반환)
    virtual int Print(const std::string& str, int x, int y);

    // spriteBegin()/spriteEnd() 사이에서 불러와서 RECT안에 들어갈 글자를 설정한다.
    virtual int Print(const std::string& str, RECT& rect, UINT format);

    // 회전 각도를 degree 형식으로 반환
    virtual float GetDegrees() { return m_angle * (180.0f / (float)PI); }

    // 회전 각도를 radian 형식으로 반환
    virtual float GetRadians() { return m_angle; }

    // 컬러 반환
    virtual COLOR_ARGB GetFontColor() { return m_color; }

    // 회전 각도를 degree 형식으로 설정
    virtual void SetDegrees(float deg) { m_angle = deg * ((float)PI / 180.0f); }

    // 회전 각도를 radian 형식으로 설정(시계 방향)
    virtual void SetRadians(float rad) { m_angle = rad; }

    // 색 설정
    virtual void SetFontColor(COLOR_ARGB c) { m_color = c; }

    // Release resources
    virtual void OnLostDevice();

    // Restore resources
    virtual void OnResetDevice();
};

