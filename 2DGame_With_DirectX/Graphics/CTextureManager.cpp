#include ".\CTextureManager.h"

CTextureManager::CTextureManager() {
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
	m_fileName = nullptr;
	m_graphics = nullptr;
	m_initialized = false;            // set true when successfully initialized
}

CTextureManager::~CTextureManager() {
	SAFE_RELEASE(m_texture);
}

bool CTextureManager::Initialize(CGraphics* g, const char* f) {
	try {
		m_graphics = g;
		m_fileName = f;

		m_result = m_graphics->LoadTexture(m_fileName, NSGraphics::TRANSCOLOR, 
			m_width, m_height, m_texture);

		if (FAILED(m_result)) {
			SAFE_RELEASE(m_texture);
			return false;
		}
	}
	catch (...) {
		return false;
	}

	m_initialized = true;
	return true;
}

// �׷��� ����̽��� �ν�Ʈ�� ������ ���
void CTextureManager::OnLostDevice() {
	if (!m_initialized) {
		return;
	}
	SAFE_RELEASE(m_texture);
}

// �׷��� ����̽��� ���µ� ���
void CTextureManager::OnResetDevice() {
	if (!m_initialized) {
		return;
	}
	m_graphics->LoadTexture(m_fileName, NSGraphics::TRANSCOLOR,
		m_width, m_height, m_texture);
}