#pragma once
#include "CCollider.h"

typedef struct _tagPixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

}PIXEL, *PPIXEL;

class CPixel :
	public CCollider
{
public:
	CPixel();
	CPixel(const CPixel& pixel);
	~CPixel();

private:
	vector<PIXEL> m_vecPixel;
	int			  m_iWidth;
	int			  m_iHeight;

	POINT		  m_tRopePos;

public:

	// Rope Pos

	void SetRopePos(int x, int y)
	{
		m_tRopePos.x = x;
		m_tRopePos.y = y;
	}

	POINT GetRopePos() const
	{
		return m_tRopePos;
	}

	bool InitColliderPixel(const char* pFileName);


	int GetWidth() const
	{
		return m_iWidth;
	}

	int GetHeight() const
	{
		return m_iHeight;
	}

	void SetSize(int x, int y)
	{
		m_iWidth = x;
		m_iHeight = y;
	}
	
	const vector<PIXEL>& GetPixelVec()
	{
		return m_vecPixel;
	}


public:
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CPixel* Clone();
	virtual void RenderText(HDC hDC);

};

