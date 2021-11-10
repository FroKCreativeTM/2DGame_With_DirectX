#include "CImage.h"

CImage::CImage() {
    m_initialized = false;           
    m_spriteData.width = 2;
    m_spriteData.height = 2;
    m_spriteData.x = 0.0;
    m_spriteData.y = 0.0;
    m_spriteData.scale = 1.0;
    m_spriteData.angle = 0.0;
    m_spriteData.rect.left = 0;       
    m_spriteData.rect.top = 0;
    m_spriteData.rect.right = m_spriteData.width;
    m_spriteData.rect.bottom = m_spriteData.height;
    m_spriteData.texture = NULL;     
    m_spriteData.flipHorizontal = false;
    m_spriteData.flipVertical = false;
    m_cols = 1;
    m_textureManager = NULL;
    m_startFrame = 0;
    m_endFrame = 0;
    m_currentFrame = 0;
    m_frameDelay = 1.0;               
    m_animTimer = 0.0;
    m_visible = true;                 
    m_loop = true;                    
    m_animComplete = false;
    m_graphics = NULL;                
    m_colorFilter = NSGraphics::WHITE;
}

CImage::~CImage() {
}

bool CImage::Initialize(CGraphics* g, int width, int height,
    int nCols, CTextureManager* textureManager) {
    try {
        m_graphics = g;
        m_textureManager = textureManager;
        m_spriteData.texture = m_textureManager->GetTexture();

        if (width == 0) {
            width = m_textureManager->GetWidth();   // 전체 폭 사용
        }
        m_spriteData.width = width;
        if (height == 0) {
            height = m_textureManager->GetHeight(); // 전체 높이 사용
        }
        m_spriteData.height = height;

        m_cols = nCols;

        if (m_cols == 0) {
            m_cols = 1; // cols가 0이라면 1을 사용
        }

        // currentFrame을 그리기 위해 spriteData.rect를 구성한다.
        m_spriteData.rect.left = (m_currentFrame % m_cols) * m_spriteData.width;
        m_spriteData.rect.right = m_spriteData.rect.left + m_spriteData.width;
        m_spriteData.rect.top = (m_currentFrame / m_cols) * m_spriteData.height;
        m_spriteData.rect.bottom = m_spriteData.rect.top + m_spriteData.height;
    }
    catch (...) {
        return false;
    }

    m_initialized = true;
    return true;
}

void CImage::Draw(COLOR_ARGB color) {
    if (!m_visible || m_graphics == nullptr) {
        return;
    }

    // onReset이 호출된 경우를 대비해 새로운 텍스처를 가져온다.
    m_spriteData.texture = m_textureManager->GetTexture();

    // 필터를 사용해서 그리는 경우
    if (color == NSGraphics::FILTER) {
        m_graphics->DrawSprite(m_spriteData, m_colorFilter);
    }
    else {
        m_graphics->DrawSprite(m_spriteData, color);
    }
}

void CImage::Draw(SPRITE_DATA spriteData, COLOR_ARGB color) {
    if (!m_visible || m_graphics == nullptr) {
        return;
    }

    spriteData.rect = m_spriteData.rect;

    // onReset이 호출된 경우를 대비해 새로운 텍스처를 가져온다.
    spriteData.texture = m_textureManager->GetTexture();

    // 필터를 사용해서 그리는 경우
    if (color == NSGraphics::FILTER) {
        m_graphics->DrawSprite(spriteData, m_colorFilter);
    }
    else {
        m_graphics->DrawSprite(spriteData, color);
    }
}

void CImage::Update(float frameTime) {
    if (m_endFrame - m_startFrame > 0) {
        m_animTimer += frameTime; // 시간 경과

        if (m_animTimer > m_frameDelay) {
            m_animTimer -= m_frameDelay;
            m_currentFrame++;

            if (m_currentFrame < m_startFrame || m_currentFrame > m_endFrame) {
                if (m_loop) { // 반복 애니메이션이라면
                    m_currentFrame = m_startFrame;
                }
                else {
                    m_currentFrame = m_endFrame;
                    m_animComplete = true;
                }
            }
            SetRect();
        }
    }
}

void CImage::SetCurrentFrame(int c) {
    if (c >= 0)
    {
        m_currentFrame = c;
        m_animComplete = false;
        SetRect();                          
    }
}

void CImage::SetRect() {
    m_spriteData.rect.left = (m_currentFrame % m_cols) * m_spriteData.width;
    m_spriteData.rect.right = m_spriteData.rect.left + m_spriteData.width;
    m_spriteData.rect.top = (m_currentFrame / m_cols) * m_spriteData.height;
    m_spriteData.rect.bottom = m_spriteData.rect.top + m_spriteData.height;
}
