#pragma once

#define WINDOW_LEAN_AND_MEAN
#include <Windows.h>
#include <mmsystem.h>
#include "CGameError.h"
#include "CGraphics.h"
#include "CInput.h"
#include "Const.h"
#include "Audio/CAudio.h"

class CGameEngine
{
protected : 
	// �� ������ ����ϴ� ���ӵ��� ���� �� �ڿ��� ����Ѵ�.
	CGraphics*				m_pGraphics;
	CInput*					m_pInput;
	CAudio*					m_pAudio;
	HWND					m_hwnd;
	HRESULT					m_hr;
	LARGE_INTEGER			m_timeStart;
	LARGE_INTEGER			m_timeEnd;
	LARGE_INTEGER			m_timerFreq;
	float					m_frameTime;
	float					m_fps;
	DWORD					m_sleepTime;
	bool					m_paused;
	bool					m_initialized;

public : 
	CGameEngine();
	virtual ~CGameEngine();

	LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ���� �ʱ�ȭ
	virtual void Initialize(HWND hwnd);

	// WinMain���� �ݺ������� run�Լ��� ȣ���Ѵ�.
	virtual void Run(HWND hwnd);

	// �׷��� ����̽��� �ν�Ʈ ���°� �� ��� ȣ���Ѵ�.
	virtual void ReleaseAll();

	// ��� ǥ���� ������ϰ� ��� ��ü�� �����Ѵ�.
	virtual void ResetAll();

	// ����� ��� �޸𸮸� �����Ѵ�.
	virtual void DeleteAll();

	// ���� ��ҵ��� �������Ѵ�.
	virtual void RenderGame();

	// �ν�Ʈ ���°� �� �׷��� ����̽��� ó���Ѵ�.
	// if ����̽��� ��ȿ�� ���°� �ƴ϶��
	//		if ����̽��� �ν�Ʈ �����̸� ������ �Ұ��� �� �����
	//			����̽��� ������ ������ ��ٸ���.
	//		else if ����̽��� ������ �����ϴٸ�
	//			����̽� ����
	//			if ������ �����Ѵٸ�
	//				��ȯ�Ѵ�.
	virtual void HandleLostGraphicsDevice();

	// �׷��Ƚ� ��ü�� ��ȯ�Ѵ�.
	CGraphics* GetGraphics() const { return m_pGraphics; }
	// Input ��ü�� ��ȯ�Ѵ�.
	CInput* GetInput() const { return m_pInput;  }
	CAudio* GetAudio() const { return m_pAudio; }

	// ���� ����
	void ExitGame() {
		PostMessage(m_hwnd, WM_DESTROY, 0, 0);
	}

	/* ======================== ���� ���� �޼��� ======================== */
	virtual void Update() = 0;
	virtual void Ai() = 0;
	virtual void Collisions() = 0;
	virtual void Render() = 0;
};

