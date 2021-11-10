#pragma once

#include "CGameEngine.h"
#include "Graphics/CTextureManager.h"
#include "Graphics/CImage.h"

class SpaceWar : public CGameEngine {
private : 
	// �ʿ��� ������
	CTextureManager m_nebulaTexture;
	CTextureManager m_planetTexture;
	CTextureManager m_shipTexture;
	CImage m_planet;
	CImage m_nebula;
	CImage m_ship;
public : 
	// ������ �� �Ҹ���
	SpaceWar();
	virtual ~SpaceWar();

	// ���� �ʱ�ȭ
	void Initialize(HWND hwnd);
	void Update();
	void Ai();
	void Collisions();
	void Render();

	void ReleaseAll();
	void ResetAll();
};

