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

	//SetBkMode(hDC, TRANSPARENT); // ��� ����

	// ������
	m_hPen[PC_BLACK] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	// �Ķ���
	m_hPen[PC_BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 162, 232));

	// ������
	m_hPen[PC_RED] = CreatePen(PS_SOLID, 1, RGB(237, 28, 36));

	// ����Ÿ
	m_hPen[PC_MAGENTA] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

	// ���
	m_hPen[PC_WHITE] = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	/////////////////////////

	// ��Ʈ ����.

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
	// ������ Color�� �����ϰ� Old�� �޾Ƶ�
	m_hOldPen = (HPEN)SelectObject(m_hDC, m_hPen[ePenColor]);

	// ������ �⺻ ���� null brush (�׳� ���� �ȴ�)
	m_hOldBrush = (HBRUSH)SelectObject(m_hDC, GetStockObject(NULL_BRUSH));

	// ���
	Rectangle(m_hDC, m_tSrc.x, m_tSrc.y, m_tDest.x, m_tDest.y);

	// ���� Brush�� ����
	SelectObject(m_hDC, m_hOldBrush);

	// Old���� �ٽ� ����
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


