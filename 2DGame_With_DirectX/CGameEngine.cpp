#include "CGameEngine.h"

CGameEngine::CGameEngine()
{
	// 키보드 입력을 초기화 한다.
	m_pInput = new CInput;
	m_paused = false;		// 
	m_pGraphics = nullptr;	// 
	m_initialized = false;	// 
}

CGameEngine::~CGameEngine()
{

}
