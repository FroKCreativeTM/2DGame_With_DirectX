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

    m_result = D3DXCreateSprite(m_device3d, &m_sprite);
    if (FAILED(m_result)) {
        throw(CGameError(NSGameError::FATAL_ERROR,
            "Error creating Direct3D sprite"));
    }
}

HRESULT CGraphics::ShowBackBuffer()
{
    m_result = E_FAIL; // 기본 값은 실패로 두고 성공시 바꾼다.

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

    // D3D 프레젠테이션 매개변수를 다시 초기화한다.
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
            1,  // 밉맵 수준(연결 안 할때는 1)
            0,
            D3DFMT_UNKNOWN,     // 표면 형식
            D3DPOOL_DEFAULT,    // 텍스처를 위한 메모리 클래스
            D3DX_DEFAULT,       // 이미지 필터
            D3DX_DEFAULT,       // 밉 필터
            transColor,         // 투명을 위한 색상 키
            &info,              // 비트맵 파일 정보(불러온 파일로부터)
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
    if (spriteData.texture == nullptr) { // 텍스처가 없다면 끝낸다.
        return;
    }

    // 먼저 스프라이트의 중심을 찾는다.
    D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale),
        (float)(spriteData.height / 2 * spriteData.scale));

    // 스프라이트의 화면 위치
    D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);

    // X, Y의 크기를 조정한다.
    D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

    // 만약 수평으로 뒤집어야 한다면
    if (spriteData.flipHorizontal) {
        scaling.x *= -1;    // 뒤집기 위해서 y 방향으로 -1을 곱한다.

        // 뒤집은 이미지의 중심을 가져온다.
        spriteCenter.x -= (float)(spriteData.width * spriteData.scale);

        // 왼쪽 모서리에서 뒤집는 작업이 일어나므로 
        // 뒤집은 이미지를 오른쪽으로 이동시켜 원래 위치에 있게 한다.
        translate.x += (float)(spriteData.width * spriteData.scale);
    }
    // 만약 수직으로 뒤집어야 한다면
    if (spriteData.flipHorizontal) {
        scaling.y *= -1;    // 뒤집기 위해서 y 방향으로 -1을 곱한다.

        // 뒤집은 이미지의 중심을 가져온다.
        spriteCenter.y -= (float)(spriteData.height * spriteData.scale);

        // 위쪽 모서리에서 뒤집는 작업이 일어나므로 
        // 뒤집은 이미지를 아래쪽으로 이동시켜 원래 위치에 있게 한다.
        translate.y += (float)(spriteData.height * spriteData.scale);
    }

    // 스프라이트를 회전 및 크기 조정, 배치하기 위한 행렬을 생성한다.
    D3DXMATRIX matrix;
    D3DXMatrixTransformation2D(
        &matrix,                    // 행렬
        nullptr,                    // 크기를 조정할 때 기준을 왼쪽 상단으로 유지
        0.0f,                       // 크기 조정 회전 없음
        &scaling,                   // 크기 조정 값
        &spriteCenter,              // 회전 중심
        (float)(spriteData.angle),  // 회전 각도
        &translate                  // X, Y 위치
    );

    // 스프라이트의 행렬 정보를 전달한다.
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
