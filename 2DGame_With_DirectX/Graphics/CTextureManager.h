#pragma once


#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>
#include "../CGraphics.h"

// 만약 다른 어플리케이션에서 그래픽 디바이스를 사용해서 
// 지금 당장에 사용할 수 없는 경우 로스트 상태가 된다
// 이 때 로스트 상태가 된 상태에서 디바이스를 찾는 과정과 같이
// 텍스처 데이터를 해제하고 다시 불러야 한다.
// 이 클래스를 이용하면 디바이스 로스트를 다룰 수 있다.
class CTextureManager {
private : 
	CGraphics* m_graphics;
	const char* m_fileName;

	HRESULT m_result;
	bool m_initialized;

	LP_TEXTURE					m_texture;
	UINT m_width;
	UINT m_height;

public : 
	CTextureManager();
	~CTextureManager();

	bool Initialize(CGraphics* g, const char* f);
	void OnLostDevice();
	void OnResetDevice();

	LP_TEXTURE GetTexture() const { return m_texture; }
	// 텍스처의 폭 반환
	UINT GetWidth() const { return m_width; }
	UINT GetHeight() const { return m_height; }
};

