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
	// 이 엔진을 상속하는 게임들은 전부 이 자원을 사용한다.
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

	// 게임 초기화
	virtual void Initialize(HWND hwnd);

	// WinMain에서 반복적으로 run함수를 호출한다.
	virtual void Run(HWND hwnd);

	// 그래픽 디바이스가 로스트 상태가 된 경우 호출한다.
	virtual void ReleaseAll();

	// 모든 표면을 재생성하고 모든 개체를 리셋한다.
	virtual void ResetAll();

	// 예약된 모든 메모리를 삭제한다.
	virtual void DeleteAll();

	// 게임 요소들을 렌더링한다.
	virtual void RenderGame();

	// 로스트 상태가 된 그래픽 디바이스를 처리한다.
	// if 디바이스가 유효한 상태가 아니라면
	//		if 디바이스가 로스트 상태이며 리셋이 불가능 한 경우라면
	//			디바이스를 복구할 때까지 기다린다.
	//		else if 디바이스가 리셋이 가능하다면
	//			디바이스 리셋
	//			if 리셋이 실패한다면
	//				반환한다.
	virtual void HandleLostGraphicsDevice();

	// 그래픽스 객체를 반환한다.
	CGraphics* GetGraphics() const { return m_pGraphics; }
	// Input 객체를 반환한다.
	CInput* GetInput() const { return m_pInput;  }
	CAudio* GetAudio() const { return m_pAudio; }

	// 게임 종료
	void ExitGame() {
		PostMessage(m_hwnd, WM_DESTROY, 0, 0);
	}

	/* ======================== 게임 관련 메서드 ======================== */
	virtual void Update() = 0;
	virtual void Ai() = 0;
	virtual void Collisions() = 0;
	virtual void Render() = 0;
};

