#pragma once
#define WIN32_LEAN_AND_MEAN
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <Windows.h>
#include "Macro.h"
#include "Const.h"

/* ================== �ڷ��� ���� ================== */
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

/* ================== ��ũ�� ���� ================== */
#define COLOR_ARGB_DWORD
#define SETCOLOR_ARGB(a, r, g, b) \
	((COLOR_ARGB) ((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | \
		(((g) & 0xff) << 8) | ((b) & 0xff)))

class CGraphics
{
private : 
	// DirectX �����Ϳ� ���� ����
	LP_3D						m_direct3d;
	LP_3DDEVICE					m_device3d;
	D3DPRESENT_PARAMETERS		m_d3dpp;
	D3DDISPLAYMODE				m_pMode;

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
};

