#include "CGameEngine.h"

CGameEngine::CGameEngine() {
	// 키보드 입력을 초기화 한다.
	m_pInput = new CInput;
	m_paused = false;		// 
	m_pGraphics = nullptr;	// 
	m_initialized = false;	// 
	m_isFPSOn = false;
	m_console = nullptr;
}

CGameEngine::~CGameEngine() {
	DeleteAll();
	ShowCursor(true);
}

LRESULT CGameEngine::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (m_initialized) {
		switch (msg)
		{
		case WM_DESTROY : 
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN : case WM_SYSKEYDOWN :
			m_pInput->KeyDown(wParam);
			return 0;
		case WM_KEYUP : case WM_SYSKEYUP :
			m_pInput->KeyUp(wParam);
			return 0;
		case WM_CHAR : 
			m_pInput->KeyIn(wParam);
			return 0;
		case WM_MOUSEMOVE :
			m_pInput->MouseIn(lParam);
			return 0;
		case WM_INPUT :
			m_pInput->MouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:
			m_pInput->SetMouseLButton(true);
			m_pInput->MouseIn(lParam);
			return 0;
		case WM_LBUTTONUP:
			m_pInput->SetMouseLButton(false);
			m_pInput->MouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN:
			m_pInput->SetMouseMButton(true);
			m_pInput->MouseIn(lParam);
			return 0;
		case WM_MBUTTONUP:
			m_pInput->SetMouseMButton(false);
			m_pInput->MouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN :
			m_pInput->SetMouseRButton(true);
			m_pInput->MouseIn(lParam);
			return 0;
		case WM_RBUTTONUP :
			m_pInput->SetMouseRButton(false);
			m_pInput->MouseIn(lParam);
			return 0;
		case WM_XBUTTONDOWN : case WM_XBUTTONUP :
			m_pInput->SetMouseXButton(wParam);
			m_pInput->MouseIn(lParam);
			return 0;
		case WM_DEVICECHANGE : 
		// 컨트롤러
			m_pInput->CheckControllers();
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CGameEngine::Initialize(HWND hwnd) {
	m_hwnd = hwnd;

	// 그래픽스 객체 생성
	m_pGraphics = new CGraphics();

	// 게임 에러를 던진다.
	m_pGraphics->Initialize(m_hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// Input 객체 초기화
	m_pInput->Initialize(hwnd, false);

	// 오디오매니저 초기화
	m_pAudio = new CAudio();
	if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0') {
		if (FAILED(m_hr = m_pAudio->Initialize())) {
			if (m_hr == __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
				throw(CGameError(NSGameError::FATAL_ERROR,
					"Error initializing Audio Engine Manager \
					because media file not found"));
			}
			else {
				throw(CGameError(NSGameError::FATAL_ERROR,
					"Error initializing Audio Engine Manager"));
			}
		}
	}

	// 윈도우에서 시스템콜로 제공하는 고성능의 타이머 사용을 시도한다.
	if (QueryPerformanceFrequency(&m_timerFreq) == false) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"Error initializing high resolution timer"));
	}

	QueryPerformanceCounter(&m_timeStart);

	// dxText 글꼴 초기화
	if (m_dxText.Initialize(m_pGraphics, NSGameEngine::POINT_SIZE, false, false, NSGameEngine::FONT) == false) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"Error initializing DirectX Font"));
	}
	m_dxText.SetFontColor(NSGameEngine::FONT_COLOR);

	// 콘솔 초기화
	m_console = new CConsole();
	m_console->Initialize(m_pGraphics, m_pInput);
	m_console->Print("----Console----");

	m_initialized = true;
}

void CGameEngine::Run(HWND hwnd) {
	if (m_pGraphics == nullptr) {
		// 그래픽 객체가 초기화 되지 않았다면
		return;
	}
	// 마지막 프레임으로 부터 경과된 시간을 계산해서 frameTime에 저장한다.
	QueryPerformanceCounter(&m_timeEnd);
	m_frameTime = (float)(m_timeEnd.QuadPart - m_timeStart.QuadPart) / (float)m_timerFreq.QuadPart;

	// 절전을 위한 코드 winmm.lib를 넣는다.
	// 경과된 시간이 원하는 프레임 속도에 충족하지 않는다면
	if (m_frameTime < MIN_FRAME_TIME) {
		m_sleepTime = (DWORD)((MIN_FRAME_TIME - m_frameTime) * 1000);
		timeBeginPeriod(1);	// 윈도우 타이머를 위한 1ms 해상도 요청
		Sleep(m_sleepTime);
		timeEndPeriod(1);	// 1ms 타이머 해상도 종료
		return;
	}

	if (m_frameTime > 0.0) {
		m_fps = (m_fps * 0.99f) + (0.01f / m_frameTime);
	}
	if (m_frameTime > MAX_FRAME_TIME) { // 프레임 속도가 매우 느리다면
		m_frameTime = MAX_FRAME_TIME;	// 최대 frameTime으로 제한한다.
	}

	m_timeStart = m_timeEnd;	// 시간 저장
	// m_pInput->ReadContollers(); // 컨트롤러 상태를 읽는다. (추후 컨트롤러 지원 예정)
	
	// update, ai, collisions는 순수 가상함수다.
	// 이 함수들은 이 엔진을 상속받은 게임들이 구현해야하는 함수이다.
	if (!m_paused) {
		Update();
		Ai();
		Collisions();
		// m_pInput->VibrateContollers(m_frameTime);
	}

	// 게임 요소들을 그려준다.
	RenderGame();

	// 오디오를 실행한다.
	m_pAudio->Run();

	// 콘솔 키를 확인한다.
	if (m_pInput->WasKeyPressed(CONSOLE_KEY)) {
		m_console->ShowHide();
		m_paused = m_console->GetVisible();
	}
	ConsoleCommand();

	m_pInput->Clear(NSInput::KEYS_PRESSED);
}

void CGameEngine::ReleaseAll()
{
	SAFE_ON_LOST_DEVICE(m_console);
	m_dxText.OnLostDevice();
	return;
}

void CGameEngine::ResetAll()
{
	m_dxText.OnResetDevice();
	SAFE_ON_RESET_DEVICE(m_console);
	return;
}

void CGameEngine::DeleteAll()
{
	ReleaseAll();               
	SAFE_DELETE(m_pGraphics);
	SAFE_DELETE(m_pInput);
	SAFE_DELETE(m_pAudio);
	SAFE_DELETE(m_console);
	m_initialized = false;
}

void CGameEngine::RenderGame() {
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];

	if (SUCCEEDED(m_pGraphics->BeginScene())) {
		// Render 메서드는 상송받는 클래스에서 반드시 구현해야 하는 순수 가상 함수이다.
		Render();

		// 스프라이트 그리기 시작
		m_pGraphics->SpriteBegin();
		if (m_isFPSOn) {
			// FPS를 Cstring으로 변환
			_snprintf_s(buffer, BUF_SIZE, "fps %d", (int)m_fps);
			m_dxText.Print(buffer, 5, 5);
		}
		m_pGraphics->SpriteEnd();
		m_console->Draw();

		m_pGraphics->EndScene();
	}
	HandleLostGraphicsDevice();

	// 화면에 백 버퍼를 표시한다.
	m_pGraphics->ShowBackBuffer();
}

// if 디바이스가 유효한 상태가 아니라면
//		if 디바이스가 로스트 상태이며 리셋이 불가능 한 경우라면
//			디바이스를 복구할 때까지 기다린다.
//		else if 디바이스가 리셋이 가능하다면
//			디바이스 리셋
//			if 리셋이 실패한다면
//				반환한다.
void CGameEngine::HandleLostGraphicsDevice() {
	m_hr = m_pGraphics->GetDeviceState();

	if (FAILED(m_hr)) {
		if (m_hr == D3DERR_DEVICELOST) {
			Sleep(100);
			return;
		}
		else if (m_hr == D3DERR_DEVICENOTRESET) {
			ReleaseAll();

			m_hr = m_pGraphics->Reset();

			if (FAILED(m_hr)) {
				return;
			}
			ResetAll();
		}
		else {
			return;
		}
	}
}

// 콘솔 명령을 처리한다.
// 새 콘솔 명령이 추가되면 상속받은 클래스에서 이 함수를 오버라이딩한다.
void CGameEngine::ConsoleCommand() {
	m_command = m_console->GetCommand();

	if (m_command == "") { // 명령이 아니라면
		return;
	}
	if (m_command == "help") {
		m_console->Print("Console command : ");
		m_console->Print("fps - toggle display of frames per second");
		return;
	}
	if (m_command == "fps") {
		m_isFPSOn = !m_isFPSOn;
		if (m_isFPSOn) {
			m_console->Print("fps on");
		}
		else {
			m_console->Print("fps off");
		}
	}

}
