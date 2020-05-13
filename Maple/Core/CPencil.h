#pragma once

#include "../Game.h"

class CPencil
{
	DECLARE_SINGLE(CPencil);

private:
	HDC		m_hDC;
	HPEN	m_hPen[PC_SIZE];
	HPEN	m_hOldPen;

	HBRUSH	m_hOldBrush;

	HFONT	m_hFont[FT_SIZE];
	HFONT	m_hOldFont;


public:
	
	bool Init(HDC hDC);

	// Ư�� ����, dc, �������� �޾Ƽ� �׷��ִ� �Լ� Rect�� �׸�
	void DrawRect(PEN_COLOR ePenColor, POSITION m_tSrc, POSITION m_tDest);
	void DrawLine(PEN_COLOR ePenColor, POSITION m_tSrc, POSITION m_tDest);
	void DrawEllipse(PEN_COLOR ePenColor, POSITION m_tSrc, POSITION m_tDest);

	
	// SetTextColor
	void FontSetting(FONT_TYPE eFont);
	void FontReturn();


};

