#include "CTexture.h"



CTexture::CTexture() :
	m_hMemDC(NULL),
	m_Color(RGB(255,0,255)),
	m_tOffset(0.f, 0.f),
	m_tRenderSize(0.f, 0.f)
{
}


CTexture::~CTexture()
{
	// ������ ��� �ִ� ������ ��ü�Ѵ�.
	SelectObject(m_hMemDC, m_hOldBitMap);

	// ��Ʈ���� �����Ѵ�
	DeleteObject(m_hBitMap);

	// �޸� DC�� �����Ѵ�
	DeleteDC(m_hMemDC);
}

bool CTexture::Init(const wstring & strName)
{
	m_hMemDC = CreateCompatibleDC(GET_SINGLE(CCreateManager)->GetMemDC());

	m_hBitMap = (HBITMAP)LoadImage(GET_SINGLE(CCreateManager)->GetHInst(), strName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// �̹����� �޸� DC�� �����ϰ� ������ ��� �ִ� ������ �޾Ƶд�.

	m_hOldBitMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

	GetObject(m_hBitMap, sizeof(m_tInfo), &m_tInfo);

	return true;
}

CTexture* CTexture::SetTexture(const wstring & strName, _SIZE m_tRenderSize, unsigned int r, unsigned int g, unsigned int b)
{
		wstring Text = L"Texture/" + strName;

		CTexture* m_pTexture = new CTexture;

		m_pTexture->Init(Text);

		m_pTexture->SetColor(r, g, b);

		m_pTexture->SetRenderSize(m_tRenderSize);

		return m_pTexture;
}


