#pragma once


#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>
#include "../CGraphics.h"

// ���� �ٸ� ���ø����̼ǿ��� �׷��� ����̽��� ����ؼ� 
// ���� ���忡 ����� �� ���� ��� �ν�Ʈ ���°� �ȴ�
// �� �� �ν�Ʈ ���°� �� ���¿��� ����̽��� ã�� ������ ����
// �ؽ�ó �����͸� �����ϰ� �ٽ� �ҷ��� �Ѵ�.
// �� Ŭ������ �̿��ϸ� ����̽� �ν�Ʈ�� �ٷ� �� �ִ�.
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
	// �ؽ�ó�� �� ��ȯ
	UINT GetWidth() const { return m_width; }
	UINT GetHeight() const { return m_height; }
};

