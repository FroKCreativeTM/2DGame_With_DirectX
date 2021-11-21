#pragma once

#include "../Graphics/CImage.h"
#include "../Const.h"

namespace NSDashboard
{
    const int   IMAGE_SIZE = 32;        // 텍스처의 사이즈
    const int   TEXTURE_COLS = 8;       // 텍스처의 col#
    const int   BAR_FRAME = 44;         // 바의 프레임 넘버
}

class CDashboard :
    public CImage {
public:
    // 대시보드 바를 초기화한다.
    bool Initialize(CGraphics* pGraphics, CTextureManager* pTextureManager, int left, int top,
        float scale, COLOR_ARGB color);
    // 바 사이즈를 조절한다.
    void Set(float percentOn);
    virtual void Update(float frameTime) {}
};

