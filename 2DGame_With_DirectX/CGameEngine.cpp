#include "CGameEngine.h"

CGameEngine::CGameEngine()
{
	// Ű���� �Է��� �ʱ�ȭ �Ѵ�.
	m_pInput = new CInput;
	m_paused = false;		// 
	m_pGraphics = nullptr;	// 
	m_initialized = false;	// 
}

CGameEngine::~CGameEngine()
{

}
