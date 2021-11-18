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

        // ���� ���, ������ ���, ������ �ϴ�, ���� �ϴ� ��
        // ������ ��Ű�� ������ ����� ������ ���� �� �ִ�.
        // �ٷ��� �⺻������ �ﰢ���� �׸� �� �� �����θ� �� �� �ִ�.
        // �ﰢ���� �ڿ��� ���ٸ� ��վ� �� �� ���� ���ε�, �̸� �����̽� �ø��̶� �θ���.
        // �����̽� �ø��� 3D�� ǥ���� �������� ����ȭ�ϴ� �� ���Ǵ� �Ϲ����� ����̴�.
        // ���� �ݽð� �������� ��ġ�� �Ѵٸ� ���ʿ��� �ﰢ���� ���� ������ ������ ���� ���̴�.
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

        // �۲� �ʱ�ȭ
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
    // "|"�� ��ü ���� ĳ���ͷ� ���ȴ�.
    m_dxFont.Print("|", m_textRect, DT_CALCRECT);
    int rowHeight = m_textRect.bottom + 2; // �� ���� ����(+2�� �� �����̴�.)
    if (rowHeight <= 0) { // ���� true�� �Ǹ� �� �Ǵ� �κ�(��������� ���� �� �ȴ�.)
        rowHeight = 20;   // ������ ����� ���� ��Ų��.
    }

    // �ֿܼ� �´� ���� ��
    m_row = (NSConsole::HEIGHT - 2 * NSConsole::MARGIN) / rowHeight;
    m_row -= 2; // �ϴ� ������Ʈ�� ���� ����

    if (m_row <= 0) { // ���� true�� �Ǹ� �� �Ǵ� �κ�
        m_row = 5;
    }

    // �ܼ� �ؽ�Ʈ�� �� ������ ǥ��
    // �ؽ�Ʈ �� ���� ǥ���� �簢���� �����Ѵ�.
    m_textRect.left = (long)(m_x + NSConsole::MARGIN);
    m_textRect.right = (long)(m_textRect.right + NSConsole::WIDTH - NSConsole::MARGIN);

    // -2 * rowHeight�� �Է� ������Ʈ�� ���� �����̴�.
    m_textRect.bottom = (long)(m_y + NSConsole::HEIGHT - 2 * NSConsole::MARGIN - 2 * rowHeight);

    for (int r = m_scrollAmount; r < m_row + m_scrollAmount && r < (int)(m_text.size()); r++) {
        // �ش� �࿡ ���� �ؽ�Ʈ�� ǥ���ϴ� �簢���� top�� �����Ѵ�.
        m_textRect.top = m_textRect.bottom - rowHeight;
        // �ؽ�Ʈ �� ���� ǥ���Ѵ�.
        m_dxFont.Print(m_text[r], m_textRect, DT_LEFT);
        // �ؽ�Ʈ�� ǥ���ϴ� �簢���� bottom�� �����Ѵ�.
        m_textRect.bottom -= rowHeight;
    }
    
    // �ܼ� ��� ������Ʈ ǥ��
    // ��� ������Ʈ�� ���� ����� ǥ���Ѵ�.
    // ������Ʈ �ؽ�Ʈ�� ǥ���ϴ� �簢���� �����Ѵ�.
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
    m_pInput->Clear(NSInput::KEYS_PRESSED | NSInput::TEXT_IN); // ������ �Է��� �����.
}

void CConsole::Print(const std::string& str) {
    if (!m_initialized) {
        return;
    }
    m_text.push_front(str); // str�� ��ť�� �߰��Ѵ�.
    if (m_text.size() > NSConsole::MAX_LINES) { // ���� ť�� �� á�ٸ�
        m_text.pop_back();  // ���� ������ ���� �����Ѵ�.
    }
}

std::string CConsole::GetCommand() {
    // �ܼ��� �ʱ�ȭ�� ���� �ʾҰų� �� ���̴� ���
    if (!m_initialized || !m_visible) {
        return "";
    }

    // �ܼ�Ű�� Ȯ��
    if (m_pInput->WasKeyPressed(CONSOLE_KEY)) {
        Hide();
    }
    // ESCŰ�� Ȯ��
    if (m_pInput->WasKeyPressed(ESC_KEY)) {
        return "";
    }
    // ��ũ�� Ȯ��
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
    // ������ ���� Ű�� �����ϸ� �� �ȴ�.
    m_pInput->Clear(NSInput::KEYS_DOWN | NSInput::KEYS_PRESSED | NSInput::MOUSE);

    if (m_command.length() == 0) {
        return "";
    }

    if (m_command.at(m_command.length() - 1) != '\r') { // ���͸� ������ �ʾҴٸ�
        return "";
    }
    m_command.erase(m_command.length() - 1); // \r�� �����.
    m_pInput->ClearTextIn(); // �Է����� �����.

    return m_command; // ��� ��ȯ
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
