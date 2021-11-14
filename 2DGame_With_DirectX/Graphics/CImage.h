#pragma once

#include "../CGraphics.h"
#include "CTextureManager.h"

class CImage {
protected : 
	bool                m_initialized;  // ���������� �ʱ�ȭ�� true 

	CGraphics*          m_graphics;
	CTextureManager *   m_textureManager;
	SPRITE_DATA         m_spriteData;

    COLOR_ARGB          m_colorFilter;      // �÷� ���� ����
	int                 m_cols;
    int                 m_startFrame;     // �ִϸ��̼� ���� ������
    int                 m_endFrame;       // �ִϸ��̼� �� ������
    int                 m_currentFrame;   // �ִϸ��̼� ���� ������
    float               m_frameDelay;     // �����Ӱ��� �ð�
    float               m_animTimer;      // �ִϸ��̼� Ÿ�̸�
    HRESULT             m_result;         
    bool                m_loop;           // ���� �������� ���°�
    bool                m_visible;        // ���� �� true
    bool                m_animComplete;   // endFrmame�̰� ���ÿ� loop�� false�� �Ǹ� true 
public : 
    CImage();
    virtual ~CImage();
	bool Initialize(CGraphics* g, int width, int height, 
		int nCols, CTextureManager* textureManager);

    virtual void Draw(COLOR_ARGB color = NSGraphics::WHITE);
    virtual void Draw(SPRITE_DATA spriteData, COLOR_ARGB color = NSGraphics::WHITE);

    virtual void Update(float frameTime);

    // ���� ������
    virtual void FlipHorizontal(bool flip) {
        m_spriteData.flipHorizontal = flip;
    }

    // ���� ������
    virtual void FlipVertical(bool flip) {
        m_spriteData.flipVertical = flip;
    }

    /* ======================= Getter/Setter ======================= */
    const virtual SPRITE_DATA& GetSpriteInfo() { return m_spriteData; }
    virtual bool  GetVisible() { return m_visible; }
    virtual float GetX() { return m_spriteData.x; }
    virtual float GetY() { return m_spriteData.y; }
    virtual float GetScale() { return m_spriteData.scale; }
    virtual int   GetWidth() { return m_spriteData.width; }
    virtual int   GetHeight() { return m_spriteData.height; }
    virtual float GetCenterX() { return m_spriteData.x + m_spriteData.width / 2 * GetScale(); }
    virtual float GetCenterY() { return m_spriteData.y + m_spriteData.height / 2 * GetScale(); }
    virtual float GetDegrees() { return m_spriteData.angle * (180.0f / (float)PI); }
    virtual float GetRadians() { return m_spriteData.angle; }
    virtual float GetFrameDelay() { return m_frameDelay; }
    virtual int   GetStartFrame() { return m_startFrame; }
    virtual int   GetEndFrame() { return m_endFrame; }
    virtual int   GetCurrentFrame() { return m_currentFrame; }
    virtual RECT  GetSpriteDataRect() { return m_spriteData.rect; }
    virtual bool  GetAnimationComplete() { return m_animComplete; }
    virtual COLOR_ARGB GetColorFilter() { return m_colorFilter; }

    virtual void SetX(float newX) { m_spriteData.x = newX; }
    virtual void SetY(float newY) { m_spriteData.y = newY; }
    virtual void SetScale(float s) { m_spriteData.scale = s; }
    virtual void SetDegrees(float deg) { m_spriteData.angle = deg * ((float)PI / 180.0f); }
    virtual void SetRadians(float rad) { m_spriteData.angle = rad; }
    virtual void SetVisible(bool v) { m_visible = v; }
    virtual void SetFrameDelay(float d) { m_frameDelay = d; }
    virtual void SetFrames(int s, int e) { m_startFrame = s; m_endFrame = e; }
    virtual void SetCurrentFrame(int c);
    virtual void SetRect();
    virtual void SetSpriteDataRect(RECT r) { m_spriteData.rect = r; }
    virtual void SetLoop(bool lp) { m_loop = lp; }
    virtual void SetAnimationComplete(bool a) { m_animComplete = a; };
    virtual void SetColorFilter(COLOR_ARGB color) { m_colorFilter = color; }
    virtual void SetTextureManager(CTextureManager* textureManager)
    {
        m_textureManager = textureManager;
    }
};

