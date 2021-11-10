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
            width = m_textureManager->GetWidth();   // ��ü �� ���
        }
        m_spriteData.width = width;
        if (height == 0) {
            height = m_textureManager->GetHeight(); // ��ü ���� ���
        }
        m_spriteData.height = height;

        m_cols = nCols;

        if (m_cols == 0) {
            m_cols = 1; // cols�� 0�̶�� 1�� ���
        }

        // currentFrame�� �׸��� ���� spriteData.rect�� �����Ѵ�.
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

    // onReset�� ȣ��� ��츦 ����� ���ο� �ؽ�ó�� �����´�.
    m_spriteData.texture = m_textureManager->GetTexture();

    // ���͸� ����ؼ� �׸��� ���
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

    // onReset�� ȣ��� ��츦 ����� ���ο� �ؽ�ó�� �����´�.
    spriteData.texture = m_textureManager->GetTexture();

    // ���͸� ����ؼ� �׸��� ���
    if (color == NSGraphics::FILTER) {
        m_graphics->DrawSprite(spriteData, m_colorFilter);
    }
    else {
        m_graphics->DrawSprite(spriteData, color);
    }
}

void CImage::Update(float frameTime) {
    if (m_endFrame - m_startFrame > 0) {
        m_animTimer += frameTime; // �ð� ���

        if (m_animTimer > m_frameDelay) {
            m_animTimer -= m_frameDelay;
            m_currentFrame++;

            if (m_currentFrame < m_startFrame || m_currentFrame > m_endFrame) {
                if (m_loop) { // �ݺ� �ִϸ��̼��̶��
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
