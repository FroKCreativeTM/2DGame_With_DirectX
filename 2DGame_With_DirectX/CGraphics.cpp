#include "CGraphics.h"
#include "CGameError.h"

void CGraphics::initD3DApp()
{
    // D3D 프레젠테이션 매개변수를 초기화한다.
    try {
        ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
        // 필요한 매개변수를 채운다.
        m_d3dpp.BackBufferWidth = m_width;
        m_d3dpp.BackBufferHeight = m_height;

        if (m_fullscreen) {
            m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 24비트 색상을 사용한다.
        }
        else {
            m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;  // 데스크탑 설정을 그대로 사용한다.
        }

        m_d3dpp.BackBufferCount = 1;    // 백버퍼는 하나만
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

    // 다이렉트 초기화
    m_direct3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (m_direct3d == nullptr) {
        throw(CGameError(NSGameError::FATAL_ERROR, "Error initializing Direct3D"));
    }

    initD3DApp();   // D3D 프레젠테이션 매개변수 초기화

    if (m_fullscreen) {
        if (IsAdapterCompatible()) {    // 어댑터가 호환되면
            m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 24비트 색상을 사용한다.
        }
        else {
            throw(CGameError(NSGameError::FATAL_ERROR,
                "The graphics device does not support the specified resolution and/or format."));
        }
    }

    // 그래픽 카드가 텍스처, 라이팅, 정점 셰이더를 지원하는 지 확인한다.
    D3DCAPS9 caps;
    DWORD behavior;
    m_result = m_direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL, &caps);

    // 디바이스가 텍스처나 라이팅을 지원하지 않거나
    // 정점 셰이더 1.1을 지원하지 않는다면 소프트웨어를 통해서 정점 처리를 실시한다.
    if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
        caps.VertexShaderVersion < D3DVS_VERSION(1, 1)) {
        behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
    else {
        behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }

    // 윈도우 표준에 맞춰서 D3D 디바이스를 생성한다.
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
}

HRESULT CGraphics::ShowBackBuffer()
{
    m_result = E_FAIL; // 기본 값은 실패로 두고 성공시 바꾼다.

    m_device3d->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0),
        0.0f, 0);

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
