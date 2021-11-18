#pragma once

#include <string>
#include <deque>
#include "CTextDX.h"
#include "../CGraphics.h"
#include "../CInput.h"

namespace NSConsole {
	const UINT WIDTH = 500;             // 콘솔 창의 너비
	const UINT HEIGHT = 400;            // 콘솔 창의 높이
	const UINT X = 5;                   // 콘솔 위치
	const UINT Y = 5;
	const UINT MARGIN = 4;              // 콘솔 가장자리 margin
	const char FONT[] = "Courier New";  // 콘솔 폰트
	const int FONT_HEIGHT = 14;         // 콘솔 HEIGHT
	const COLOR_ARGB FONT_COLOR = NSGraphics::WHITE;    // 콘솔 텍스트의 색상
	const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192, 128, 128, 128);    // 백그라운드 컬러
	const int MAX_LINES = 256;          // 텍스트 버퍼의 최대 라인 수
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
	VertexC					m_vertices[4]; // 배경을 위한 버텍스
	CTextDX					m_dxFont;
	std::string				m_inputStr;
	std::string				m_command;
	std::deque<std::string> m_text;       // 콘솔에 출력할 텍스트 큐
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

