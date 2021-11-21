#pragma once

#define WIN32_LEAN_AND_MEAN
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include "Macro.h"
#include "Const.h"

/* ================== 자료형 선언 ================== */
#define LP_3DDEVICE			LPDIRECT3DDEVICE9
#define LP_3D				LPDIRECT3D9
#define LP_TEXTURE			LPDIRECT3DTEXTURE9
#define LP_SPRITE			LPD3DXSPRITE
#define LP_VERTEXBUFFER		IDirect3DVertexBuffer9*

/* ================== 구조체 선언 ================== */
typedef struct _tagSpriteData {
	int	width;				// 스프라이트의 픽셀 폭
	int height;				// 스프라이트의 픽셀 높이
	float x;				// 화면 위치
	float y;
	float scale;			// <1이면 작게, > 1이면 크게
	float angle;			// 라디안 단위의 회전 각도
	RECT rect;				// 큰 텍스처에서 사용할 이미지 선택
	LP_TEXTURE texture;		// 텍스처 리소스가 저장된 주소를 가리키는 포인터
	bool flipHorizontal;	// true인 경우 수평으로 뒤집는다.
	bool flipVertical;		// 수직으로 뒤집는다.
}SPRITE_DATA, *PSPRITE_DATA;

/* ================== 매크로 선언 ================== */
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a, r, g, b) \
	((COLOR_ARGB) ((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | \
		(((g) & 0xff) << 8) | ((b) & 0xff)))


namespace NSGraphics
{
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);			// colorFilter를 명시해서 그릴 때 사용
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);  	// 25% 알파값
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255); 	// 50% 알파값
	const COLOR_ARGB BACK_COLOR = NAVY;			
	const COLOR_ARGB TRANSCOLOR = MAGENTA;

	enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}

struct VertexC {
	float x, y, z;			// DirectX는 3D기 때문에 3개의 축을 중심으로 한다.
	float rhw;				// homogeneous w의 역수 좌표로써 1로 세팅한다.  
	unsigned long color;	// 버텍스의 컬러를 지칭한다.
};

// D3DFVF_XYZRHW : 이 버텍스들은 transform되었다.
// D3DFVF_XYZRHW : 디퓨즈(난반사) 컬러 데이터를 가지고 있다.
#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class CGraphics
{
private : 
	// DirectX 포인터와 관련 변수
	LP_3D						m_direct3d;
	LP_3DDEVICE					m_device3d;
	D3DPRESENT_PARAMETERS		m_d3dpp;
	D3DDISPLAYMODE				m_pMode;
	LP_SPRITE					m_sprite;		// DirectX 스프라이트는 DirectX 프리미티브 타입과 텍스터 데이터로 구성되어있음
												// 2D 이미지를 그리기 위해 설계된 DirectX API의 일부
	LP_TEXTURE					m_texture;
	COLOR_ARGB					m_backColor;     

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

	// 디바이스 로스트를 확인한다.
	HRESULT GetDeviceState();

	// 그래픽 디바이스를 리셋한다.
	HRESULT Reset();

	LP_3DDEVICE Get3Ddevice() const { return m_device3d; }

	// 텍스처를 로딩하는 메서드
	HRESULT LoadTexture(const char* fileName, COLOR_ARGB transColor, 
		UINT& width, UINT& height, LP_TEXTURE & texture);

	// 스프라이트를 그리기 위한 메서드
	void DrawSprite(const SPRITE_DATA& spriteData, COLOR_ARGB color);
	LP_SPRITE GetSprite() const { return m_sprite; }


	void SetBackColor(COLOR_ARGB c) { m_backColor = c; }

	// Quad를 그리는 함수
	bool DrawQuad(LP_VERTEXBUFFER vertexBuffer);
	// 정점 버퍼를 생성하는 함수
	HRESULT CreateVertexBuffer(VertexC vertices[], UINT size, LP_VERTEXBUFFER& vertexBuffer);

	// 텍스처를 시스템 메모리에서 불러온다.
	// 시스템 메모리는 기본적으로 락을 걸 수 있다
	// 픽셀 데이터에 직접적인 접근이 가능하다.
	// 이를 이용해서 TetureData 구조체를 채운다.
	HRESULT LoadTextureSystemMem(const char* fileName, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture);

	// 게임의 신을 시작하고 끝내는 함수
	HRESULT BeginScene();
	HRESULT EndScene();

	// 스프라이트 그리기 시작
	void SpriteBegin() {
		m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	// 스프라이트 그리기 끝
	void SpriteEnd() {
		m_sprite->End();
	}
};

