#include "CAudio.h"

CAudio::CAudio()
{
	m_xactEngine = nullptr;
	m_wavebank = nullptr;
	m_soundBank = nullptr;
	m_cueIdx = 0;
	m_mapWaveBank = nullptr;         // Call UnmapViewOfFile() to release file
	m_soundBankData = nullptr;

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		m_coinitialized = true;
	}
	else {
		m_coinitialized = false;
	}
}

CAudio::~CAudio() {
	if (m_xactEngine) {
		m_xactEngine->ShutDown();
		m_xactEngine->Release();
	}

	if (m_soundBankData) {
		delete[] m_soundBankData;
	}
	m_soundBankData = nullptr;

	if (m_mapWaveBank) {
		UnmapViewOfFile(m_mapWaveBank);
	}
	m_mapWaveBank = nullptr;

	if (m_coinitialized) {
		CoUninitialize();
	}
}

HRESULT CAudio::Initialize() {
	HRESULT result = XACT3CreateEngine(0, &m_xactEngine);
	if (FAILED(result) || m_xactEngine == nullptr) {
		return E_FAIL;
	}

	XACT_RUNTIME_PARAMETERS xactParams = { 0 };
	xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	result = m_xactEngine->Initialize(&xactParams);

	if (FAILED(result)) {
		return result;
	}

	/* ========= 웨이브 뱅크 등록 ========= */
	result = E_FAIL;
	HANDLE hFile= CreateFile(WAVE_BANK, GENERIC_READ, FILE_SHARE_READ,
		nullptr, OPEN_EXISTING, 0, nullptr);

	// 파일 읽기에 성공한다면
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD fileSize = GetFileSize(hFile, nullptr);
		if (fileSize != -1) {
			HANDLE hMapFile = CreateFileMapping(hFile, nullptr, PAGE_READONLY,
				0, fileSize, nullptr);

			if (hMapFile) {
				LPVOID mapWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

				if (mapWaveBank) {
					result = m_xactEngine->CreateInMemoryWaveBank(
						mapWaveBank, fileSize, 0, 0, &m_wavebank
					);
				}
				// 사용하던 핸들을 해제한다.
				CloseHandle(hMapFile);
			}
		}
		CloseHandle(hFile);
	}

	if (FAILED(result)) {
		return __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}

	/* ========= 사운드 뱅크 등록 ========= */
	result = E_FAIL;
	hFile = CreateFile(SOUND_BANK, GENERIC_READ, FILE_SHARE_READ,
		nullptr, OPEN_EXISTING, 0, nullptr);
	DWORD bytesRead;

	// 파일 읽기에 성공한다면
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD fileSize = GetFileSize(hFile, nullptr);
		if (fileSize != -1) {
			m_soundBankData = new BYTE[fileSize];
			if (m_soundBankData) {
				if (ReadFile(hFile, m_soundBankData, fileSize, &bytesRead, nullptr) != 0) {
					result = m_xactEngine->CreateSoundBank(
						m_soundBankData,
						fileSize, 0, 0,
						&m_soundBank);
				}
			}
		}
		CloseHandle(hFile);
	}

	if (FAILED(result)) {
		return __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}
	return S_OK;
}

void CAudio::Run() {
	if (m_xactEngine == nullptr) {
		return;
	}
	m_xactEngine->DoWork();
}

void CAudio::PlayCue(const char* cue) {
	if (m_soundBank == nullptr) {
		return;
	}
	m_cueIdx = m_soundBank->GetCueIndex(cue);
	m_soundBank->Play(m_cueIdx, 0, 0, nullptr);
}

void CAudio::StopCue(const char* cue) {
	if (m_soundBank == nullptr) {
		return;
	}
	m_cueIdx = m_soundBank->GetCueIndex(cue);
	m_soundBank->Stop(m_cueIdx, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
