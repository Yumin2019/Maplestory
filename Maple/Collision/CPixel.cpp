#include "CPixel.h"
#include "../Scene/CInGameScene.h"
#include "../Core/CPencil.h"
#include "../Object/CObj.h"


CPixel::CPixel() 
{
	m_tRopePos.x = 0;
	m_tRopePos.y = 0;

	m_iHeight = 0;
	m_iWidth = 0;
	m_eShape = ST_PIXEL;
}

CPixel::CPixel(const CPixel & pixel) :
	CCollider(pixel)
{
	m_tRopePos.x = 0;
	m_tRopePos.y = 0;

	m_iHeight = pixel.m_iHeight;
	m_iWidth = pixel.m_iWidth;

	// �ȼ��� �׳� �����͸� ������ ����. �װ� �� ���ڴ�.

	// �ѹ� �ҷ��ٰ�. Scene�ٲ� ������ �θ��� �ɷ�. 
	// �迭�� ������ �ְ�, ref = 2 ���·�.��
	// ��ŻŻ �� �ٸ� �ɷ� change ����. ũ
}


CPixel::~CPixel()
{
	m_vecPixel.clear();
}

bool CPixel::InitColliderPixel(const char * pFileName)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, pFileName, "rb");

	if (!pFile)
		return false;

	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;

	fread(&fh, sizeof(fh), 1, pFile);
	fread(&ih, sizeof(ih), 1, pFile);

	m_iHeight = ih.biHeight;
	m_iWidth = ih.biWidth;

	m_vecPixel.clear();

	m_vecPixel.resize(ih.biWidth * ih.biHeight);

	fread(&m_vecPixel[0], sizeof(PIXEL), ih.biWidth * ih.biHeight, pFile);


	PPIXEL pPixelArr = new PIXEL[m_iWidth];

	// �� �Ʒ� ����

	int max = m_iHeight / 2;
	for (int i = 0; i < max; ++i)
	{
		memcpy(pPixelArr, &m_vecPixel[i * m_iWidth], sizeof(PIXEL) * m_iWidth);
		memcpy(&m_vecPixel[i * m_iWidth], &m_vecPixel[(m_iHeight - i - 1) * m_iWidth], sizeof(PIXEL) * m_iWidth);
		memcpy(&m_vecPixel[(m_iHeight - i - 1) * m_iWidth], pPixelArr, sizeof(PIXEL) * m_iWidth);
	}

	delete[] pPixelArr;

	fclose(pFile);

	return true;
}


void CPixel::Update(float fDeltaTime)
{
	CCollider::Update(fDeltaTime);

	// ������ �������� �ʾ�.

	
}

void CPixel::Render(HDC hDC, float fDeltaTime)
{
	CCollider::Render(hDC, fDeltaTime);



}

CPixel * CPixel::Clone()
{
	return new CPixel(*this);
}

void CPixel::RenderText(HDC hDC)
{
	wchar_t str[40];
	wsprintf(str, L"Screen : (%d, %d)", m_iWidth, m_iHeight);
	TextOut(hDC, 1100, 20, str, lstrlen(str));


	wsprintf(str, L"Rope Pos : (%d, %d)", m_tRopePos.x, m_tRopePos.y);
	TextOut(hDC, 350, 20, str, lstrlen(str));
}
