#include "CDashboard.h"

bool CDashboard::Initialize(CGraphics* pGraphics, CTextureManager* pTextureManager, 
    int left, int top, 
    float scale, COLOR_ARGB color) {
    try {
        CImage::Initialize(pGraphics, NSDashboard::IMAGE_SIZE, NSDashboard::IMAGE_SIZE,
            NSDashboard::TEXTURE_COLS, pTextureManager);

        SetCurrentFrame(NSDashboard::BAR_FRAME);
        m_spriteData.x = (float)left;
        m_spriteData.y = (float)top;
        m_spriteData.scale = scale;
        m_colorFilter = color;
    }
    catch (...) {
        return false;
    }
    // 완료되면 true 리턴
    return true;
}

void CDashboard::Set(float percentOn) {
    if (percentOn < 0) {
        percentOn = 0;
    }
    else if (percentOn > 100) {
        percentOn = 100;
    }
    m_spriteData.rect.right = m_spriteData.rect.left + (LONG)(m_spriteData.width * percentOn / 100);
}
