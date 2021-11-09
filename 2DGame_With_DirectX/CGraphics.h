#pragma once
#define WIN32_LEAN_AND_MEAN
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <Windows.h>
#include "Macro.h"
#include "Const.h"

/* ================== 자료형 선언 ================== */
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

/* ================== 매크로 선언 ================== */
#define COLOR_ARGB_DWORD
#define SETCOLOR_ARGB(a, r, g, b) \
	((COLOR_ARGB) ((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | \
		(((g) & 0xff) << 8) | ((b) & 0xff)))

class CGraphics
{
private : 
	// DirectX 포인터와 관련 변수
	LP_3D						m_direct3d;
	LP_3DDEVICE					m_device3d;
	D3DPRESENT_PARAMETERS		m_d3dpp;
	D3DDISPLAYMODE				m_pMode;

	// 윈도우 관련 변수
	HRESULT						m_result;
	HWND						m_hwnd;
	bool						m_fullscreen;
	int							m_width;
	int							m_height;

	// 엔진 내부에서만 사용하는 D3D 프레젠테이션 
	// 매개변수를 초기화한다.
	void initD3DApp();

public : 
	// 생성자
	CGraphics();

	// 소멸자
	virtual ~CGraphics();

	// 동적 할당된 Direct3D와 Device3D를 해제한다.
	void ReleaseAll();

	// DirectX 그래픽을 초기화한다.
	void Initialize(HWND hwnd, int width, int height, bool fullscreen);

	// 화면에 보이지 않는 백버퍼를 화면에 표기한다.
	HRESULT ShowBackBuffer();

	// 디스플레이 어뎁터의 호환성을 확인하는 함수
	bool IsAdapterCompatible();
};

