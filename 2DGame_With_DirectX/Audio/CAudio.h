#pragma once

#include <XAudio2.h>
#include <xact3.h>
#include "../Const.h"

// 이 클래스를 이용해서 게임 전반의 사운드를 담당한다.
class CAudio {
private : 
	// XACT 사운드 엔진
	IXACT3Engine*		m_xactEngine;	// 사운드 엔진
	IXACT3WaveBank*		m_wavebank;		// 웨이브 뱅크
	IXACT3SoundBank*	m_soundBank;	// 사운드 뱅크
	XACTINDEX			m_cueIdx;		// 사운드 인덱스
	void*				m_mapWaveBank;	// 파일을 해제하기 위해 UpmapViewOfFile() 호출
	void*				m_soundBankData;
	bool				m_coinitialized;

public : 
	CAudio();
	virtual ~CAudio();

	// 오디오 초기화
	HRESULT Initialize();

	// 사운드 엔진 작업을 수행
	void Run();

	// 사운드 뱅크의 큐에서 지정한 사운드 재생
	// 큐가 존재하지 않더라도 에러 발생 X (단지 재생중인 사운드가 없다.)
	void PlayCue(const char* cue);

	// 사운드 뱅크의 큐에서 지정한 사운드를 멈춘다.
	// 큐가 존재하지 않더라도 에러를 발생시키지 않는다.
	void StopCue(const char* cue);

	// 카테고리를 전달하면 그에 맞는 카테고리 음악을 중단한다.
	void PauseCategory(const char category[]);

	// 카테고리를 전달하면 그에 맞는 카테고리 음악을 다시 실행한다.
	void ResumeCategory(const char category[]);
};

