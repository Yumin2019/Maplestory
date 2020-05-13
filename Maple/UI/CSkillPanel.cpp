#include "CSkillPanel.h"
#include "../Object/CPlayer.h"
#include "../Collision/CShape.h"
#include "../Core/CInput.h"
#include "../UI/CMouse.h"
#include "../Core/CPencil.h"


CSkillPanel::CSkillPanel()
{
	CShape* pX = new CShape;

	pX->InitColliderShape(_SIZE(11.f, 11.f), GetPoint(136, 5), ST_SKILL_UI_BUTTON);

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_STAY, this, &CSkillPanel::MouseOnX);

	AddCollider(pX);


	pX = new CShape;

	//pX->InitColliderShape(_SIZE(157.f, 257.f), GetPoint(0, 0), ST_SKILL_UI_BUTTON);


	pX->InitColliderShape(_SIZE(157.f, 22.f), GetPoint(0, 0), ST_SKILL_UI_BUTTON);

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_START, this, &CSkillPanel::MouseMoveStart);
	pX->AddCollisionFunc(CS_STAY, this, &CSkillPanel::MouseMoveStay);

	AddCollider(pX);
}

CSkillPanel::CSkillPanel(const CSkillPanel & panel) :
	CUIPanel(panel)
{
}


CSkillPanel::~CSkillPanel()
{
}

bool CSkillPanel::Init()
{
	return true;
}

void CSkillPanel::Input(float fDeltaTime)
{
}

void CSkillPanel::Collision(float fDeltaTime)
{
}

void CSkillPanel::Update(float fDeltaTime)
{
	if (!((CPlayer*)m_pObj)->GetSkill())
		return;

		// 스킬 업데이트 

	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

void CSkillPanel::Render(HDC hDC, float fDeltaTime)
{
	if (!((CPlayer*)m_pObj)->GetSkill())
		return;

	CUI::Render(hDC, fDeltaTime);

	wchar_t str[30];

	float X = m_tRenderPos.x + 122.f;
	float Y = m_tRenderPos.y + 33.f;

	PEN->FontSetting(FT_UI);

	// 스킬 포인트



	PEN->FontReturn();

	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

CSkillPanel * CSkillPanel::Clone()
{
	return new CSkillPanel(*this);
}

void CSkillPanel::MouseOnX(CCollider * pSrc, CCollider * pDest, float fTime)
{

	if (CMouse::m_pPanel != this)
		return;


	if (KEYDOWN("MouseLButton"))
	{
		CPlayer* pPlayer = (CPlayer*)m_pObj;

		pPlayer->SetSkill(!pPlayer->GetSkill());


	}
}

void CSkillPanel::MouseMoveStart(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void CSkillPanel::MouseMoveStay(CCollider * pSrc, CCollider * pDest, float fTime)
{
}


