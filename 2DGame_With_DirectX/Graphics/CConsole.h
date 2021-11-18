#pragma once

#include <string>
#include <deque>
#include "CTextDX.h"
#include "../CGraphics.h"
#include "../CInput.h"

namespace NSConsole {
	const UINT WIDTH = 500;             // �ܼ� â�� �ʺ�
	const UINT HEIGHT = 400;            // �ܼ� â�� ����
	const UINT X = 5;                   // �ܼ� ��ġ
	const UINT Y = 5;
	const UINT MARGIN = 4;              // �ܼ� �����ڸ� margin
	const char FONT[] = "Courier New";  // �ܼ� ��Ʈ
	const int FONT_HEIGHT = 14;         // �ܼ� HEIGHT
	const COLOR_ARGB FONT_COLOR = NSGraphics::WHITE;    // �ܼ� �ؽ�Ʈ�� ����
	const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192, 128, 128, 128);    // ��׶��� �÷�
	const int MAX_LINES = 256;          // �ؽ�Ʈ ������ �ִ� ���� ��
}

class CConsole {
private : 
	bool					m_initialized;
	CGraphics*				m_pGraphics;
	CInput*					m_pInput;
	bool					m_visible;
	COLOR_ARGB				m_fontColor;
	COLOR_ARGB				m_backColor;
	float					m_x, m_y;
	RECT					m_textRect;
	VertexC					m_vertices[4]; // ����� ���� ���ؽ�
	CTextDX					m_dxFont;
	std::string				m_inputStr;
	std::string				m_command;
	std::deque<std::string> m_text;       // �ֿܼ� ����� �ؽ�Ʈ ť
	LP_VERTEXBUFFER			m_vertexBuffer;
	int						m_row;
	int						m_scrollAmount;
	
public : 
	CConsole();
	virtual ~CConsole();

	bool Initialize(CGraphics* pGraphics, CInput* pInput);
	const void Draw();
	void ShowHide();
	bool GetVisible() { return m_visible; }
	void Show() { m_visible = true; }
	void Hide() { m_visible = false; }
	void Print(const std::string& str);
	std::string GetCommand();
	std::string GetInput() { return m_inputStr; }
	void ClearInput() { m_inputStr = ""; }

	void OnLostDevice();
	void OnResetDevice();
};

