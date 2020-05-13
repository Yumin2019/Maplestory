#include "CScroll.h"
#include "../Core/CSceneManager.h"
#include "../Object/CPlayer.h"
#include "../Collision/CShape.h"
#include "../UI/CShopUI.h"
#include "../UI/CMouse.h"
#include "../Core/CInput.h"


CScroll::CScroll() :
	m_pShopUI(NULL)
{
	CShape* pBlock = new CShape;

	pBlock->InitColliderShape(_SIZE(9.f, 26.f), GetPoint(0,0), ST_SHOP_SCROLL);

	pBlock->SetObj(this);

	AddCollider(pBlock);
}

CScroll::CScroll(const CScroll & panel) :
	CUIPanel(panel)
{
}


CScroll::~CScroll()
{
}

bool CScroll::Init()
{
	return false;
}

void CScroll::Input(float fDeltaTime)
{
}

void CScroll::Collision(float fDeltaTime)
{

}

void CScroll::Update(float fDeltaTime)
{

	if (!((CPlayer*)m_pObj)->GetShop())
		return;


	// true인 경우에만 Update

	// 489 135

	m_tRenderPos = m_pShopUI->GetRenderPos();

	m_tRenderPos.x += 490.f; 
	m_tRenderPos.y += 135.f + CShopUI::m_iSlot * SCROLL_HEIGHT; // 349 / 16 -> 15개 이용 22


	 // 그대로. 15가 마지막 인덱스. 즉, slot은 0 ~ 15의 범위. 하지만, 칸은 14칸만 이용할 수 있다.
	// 15번을 움직일 수 있어야 한다. -> 16번을 쪼개야 한다.

	// m_iSlot 은 0 ~ 14까지 옮길 수 있다.
	// 이 m_iSlot의 값에 따라 그러니까... 
	// m_iSlot이 14면, 스크롤이 맨 아래에 존재해야 한다.
	// m_iSlot의 값에 따라 출력되는 offset을 바꾼다.



	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

void CScroll::Render(HDC hDC, float fDeltaTime)
{
	if (!((CPlayer*)m_pObj)->GetShop())
		return;

	CUI::Render(hDC, fDeltaTime);



	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

CScroll * CScroll::Clone()
{
	return new CScroll(*this);
}

void CScroll::MouseMoveStart(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CScroll::MouseMoveStay(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
