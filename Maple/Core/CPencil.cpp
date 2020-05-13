#include "CPencil.h"

DEFINITION_SINGLE(CPencil);

CPencil::~CPencil()
{
	for (int i = PC_BLACK; i < PC_SIZE; ++i)
	{
		DeleteObject(m_hPen[i]);
	}

	for (int i = FT_UI; i < FT_SIZE; ++i)
	{
		DeleteObject(m_hFont[i]);
	}
}

CPencil::CPencil()
{

}

bool CPencil::Init(HDC hDC)
{
	m_hDC = hDC;

	//SetBkMode(hDC, TRANSPARENT); // 배경 투명

	// 검정펜
	m_hPen[PC_BLACK] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	// 파랑색
	m_hPen[PC_BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 162, 232));

	// 빨강색
	m_hPen[PC_RED] = CreatePen(PS_SOLID, 1, RGB(237, 28, 36));

	// 마젠타
	m_hPen[PC_MAGENTA] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

	// 흰색
	m_hPen[PC_WHITE] = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	/////////////////////////

	// 폰트 지정.

	LOGFONT lf;

	lf.lfHeight = 10;
	lf.lfWidth = 8;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 700;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	lstrcpy(lf.lfFaceName, L"UIFont");

	m_hFont[FT_UI] = CreateFontIndirect(&lf);

	// LV
	lf.lfHeight = 17;
	lf.lfWidth = 0;

	lstrcpy(lf.lfFaceName, L"LvFont");

	m_hFont[FT_LV] = CreateFontIndirect(&lf);

	// ITEM
	lf.lfHeight = 15;
	lf.lfWeight = 400;

	lstrcpy(lf.lfFaceName, L"ItemFont");

	m_hFont[FT_ITEM] = CreateFontIndirect(&lf);

	


	return true;
}

void CPencil::DrawRect(PEN_COLOR ePenColor, POSITION m_tSrc, POSITION m_tDest)
{
	// 선택한 Color를 장착하고 Old를 받아둠
	m_hOldPen = (HPEN)SelectObject(m_hDC, m_hPen[ePenColor]);

	// 윈도우 기본 제공 null brush (그냥 쓰면 된다)
	m_hOldBrush = (HBRUSH)SelectObject(m_hDC, GetStockObject(NULL_BRUSH));

	// 사용
	Rectangle(m_hDC, m_tSrc.x, m_tSrc.y, m_tDest.x, m_tDest.y);

	// 이전 Brush를 장착
	SelectObject(m_hDC, m_hOldBrush);

	// Old펜을 다시 장착
	SelectObject(m_hDC, m_hOldPen);
}

void CPencil::DrawLine(PEN_COLOR ePenColor, POSITION m_tSrc, POSITION m_tDest)
{
	m_hOldPen = (HPEN)SelectObject(m_hDC, m_hPen[ePenColor]);

	MoveToEx(m_hDC, m_tSrc.x, m_tSrc.y, NULL);
	LineTo(m_hDC, m_tDest.x, m_tDest.y);

	SelectObject(m_hDC, m_hOldPen);
}

void CPencil::DrawEllipse(PEN_COLOR ePenColor, POSITION m_tSrc, POSITION m_tDest)
{
	m_hOldPen = (HPEN)SelectObject(m_hDC, m_hPen[ePenColor]);

	m_hOldBrush = (HBRUSH)SelectObject(m_hDC, GetStockObject(NULL_BRUSH));

	Ellipse(m_hDC, m_tSrc.x, m_tSrc.y, m_tDest.x, m_tDest.y);

	SelectObject(m_hDC, m_hOldBrush);

	SelectObject(m_hDC, m_hOldPen);
}

void CPencil::FontSetting(FONT_TYPE eFont)
{
	m_hOldFont = (HFONT)SelectObject(m_hDC, m_hFont[eFont]);
}

void CPencil::FontReturn()
{
	SelectObject(m_hDC, m_hOldFont);
}


