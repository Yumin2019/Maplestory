#pragma once

#include "../CRef.h"
#include "../Core/CCreateManager.h"

class CTexture :
	public CRef
{
private:
	CTexture();
	~CTexture();


private:
	HDC m_hMemDC;

	HBITMAP m_hBitMap;
	HBITMAP m_hOldBitMap;

	BITMAP m_tInfo;

	COLORREF m_Color;

	_SIZE	m_tOffset;
	_SIZE	m_tRenderSize;

public:

	// RenderSize

	void SetRenderSize(float x, float y)
	{
		m_tRenderSize = _SIZE(x, y);
	}

	void SetRenderSize(_SIZE tPos)
	{
		m_tRenderSize = tPos;
	}

	_SIZE GetRenderSize() const
	{
		return m_tRenderSize;
	}

	float GetRenderSizeX() const
	{
		return m_tRenderSize.x;
	}

	float GetRenderSizeY() const
	{
		return m_tRenderSize.y;
	}
	// OffSet

	void SetOffset(float x, float y)
	{
		m_tOffset = _SIZE(x, y);
	}

	void SetOffset(_SIZE tSize)
	{
		m_tOffset = tSize;
	}

	_SIZE GetOffset() const
	{
		return m_tOffset;
	}

	// Color

	void SetColor(unsigned int r, unsigned int g, unsigned int b)
	{
		m_Color = RGB(r, g, b);
	}

	void SetColor(COLORREF color)
	{
		m_Color = color;
	}

	COLORREF GetColor() const
	{
		return m_Color;
	}

	HDC GetDC() const
	{
		return m_hMemDC;
	}

	// Texture의 너비와 높이 얻어오기

	int GetWidth() const
	{
		return m_tInfo.bmWidth;
	}


	int GetHeight() const
	{
		return m_tInfo.bmHeight;
	}

	// 생성용
	static CTexture* SetTexture(const wstring & strName, _SIZE m_tRenderSize, unsigned int r, unsigned int g, unsigned int b);

	// wstring - 경로 지정.
	bool Init(const wstring& strName);


};

