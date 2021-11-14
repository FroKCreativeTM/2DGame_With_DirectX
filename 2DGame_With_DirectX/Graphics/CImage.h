#pragma once

#include "../CGraphics.h"
#include "CTextureManager.h"

class CImage {
protected : 
	bool                m_initialized;  // 성공적으로 초기화시 true 

	CGraphics*          m_graphics;
	CTextureManager *   m_textureManager;
	SPRITE_DATA         m_spriteData;

    COLOR_ARGB          m_colorFilter;      // 컬러 필터 적용
	int                 m_cols;
    int                 m_startFrame;     // 애니메이션 시작 프레임
    int                 m_endFrame;       // 애니메이션 끝 프레임
    int                 m_currentFrame;   // 애니메이션 현재 프레임
    float               m_frameDelay;     // 프레임간의 시간
    float               m_animTimer;      // 애니메이션 타이머
    HRESULT             m_result;         
    bool                m_loop;           // 루프 프레임을 도는가
    bool                m_visible;        // 보일 때 true
    bool                m_animComplete;   // endFrmame이고 동시에 loop가 false가 되면 true 
public : 
    CImage();
    virtual ~CImage();
	bool Initialize(CGraphics* g, int width, int height, 
		int nCols, CTextureManager* textureManager);

    virtual void Draw(COLOR_ARGB color = NSGraphics::WHITE);
    virtual void Draw(SPRITE_DATA spriteData, COLOR_ARGB color = NSGraphics::WHITE);

    virtual void Update(float frameTime);

    // 수평 뒤집기
    virtual void FlipHorizontal(bool flip) {
        m_spriteData.flipHorizontal = flip;
    }

    // 수직 뒤집기
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

