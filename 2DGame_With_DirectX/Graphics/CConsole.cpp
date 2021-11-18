#include "CConsole.h"

CConsole::CConsole() {
    m_initialized = false;
    m_pGraphics = nullptr;
    m_visible = false;
    m_fontColor = NSConsole::FONT_COLOR;
    m_backColor = NSConsole::BACK_COLOR;
    m_x = NSConsole::X;
    m_y = NSConsole::Y;
    m_textRect.bottom = NSConsole::Y + NSConsole::HEIGHT - NSConsole::MARGIN;
    m_textRect.left = NSConsole::X + NSConsole::MARGIN;
    m_textRect.right = NSConsole::X + NSConsole::WIDTH - NSConsole::MARGIN;
    m_textRect.top = NSConsole::Y + NSConsole::MARGIN;
    m_vertexBuffer = nullptr;
    m_row = 0;
    m_scrollAmount = 0;
}

CConsole::~CConsole() {
}

bool CConsole::Initialize(CGraphics* pGraphics, CInput* pInput) {
    try {
        m_pGraphics = pGraphics;
        m_pInput = pInput;

        // 왼쪽 상단, 오른쪽 상단, 오른쪽 하단, 왼쪽 하단 순
        // 순서를 지키지 않으면 쿼드는 보이지 않을 수 있다.
        // 다렉은 기본적으로 삼각형을 그릴 때 한 쪽으로만 볼 수 있다.
        // 삼각형을 뒤에서 본다면 꿰뚫어 볼 수 있을 것인데, 이를 백페이스 컬링이라 부른다.
        // 백페이스 컬링은 3D의 표면의 렌더링을 가속화하는 데 사용되는 일반적인 기술이다.
        // 만약 반시계 방향으로 배치를 한다면 뒤쪽에서 삼각형을 보기 때문에 보이지 않을 것이다.
        m_vertices[0].x = m_x;
        m_vertices[0].y = m_y;
        m_vertices[0].z = 0.0f;
        m_vertices[0].rhw = 1.0f;
        m_vertices[0].color = m_backColor;

        m_vertices[1].x = m_x + NSConsole::WIDTH;
        m_vertices[1].y = m_y;
        m_vertices[1].z = 0.0f;
        m_vertices[1].rhw = 1.0f;
        m_vertices[1].color = m_backColor;

        m_vertices[2].x = m_x + NSConsole::WIDTH;
        m_vertices[2].y = m_y + NSConsole::HEIGHT;
        m_vertices[2].z = 0.0f;
        m_vertices[2].rhw = 1.0f;
        m_vertices[2].color = m_backColor;

        m_vertices[3].x = m_x;
        m_vertices[3].y = m_y + NSConsole::HEIGHT;
        m_vertices[3].z = 0.0f;
        m_vertices[3].rhw = 1.0f;
        m_vertices[3].color = m_backColor;

        m_pGraphics->CreateVertexBuffer(m_vertices, sizeof(m_vertices), m_vertexBuffer);

        // 글꼴 초기화
        if (m_dxFont.Initialize(m_pGraphics, NSConsole::FONT_HEIGHT, false, false, NSConsole::FONT) == false) {
            return false;
        }
        m_dxFont.SetFontColor(m_fontColor);
    }
    catch (...) {

    }
    m_initialized = true;
    return true;
}

const void CConsole::Draw() {
    if (!m_visible || m_pGraphics == nullptr || !m_initialized) {
        return;
    }
    m_pGraphics->DrawQuad(m_vertexBuffer);
    if (m_text.size() == 0) {
        return;
    }

    m_pGraphics->SpriteBegin();

    m_textRect.left = 0;
    m_textRect.top = 0;
    // "|"는 전체 높이 캐릭터로 사용된다.
    m_dxFont.Print("|", m_textRect, DT_CALCRECT);
    int rowHeight = m_textRect.bottom + 2; // 한 행의 높이(+2는 행 간격이다.)
    if (rowHeight <= 0) { // 절대 true가 되면 안 되는 부분(상식적으로 말이 안 된다.)
        rowHeight = 20;   // 가능한 결과로 강제 시킨다.
    }

    // 콘솔에 맞는 행의 수
    m_row = (NSConsole::HEIGHT - 2 * NSConsole::MARGIN) / rowHeight;
    m_row -= 2; // 하단 프롬프트를 위한 공간

    if (m_row <= 0) { // 절대 true가 되면 안 되는 부분
        m_row = 5;
    }

    // 콘솔 텍스트를 행 단위로 표시
    // 텍스트 한 행을 표시할 사각형을 설정한다.
    m_textRect.left = (long)(m_x + NSConsole::MARGIN);
    m_textRect.right = (long)(m_textRect.right + NSConsole::WIDTH - NSConsole::MARGIN);

    // -2 * rowHeight는 입력 프롬프트를 위한 공간이다.
    m_textRect.bottom = (long)(m_y + NSConsole::HEIGHT - 2 * NSConsole::MARGIN - 2 * rowHeight);

    for (int r = m_scrollAmount; r < m_row + m_scrollAmount && r < (int)(m_text.size()); r++) {
        // 해당 행에 대해 텍스트를 표시하는 사각형의 top을 설정한다.
        m_textRect.top = m_textRect.bottom - rowHeight;
        // 텍스트 한 행을 표시한다.
        m_dxFont.Print(m_text[r], m_textRect, DT_LEFT);
        // 텍스트를 표시하는 사각형의 bottom을 조정한다.
        m_textRect.bottom -= rowHeight;
    }
    
    // 콘솔 명령 프롬프트 표시
    // 명령 프롬프트와 현재 명령을 표시한다.
    // 프롬프트 텍스트를 표시하는 사각형을 설정한다.
    m_textRect.bottom = (long)(m_y + NSConsole::HEIGHT - NSConsole::MARGIN);
    m_textRect.top = m_textRect.bottom - rowHeight;
    std::string prompt = ">";
    prompt += m_pInput->GetTextIn();
    m_dxFont.Print(prompt, m_textRect, DT_LEFT);

    m_pGraphics->SpriteEnd();
}

void CConsole::ShowHide() {
    if (!m_initialized) {
        return;
    }
    m_visible = !m_visible;
    m_pInput->Clear(NSInput::KEYS_PRESSED | NSInput::TEXT_IN); // 이전의 입력을 지운다.
}

void CConsole::Print(const std::string& str) {
    if (!m_initialized) {
        return;
    }
    m_text.push_front(str); // str을 데큐에 추가한다.
    if (m_text.size() > NSConsole::MAX_LINES) { // 만약 큐가 다 찼다면
        m_text.pop_back();  // 가장 오래된 줄을 삭제한다.
    }
}

std::string CConsole::GetCommand() {
    // 콘솔이 초기화가 되지 않았거나 안 보이는 경우
    if (!m_initialized || !m_visible) {
        return "";
    }

    // 콘솔키를 확인
    if (m_pInput->WasKeyPressed(CONSOLE_KEY)) {
        Hide();
    }
    // ESC키를 확인
    if (m_pInput->WasKeyPressed(ESC_KEY)) {
        return "";
    }
    // 스크롤 확인
    if (m_pInput->WasKeyPressed(VK_UP)) {
        m_scrollAmount++;
    }
    else if (m_pInput->WasKeyPressed(VK_DOWN)) {
        m_scrollAmount--;
    }
    else if (m_pInput->WasKeyPressed(VK_PRIOR)) {
        m_scrollAmount += m_row;
    }
    else if (m_pInput->WasKeyPressed(VK_NEXT)) {
        m_scrollAmount -= m_row;
    }

    if (m_scrollAmount < 0) {
        m_scrollAmount = 0;
    }
    if (m_scrollAmount > NSConsole::MAX_LINES - 1) {
        m_scrollAmount = NSConsole::MAX_LINES - 1;
    }
    if (m_scrollAmount > (int)(m_text.size()) - 1) {
        m_scrollAmount = (int)(m_text.size()) - 1;
    }

    m_command = m_pInput->GetTextIn();
    // 게임을 통해 키를 전달하면 안 된다.
    m_pInput->Clear(NSInput::KEYS_DOWN | NSInput::KEYS_PRESSED | NSInput::MOUSE);

    if (m_command.length() == 0) {
        return "";
    }

    if (m_command.at(m_command.length() - 1) != '\r') { // 엔터를 누르지 않았다면
        return "";
    }
    m_command.erase(m_command.length() - 1); // \r는 지운다.
    m_pInput->ClearTextIn(); // 입력줄은 지운다.

    return m_command; // 명령 반환
}

void CConsole::OnLostDevice() {
    if (!m_initialized)
        return;
    m_dxFont.OnLostDevice();
    SAFE_RELEASE(m_vertexBuffer);
}

void CConsole::OnResetDevice() {
    if (!m_initialized)
        return;
    m_pGraphics->CreateVertexBuffer(m_vertices, sizeof m_vertices, m_vertexBuffer);
    m_dxFont.OnResetDevice();
}
