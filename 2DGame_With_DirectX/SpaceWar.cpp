#include "SpaceWar.h"

SpaceWar::SpaceWar() {
}

SpaceWar::~SpaceWar() {
	ReleaseAll();
}

void SpaceWar::Initialize(HWND hwnd) {
	CGameEngine::Initialize(hwnd);
	return;
}

void SpaceWar::Update() {
}

void SpaceWar::Ai() {
}

void SpaceWar::Collisions() {
}

void SpaceWar::Render() {
}

void SpaceWar::ReleaseAll() {
	CGameEngine::ReleaseAll();
	return;
}

void SpaceWar::ResetAll() {
	CGameEngine::ResetAll();
	return;
}
