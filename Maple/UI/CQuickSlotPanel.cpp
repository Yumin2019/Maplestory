#include "CQuickSlotPanel.h"
#include "../Collision/CShape.h"


CQuickSlotPanel::CQuickSlotPanel()
{
}

CQuickSlotPanel::CQuickSlotPanel(const CQuickSlotPanel & panel) :
	CUIPanel(panel)
{

}


CQuickSlotPanel::~CQuickSlotPanel()
{
}

bool CQuickSlotPanel::Init()
{
	return true;
}

void CQuickSlotPanel::Input(float fDeltaTime)
{
}

void CQuickSlotPanel::Collision(float fDeltaTime)
{
}

void CQuickSlotPanel::Update(float fDeltaTime)
{

}

void CQuickSlotPanel::Render(HDC hDC, float fDeltaTime)
{
	CUI::Render(hDC, fDeltaTime);




	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

CQuickSlotPanel * CQuickSlotPanel::Clone()
{
	return new CQuickSlotPanel(*this);
}
