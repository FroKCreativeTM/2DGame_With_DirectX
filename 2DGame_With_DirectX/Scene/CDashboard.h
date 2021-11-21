#pragma once

#include "../Graphics/CImage.h"
#include "../Const.h"

namespace NSDashboard
{
    const int   IMAGE_SIZE = 32;        // �ؽ�ó�� ������
    const int   TEXTURE_COLS = 8;       // �ؽ�ó�� col#
    const int   BAR_FRAME = 44;         // ���� ������ �ѹ�
}

class CDashboard :
    public CImage {
public:
    // ��ú��� �ٸ� �ʱ�ȭ�Ѵ�.
    bool Initialize(CGraphics* pGraphics, CTextureManager* pTextureManager, int left, int top,
        float scale, COLOR_ARGB color);
    // �� ����� �����Ѵ�.
    void Set(float percentOn);
    virtual void Update(float frameTime) {}
};

