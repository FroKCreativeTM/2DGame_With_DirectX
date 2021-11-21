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

/* ================== �ڷ��� ���� ================== */
#define LP_3DDEVICE			LPDIRECT3DDEVICE9
#define LP_3D				LPDIRECT3D9
#define LP_TEXTURE			LPDIRECT3DTEXTURE9
#define LP_SPRITE			LPD3DXSPRITE
#define LP_VERTEXBUFFER		IDirect3DVertexBuffer9*

/* ================== ����ü ���� ================== */
typedef struct _tagSpriteData {
	int	width;				// ��������Ʈ�� �ȼ� ��
	int height;				// ��������Ʈ�� �ȼ� ����
	float x;				// ȭ�� ��ġ
	float y;
	float scale;			// <1�̸� �۰�, > 1�̸� ũ��
	float angle;			// ���� ������ ȸ�� ����
	RECT rect;				// ū �ؽ�ó���� ����� �̹��� ����
	LP_TEXTURE texture;		// �ؽ�ó ���ҽ��� ����� �ּҸ� ����Ű�� ������
	bool flipHorizontal;	// true�� ��� �������� �����´�.
	bool flipVertical;		// �������� �����´�.
}SPRITE_DATA, *PSPRITE_DATA;

/* ================== ��ũ�� ���� ================== */
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
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);			// colorFilter�� ����ؼ� �׸� �� ���
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);  	// 25% ���İ�
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255); 	// 50% ���İ�
	const COLOR_ARGB BACK_COLOR = NAVY;			
	const COLOR_ARGB TRANSCOLOR = MAGENTA;

	enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}

struct VertexC {
	float x, y, z;			// DirectX�� 3D�� ������ 3���� ���� �߽����� �Ѵ�.
	float rhw;				// homogeneous w�� ���� ��ǥ�ν� 1�� �����Ѵ�.  
	unsigned long color;	// ���ؽ��� �÷��� ��Ī�Ѵ�.
};

// D3DFVF_XYZRHW : �� ���ؽ����� transform�Ǿ���.
// D3DFVF_XYZRHW : ��ǻ��(���ݻ�) �÷� �����͸� ������ �ִ�.
#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class CGraphics
{
private : 
	// DirectX �����Ϳ� ���� ����
	LP_3D						m_direct3d;
	LP_3DDEVICE					m_device3d;
	D3DPRESENT_PARAMETERS		m_d3dpp;
	D3DDISPLAYMODE				m_pMode;
	LP_SPRITE					m_sprite;		// DirectX ��������Ʈ�� DirectX ������Ƽ�� Ÿ�԰� �ؽ��� �����ͷ� �����Ǿ�����
												// 2D �̹����� �׸��� ���� ����� DirectX API�� �Ϻ�
	LP_TEXTURE					m_texture;
	COLOR_ARGB					m_backColor;     

	// ������ ���� ����
	HRESULT						m_result;
	HWND						m_hwnd;
	bool						m_fullscreen;
	int							m_width;
	int							m_height;


	// ���� ���ο����� ����ϴ� D3D ���������̼� 
	// �Ű������� �ʱ�ȭ�Ѵ�.
	void initD3DApp();

public : 
	// ������
	CGraphics();

	// �Ҹ���
	virtual ~CGraphics();

	// ���� �Ҵ�� Direct3D�� Device3D�� �����Ѵ�.
	void ReleaseAll();

	// DirectX �׷����� �ʱ�ȭ�Ѵ�.
	void Initialize(HWND hwnd, int width, int height, bool fullscreen);

	// ȭ�鿡 ������ �ʴ� ����۸� ȭ�鿡 ǥ���Ѵ�.
	HRESULT ShowBackBuffer();

	// ���÷��� ����� ȣȯ���� Ȯ���ϴ� �Լ�
	bool IsAdapterCompatible();

	// ����̽� �ν�Ʈ�� Ȯ���Ѵ�.
	HRESULT GetDeviceState();

	// �׷��� ����̽��� �����Ѵ�.
	HRESULT Reset();

	LP_3DDEVICE Get3Ddevice() const { return m_device3d; }

	// �ؽ�ó�� �ε��ϴ� �޼���
	HRESULT LoadTexture(const char* fileName, COLOR_ARGB transColor, 
		UINT& width, UINT& height, LP_TEXTURE & texture);

	// ��������Ʈ�� �׸��� ���� �޼���
	void DrawSprite(const SPRITE_DATA& spriteData, COLOR_ARGB color);
	LP_SPRITE GetSprite() const { return m_sprite; }


	void SetBackColor(COLOR_ARGB c) { m_backColor = c; }

	// Quad�� �׸��� �Լ�
	bool DrawQuad(LP_VERTEXBUFFER vertexBuffer);
	// ���� ���۸� �����ϴ� �Լ�
	HRESULT CreateVertexBuffer(VertexC vertices[], UINT size, LP_VERTEXBUFFER& vertexBuffer);

	// �ؽ�ó�� �ý��� �޸𸮿��� �ҷ��´�.
	// �ý��� �޸𸮴� �⺻������ ���� �� �� �ִ�
	// �ȼ� �����Ϳ� �������� ������ �����ϴ�.
	// �̸� �̿��ؼ� TetureData ����ü�� ä���.
	HRESULT LoadTextureSystemMem(const char* fileName, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture);

	// ������ ���� �����ϰ� ������ �Լ�
	HRESULT BeginScene();
	HRESULT EndScene();

	// ��������Ʈ �׸��� ����
	void SpriteBegin() {
		m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	// ��������Ʈ �׸��� ��
	void SpriteEnd() {
		m_sprite->End();
	}
};

