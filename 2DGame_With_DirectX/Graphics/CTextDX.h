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
    // ����&�Ҹ���
    CTextDX();
    virtual ~CTextDX();

    // DX�� ��Ʈ�� �ʱ�ȭ�Ѵ�.
    virtual bool Initialize(CGraphics* g, int height, bool bold, bool italic, const std::string& fontName);

    // spriteBegin()/spriteEnd() ���̿��� x�� y���� �����Ѵ�. (������ height ��ȯ, �ƴϸ� 0��ȯ)
    virtual int Print(const std::string& str, int x, int y);

    // spriteBegin()/spriteEnd() ���̿��� �ҷ��ͼ� RECT�ȿ� �� ���ڸ� �����Ѵ�.
    virtual int Print(const std::string& str, RECT& rect, UINT format);

    // ȸ�� ������ degree �������� ��ȯ
    virtual float GetDegrees() { return m_angle * (180.0f / (float)PI); }

    // ȸ�� ������ radian �������� ��ȯ
    virtual float GetRadians() { return m_angle; }

    // �÷� ��ȯ
    virtual COLOR_ARGB GetFontColor() { return m_color; }

    // ȸ�� ������ degree �������� ����
    virtual void SetDegrees(float deg) { m_angle = deg * ((float)PI / 180.0f); }

    // ȸ�� ������ radian �������� ����(�ð� ����)
    virtual void SetRadians(float rad) { m_angle = rad; }

    // �� ����
    virtual void SetFontColor(COLOR_ARGB c) { m_color = c; }

    // Release resources
    virtual void OnLostDevice();

    // Restore resources
    virtual void OnResetDevice();
};

