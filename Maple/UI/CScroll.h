#pragma once
#include "CUIPanel.h"


class CScroll :
	public CUIPanel
{

public:
	CScroll();
	CScroll(const CScroll& panel);
	~CScroll();






private:
	class CShopUI* m_pShopUI;

public:
	void SetShopPointer(class CShopUI* pShop)
	{
		m_pShopUI = pShop;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CScroll* Clone();

private:

	void MouseMoveStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseMoveStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

