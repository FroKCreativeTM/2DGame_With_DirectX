#pragma once

#include "CGameEngine.h"
#include "Graphics/CTextureManager.h"
#include "Graphics/CImage.h"

#include "Entity/CShip.h"
#include "Entity/CPlanet.h"

class SpaceWar : public CGameEngine {
private : 
	// 필요한 변수들
	CTextureManager m_nebulaTexture;
	CTextureManager m_gameTexture;
	CPlanet m_planet;
	CImage m_nebula;
	CShip m_ship1;
	CShip m_ship2;
public : 
	// 생성자 및 소멸자
	SpaceWar();
	virtual ~SpaceWar();

	// 게임 초기화
	void Initialize(HWND hwnd);
	void Update();
	void Ai();
	void Collisions();
	void Render();

	void ReleaseAll();
	void ResetAll();
};

