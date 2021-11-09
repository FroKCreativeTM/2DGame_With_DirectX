#pragma once

#include "CGameEngine.h"

class SpaceWar : public CGameEngine {
private : 
	// �ʿ��� ������
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

