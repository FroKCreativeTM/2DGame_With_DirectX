#pragma once

#include "CGameEngine.h"

class SpaceWar : public CGameEngine {
private : 
	// 필요한 변수들
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

