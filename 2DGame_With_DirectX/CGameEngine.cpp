#include "CGameEngine.h"

CGameEngine::CGameEngine() {
	// Ű���� �Է��� �ʱ�ȭ �Ѵ�.
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
		// ��Ʈ�ѷ�
			m_pInput->CheckControllers();
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CGameEngine::Initialize(HWND hwnd) {
	m_hwnd = hwnd;

	// �׷��Ƚ� ��ü ����
	m_pGraphics = new CGraphics();

	// ���� ������ ������.
	m_pGraphics->Initialize(m_hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// Input ��ü �ʱ�ȭ
	m_pInput->Initialize(hwnd, false);

	// ������Ŵ��� �ʱ�ȭ
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

	// �����쿡�� �ý����ݷ� �����ϴ� ������ Ÿ�̸� ����� �õ��Ѵ�.
	if (QueryPerformanceFrequency(&m_timerFreq) == false) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"Error initializing high resolution timer"));
	}

	QueryPerformanceCounter(&m_timeStart);

	// dxText �۲� �ʱ�ȭ
	if (m_dxText.Initialize(m_pGraphics, NSGameEngine::POINT_SIZE, false, false, NSGameEngine::FONT) == false) {
		throw(CGameError(NSGameError::FATAL_ERROR,
			"Error initializing DirectX Font"));
	}
	m_dxText.SetFontColor(NSGameEngine::FONT_COLOR);

	// �ܼ� �ʱ�ȭ
	m_console = new CConsole();
	m_console->Initialize(m_pGraphics, m_pInput);
	m_console->Print("----Console----");

	m_initialized = true;
}

void CGameEngine::Run(HWND hwnd) {
	if (m_pGraphics == nullptr) {
		// �׷��� ��ü�� �ʱ�ȭ ���� �ʾҴٸ�
		return;
	}
	// ������ ���������� ���� ����� �ð��� ����ؼ� frameTime�� �����Ѵ�.
	QueryPerformanceCounter(&m_timeEnd);
	m_frameTime = (float)(m_timeEnd.QuadPart - m_timeStart.QuadPart) / (float)m_timerFreq.QuadPart;

	// ������ ���� �ڵ� winmm.lib�� �ִ´�.
	// ����� �ð��� ���ϴ� ������ �ӵ��� �������� �ʴ´ٸ�
	if (m_frameTime < MIN_FRAME_TIME) {
		m_sleepTime = (DWORD)((MIN_FRAME_TIME - m_frameTime) * 1000);
		timeBeginPeriod(1);	// ������ Ÿ�̸Ӹ� ���� 1ms �ػ� ��û
		Sleep(m_sleepTime);
		timeEndPeriod(1);	// 1ms Ÿ�̸� �ػ� ����
		return;
	}

	if (m_frameTime > 0.0) {
		m_fps = (m_fps * 0.99f) + (0.01f / m_frameTime);
	}
	if (m_frameTime > MAX_FRAME_TIME) { // ������ �ӵ��� �ſ� �����ٸ�
		m_frameTime = MAX_FRAME_TIME;	// �ִ� frameTime���� �����Ѵ�.
	}

	m_timeStart = m_timeEnd;	// �ð� ����
	// m_pInput->ReadContollers(); // ��Ʈ�ѷ� ���¸� �д´�. (���� ��Ʈ�ѷ� ���� ����)
	
	// update, ai, collisions�� ���� �����Լ���.
	// �� �Լ����� �� ������ ��ӹ��� ���ӵ��� �����ؾ��ϴ� �Լ��̴�.
	if (!m_paused) {
		Update();
		Ai();
		Collisions();
		// m_pInput->VibrateContollers(m_frameTime);
	}

	// ���� ��ҵ��� �׷��ش�.
	RenderGame();

	// ������� �����Ѵ�.
	m_pAudio->Run();

	// �ܼ� Ű�� Ȯ���Ѵ�.
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
		// Render �޼���� ��۹޴� Ŭ�������� �ݵ�� �����ؾ� �ϴ� ���� ���� �Լ��̴�.
		Render();

		// ��������Ʈ �׸��� ����
		m_pGraphics->SpriteBegin();
		if (m_isFPSOn) {
			// FPS�� Cstring���� ��ȯ
			_snprintf_s(buffer, BUF_SIZE, "fps %d", (int)m_fps);
			m_dxText.Print(buffer, 5, 5);
		}
		m_pGraphics->SpriteEnd();
		m_console->Draw();

		m_pGraphics->EndScene();
	}
	HandleLostGraphicsDevice();

	// ȭ�鿡 �� ���۸� ǥ���Ѵ�.
	m_pGraphics->ShowBackBuffer();
}

// if ����̽��� ��ȿ�� ���°� �ƴ϶��
//		if ����̽��� �ν�Ʈ �����̸� ������ �Ұ��� �� �����
//			����̽��� ������ ������ ��ٸ���.
//		else if ����̽��� ������ �����ϴٸ�
//			����̽� ����
//			if ������ �����Ѵٸ�
//				��ȯ�Ѵ�.
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

// �ܼ� ����� ó���Ѵ�.
// �� �ܼ� ����� �߰��Ǹ� ��ӹ��� Ŭ�������� �� �Լ��� �������̵��Ѵ�.
void CGameEngine::ConsoleCommand() {
	m_command = m_console->GetCommand();

	if (m_command == "") { // ����� �ƴ϶��
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
