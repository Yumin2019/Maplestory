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


	// true�� ��쿡�� Update

	// 489 135

	m_tRenderPos = m_pShopUI->GetRenderPos();

	m_tRenderPos.x += 490.f; 
	m_tRenderPos.y += 135.f + CShopUI::m_iSlot * SCROLL_HEIGHT; // 349 / 16 -> 15�� �̿� 22


	 // �״��. 15�� ������ �ε���. ��, slot�� 0 ~ 15�� ����. ������, ĭ�� 14ĭ�� �̿��� �� �ִ�.
	// 15���� ������ �� �־�� �Ѵ�. -> 16���� �ɰ��� �Ѵ�.

	// m_iSlot �� 0 ~ 14���� �ű� �� �ִ�.
	// �� m_iSlot�� ���� ���� �׷��ϱ�... 
	// m_iSlot�� 14��, ��ũ���� �� �Ʒ��� �����ؾ� �Ѵ�.
	// m_iSlot�� ���� ���� ��µǴ� offset�� �ٲ۴�.



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
