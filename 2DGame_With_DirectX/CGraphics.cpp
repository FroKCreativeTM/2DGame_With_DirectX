#include "CGraphics.h"
#include "CGameError.h"

void CGraphics::initD3DApp()
{
    // D3D ���������̼� �Ű������� �ʱ�ȭ�Ѵ�.
    try {
        ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
        // �ʿ��� �Ű������� ä���.
        m_d3dpp.BackBufferWidth = m_width;
        m_d3dpp.BackBufferHeight = m_height;

        if (m_fullscreen) {
            m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 24��Ʈ ������ ����Ѵ�.
        }
        else {
            m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;  // ����ũž ������ �״�� ����Ѵ�.
        }

        m_d3dpp.BackBufferCount = 1;    // ����۴� �ϳ���
        m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        m_d3dpp.hDeviceWindow = m_hwnd;
        m_d3dpp.Windowed = (!m_fullscreen);
        m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }
    catch (...) {
        throw(CGameError(NSGameError::FATAL_ERROR, "Error Initializing D3D Presentation Parameters"));
    } 
}

CGraphics::CGraphics()
{
    m_direct3d = nullptr;
    m_device3d = nullptr;
    m_fullscreen = false;
    m_width = GAME_WIDTH;
    m_height = GAME_HEIGHT;
    m_backColor = NSGraphics::BLUE;
}

CGraphics::~CGraphics()
{
    ReleaseAll();
}

void CGraphics::ReleaseAll()
{
    SAFE_RELEASE(m_device3d);
    SAFE_RELEASE(m_direct3d);
}

void CGraphics::Initialize(HWND hwnd, int width, int height, bool fullscreen)
{
    m_hwnd = hwnd;
    m_width = width;
    m_height = height;
    m_fullscreen = fullscreen;

    // ���̷�Ʈ �ʱ�ȭ
    m_direct3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (m_direct3d == nullptr) {
        throw(CGameError(NSGameError::FATAL_ERROR, "Error initializing Direct3D"));
    }

    initD3DApp();   // D3D ���������̼� �Ű����� �ʱ�ȭ

    if (m_fullscreen) {
        if (IsAdapterCompatible()) {    // ����Ͱ� ȣȯ�Ǹ�
            m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 24��Ʈ ������ ����Ѵ�.
        }
        else {
            throw(CGameError(NSGameError::FATAL_ERROR,
                "The graphics device does not support the specified resolution and/or format."));
        }
    }

    // �׷��� ī�尡 �ؽ�ó, ������, ���� ���̴��� �����ϴ� �� Ȯ���Ѵ�.
    D3DCAPS9 caps;
    DWORD behavior;
    m_result = m_direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL, &caps);

    // ����̽��� �ؽ�ó�� �������� �������� �ʰų�
    // ���� ���̴� 1.1�� �������� �ʴ´ٸ� ����Ʈ��� ���ؼ� ���� ó���� �ǽ��Ѵ�.
    if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
        caps.VertexShaderVersion < D3DVS_VERSION(1, 1)) {
        behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
    else {
        behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }

    // ������ ǥ�ؿ� ���缭 D3D ����̽��� �����Ѵ�.
    m_result = m_direct3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        this->m_hwnd,
        behavior,
        &m_d3dpp,
        &m_device3d
    );

    if (FAILED(m_result)) {
        throw(CGameError(NSGameError::FATAL_ERROR,
            "Error creating Direct3D device"));
    }

    m_result = D3DXCreateSprite(m_device3d, &m_sprite);
    if (FAILED(m_result)) {
        throw(CGameError(NSGameError::FATAL_ERROR,
            "Error creating Direct3D sprite"));
    }
}

HRESULT CGraphics::ShowBackBuffer()
{
    m_result = E_FAIL; // �⺻ ���� ���з� �ΰ� ������ �ٲ۴�.

    m_result = m_device3d->Present(nullptr, nullptr, nullptr, nullptr);

    return m_result;
}

bool CGraphics::IsAdapterCompatible()
{
    UINT modes = m_direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, m_d3dpp.BackBufferFormat);

    for (UINT i = 0; i < modes; ++i)
    {
        m_result = m_direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
            m_d3dpp.BackBufferFormat, i, &m_pMode);

        if (m_pMode.Height == m_d3dpp.BackBufferHeight &&
            m_pMode.Width == m_d3dpp.BackBufferWidth &&
            m_pMode.RefreshRate >= m_d3dpp.FullScreen_RefreshRateInHz) {
            return true;
        }
    }

    return false;
}

HRESULT CGraphics::GetDeviceState()
{
    m_result = E_FAIL;

    if (m_device3d == nullptr) {
        return m_result;
    }
    m_result = m_device3d->TestCooperativeLevel();
    return m_result;
}

HRESULT CGraphics::Reset()
{
    m_result = E_FAIL;

    // D3D ���������̼� �Ű������� �ٽ� �ʱ�ȭ�Ѵ�.
    initD3DApp();

    m_result = m_device3d->Reset(&m_d3dpp);

    return m_result;

}

HRESULT CGraphics::LoadTexture(const char* fileName, COLOR_ARGB transColor, 
    UINT& width, UINT& height, LP_TEXTURE & texture) {
    D3DXIMAGE_INFO info;
    m_result = E_FAIL;

    try {
        if (fileName == nullptr) {
            m_texture = nullptr;
            return D3DERR_INVALIDCALL;
        }

        m_result = D3DXGetImageInfoFromFile(fileName, &info);
        
        if (m_result != D3D_OK) {
            return m_result;
        }

        width = info.Width;
        height = info.Height;

        m_result = D3DXCreateTextureFromFileEx(
            m_device3d,
            fileName,
            info.Width,
            info.Height,
            1,  // �Ӹ� ����(���� �� �Ҷ��� 1)
            0,
            D3DFMT_UNKNOWN,     // ǥ�� ����
            D3DPOOL_DEFAULT,    // �ؽ�ó�� ���� �޸� Ŭ����
            D3DX_DEFAULT,       // �̹��� ����
            D3DX_DEFAULT,       // �� ����
            transColor,         // ������ ���� ���� Ű
            &info,              // ��Ʈ�� ���� ����(�ҷ��� ���Ϸκ���)
            nullptr,
            &texture);
    }
    catch (...) {
        throw(CGameError(NSGameError::FATAL_ERROR,
            "Error creating Graphics::LoadTexture()"));
    }

    return m_result;
}

void CGraphics::DrawSprite(const SPRITE_DATA& spriteData, COLOR_ARGB color) {
    if (spriteData.texture == nullptr) { // �ؽ�ó�� ���ٸ� ������.
        return;
    }

    // ���� ��������Ʈ�� �߽��� ã�´�.
    D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale),
        (float)(spriteData.height / 2 * spriteData.scale));

    // ��������Ʈ�� ȭ�� ��ġ
    D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);

    // X, Y�� ũ�⸦ �����Ѵ�.
    D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

    // ���� �������� ������� �Ѵٸ�
    if (spriteData.flipHorizontal) {
        scaling.x *= -1;    // ������ ���ؼ� y �������� -1�� ���Ѵ�.

        // ������ �̹����� �߽��� �����´�.
        spriteCenter.x -= (float)(spriteData.width * spriteData.scale);

        // ���� �𼭸����� ������ �۾��� �Ͼ�Ƿ� 
        // ������ �̹����� ���������� �̵����� ���� ��ġ�� �ְ� �Ѵ�.
        translate.x += (float)(spriteData.width * spriteData.scale);
    }
    // ���� �������� ������� �Ѵٸ�
    if (spriteData.flipHorizontal) {
        scaling.y *= -1;    // ������ ���ؼ� y �������� -1�� ���Ѵ�.

        // ������ �̹����� �߽��� �����´�.
        spriteCenter.y -= (float)(spriteData.height * spriteData.scale);

        // ���� �𼭸����� ������ �۾��� �Ͼ�Ƿ� 
        // ������ �̹����� �Ʒ������� �̵����� ���� ��ġ�� �ְ� �Ѵ�.
        translate.y += (float)(spriteData.height * spriteData.scale);
    }

    // ��������Ʈ�� ȸ�� �� ũ�� ����, ��ġ�ϱ� ���� ����� �����Ѵ�.
    D3DXMATRIX matrix;
    D3DXMatrixTransformation2D(
        &matrix,                    // ���
        nullptr,                    // ũ�⸦ ������ �� ������ ���� ������� ����
        0.0f,                       // ũ�� ���� ȸ�� ����
        &scaling,                   // ũ�� ���� ��
        &spriteCenter,              // ȸ�� �߽�
        (float)(spriteData.angle),  // ȸ�� ����
        &translate                  // X, Y ��ġ
    );

    // ��������Ʈ�� ��� ������ �����Ѵ�.
    m_sprite->SetTransform(&matrix);

    m_sprite->Draw(spriteData.texture, &spriteData.rect, nullptr, nullptr, color);
}

HRESULT CGraphics::BeginScene() {
    m_result = E_FAIL;

    if (m_device3d == nullptr) {
        return m_result;
    }

    m_device3d->Clear(0, nullptr, D3DCLEAR_TARGET, m_backColor, 0.1f, 0);
    m_result = m_device3d->BeginScene();
    return m_result;
}

HRESULT CGraphics::EndScene() {
    m_result = E_FAIL;
    if (m_device3d) {
        m_device3d->EndScene();
    }
    return m_result;
}
