#pragma once

#include <XAudio2.h>
#include <xact3.h>
#include "../Const.h"

// �� Ŭ������ �̿��ؼ� ���� ������ ���带 ����Ѵ�.
class CAudio {
private : 
	// XACT ���� ����
	IXACT3Engine*		m_xactEngine;	// ���� ����
	IXACT3WaveBank*		m_wavebank;		// ���̺� ��ũ
	IXACT3SoundBank*	m_soundBank;	// ���� ��ũ
	XACTINDEX			m_cueIdx;		// ���� �ε���
	void*				m_mapWaveBank;	// ������ �����ϱ� ���� UpmapViewOfFile() ȣ��
	void*				m_soundBankData;
	bool				m_coinitialized;

public : 
	CAudio();
	virtual ~CAudio();

	// ����� �ʱ�ȭ
	HRESULT Initialize();

	// ���� ���� �۾��� ����
	void Run();

	// ���� ��ũ�� ť���� ������ ���� ���
	// ť�� �������� �ʴ��� ���� �߻� X (���� ������� ���尡 ����.)
	void PlayCue(const char* cue);

	// ���� ��ũ�� ť���� ������ ���带 �����.
	// ť�� �������� �ʴ��� ������ �߻���Ű�� �ʴ´�.
	void StopCue(const char* cue);

	// ī�װ��� �����ϸ� �׿� �´� ī�װ� ������ �ߴ��Ѵ�.
	void PauseCategory(const char category[]);

	// ī�װ��� �����ϸ� �׿� �´� ī�װ� ������ �ٽ� �����Ѵ�.
	void ResumeCategory(const char category[]);
};

