#pragma once

#include "CUI.h"
class CMouse :
	public CUI
{
public:
	CMouse();
	CMouse(const CMouse& mouse);
	~CMouse();

private:
	POSITION m_tMove;
	bool	 m_bAccess;

public:
	static class CUIPanel* m_pPanel;
	static bool m_bPress;
	static bool m_bCollision;
	static bool m_bClickCycle;

	float m_fScrollPx;

	float m_fClickTime;


	// Limit Px는 하드코딩.
	
public:

	// 이동 거리를 구하는 함수.
	POSITION GetMove() const
	{
		return m_tMove;
	}

	void SetScrollPx()
	{
		m_fScrollPx = 0.f;
	}

public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CMouse* Clone();

public:
	void MouseStay(class CCollider* pSrc, class CCollider* pDest, float fTime);

private:
	void MouseStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

	void DefaultMove();
	void ScrollMove();

};

