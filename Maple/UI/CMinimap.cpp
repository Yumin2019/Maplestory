#include "CMinimap.h"
#include "../Collision/CShape.h"
#include "../Core/CInput.h"
#include "../UI/CMouse.h"
#include "../Object/CPlayer.h"


CMinimap::CMinimap()
{
	CShape* pX = new CShape;

	pX->InitColliderShape(_SIZE(181.f, 120.f), GetPoint(0, 0), ST_MINIMAP);

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_START, this, &CMinimap::MouseMoveStart);
	pX->AddCollisionFunc(CS_STAY, this, &CMinimap::MouseMoveStay);

	AddCollider(pX);
}

CMinimap::CMinimap(const CMinimap & panel) :
	CUIPanel(panel)
{
}


CMinimap::~CMinimap()
{
}

bool CMinimap::Init()
{
	return false;
}

void CMinimap::Input(float fDeltaTime)
{
}

void CMinimap::Collision(float fDeltaTime)
{
}

void CMinimap::Update(float fDeltaTime)
{
	CUIPanel::Update(fDeltaTime);

	if (!((CPlayer*)m_pObj)->GetMinimap())
		return;


	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

void CMinimap::Render(HDC hDC, float fDeltaTime)
{
	CUIPanel::Render(hDC, fDeltaTime);

	if (!((CPlayer*)m_pObj)->GetMinimap())
		return;


	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

CMinimap * CMinimap::Clone()
{
	return new CMinimap(*this);
}

void CMinimap::MouseMoveStart(CCollider * pSrc, CCollider * pDest, float fTime)
{
	
}

void CMinimap::MouseMoveStay(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
