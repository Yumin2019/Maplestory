#include "CAbilityPanel.h"
#include "../Object/CPlayer.h"
#include "../Collision/CShape.h"
#include "../Core/CInput.h"
#include "../UI/CMouse.h"
#include "../Core/CPencil.h"


CAbilityPanel::CAbilityPanel()
{
	CShape* pX = new CShape;

	pX->InitColliderShape(_SIZE(11.f, 11.f), GetPoint(193, 10), ST_ABILITY_UI_BUTTON);

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_STAY, this, &CAbilityPanel::MouseOnX);

	AddCollider(pX);


	pX = new CShape;

	//pX->InitColliderShape(_SIZE(210.f, 293.f), GetPoint(2, 3), ST_ABILITY_UI_BUTTON); 

	pX->InitColliderShape(_SIZE(212.f, 25.f), GetPoint(1, 1), ST_ABILITY_UI_BUTTON); 

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_START, this, &CAbilityPanel::MouseMoveStart);
	pX->AddCollisionFunc(CS_STAY, this, &CAbilityPanel::MouseMoveStay);

	AddCollider(pX);
}

CAbilityPanel::CAbilityPanel(const CAbilityPanel & panel) :
	CUIPanel(panel)
{
}


CAbilityPanel::~CAbilityPanel()
{
}

bool CAbilityPanel::Init()
{
	return true;
}

void CAbilityPanel::Input(float fDeltaTime)
{
}

void CAbilityPanel::Collision(float fDeltaTime)
{
}

void CAbilityPanel::Update(float fDeltaTime)
{
	if (!((CPlayer*)m_pObj)->GetAbility())
		return;

	// 업데이트 
	if (((CPlayer*)m_pObj)->GetAbility())
	{
		// 아직 없다.
	}

	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

void CAbilityPanel::Render(HDC hDC, float fDeltaTime)
{
	if (!((CPlayer*)m_pObj)->GetAbility())
		return;

	CUI::Render(hDC, fDeltaTime);


	// 다양한 능력치를 출력 32 y -> 18 

	wchar_t str[30];

	float X = m_tRenderPos.x + 74.f;
	float Y = m_tRenderPos.y + 32.f;

	PEN->FontSetting(FT_ITEM);


	CPlayer* pPlayer = (CPlayer*)m_pObj;

	// 이름 출력

	TextOut(hDC, X, Y, L"명랑코딩", 4);

	Y += 18.f;

	// 직업
	TextOut(hDC, X, Y, L"검사", 2);

	Y += 18.f;

	// 치명타 확률

	swprintf_s(str, L"%d%%", pPlayer->GetCriticalPercent());

	TextOut(hDC, X, Y, str, lstrlen(str));

	// 스탠스 확률 x + 99

	swprintf_s(str, L"%d%%", pPlayer->GetStancePercent());

	TextOut(hDC, X + 99.f, Y, str, lstrlen(str));


	// 스탯 공격력

	swprintf_s(str, L"%d ~ %d", pPlayer->GetAttackMin(), pPlayer->GetAttackMax());

	Y += 18.f;

	TextOut(hDC, X, Y, str, lstrlen(str));

	// 방어력

	swprintf_s(str, L"%d", pPlayer->GetArmor());

	Y += 18.f;

	TextOut(hDC, X, Y, str, lstrlen(str));

	// HP / Max HP

	swprintf_s(str, L"%d / %d", pPlayer->GetHP(), pPlayer->GetHPMax());

	Y += 18.f;

	TextOut(hDC, X, Y, str, lstrlen(str));

	// MP / Max MP

	swprintf_s(str, L"%d / %d", pPlayer->GetMP(), pPlayer->GetMPMax());

	Y += 18.f;

	TextOut(hDC, X, Y, str, lstrlen(str));

	// Str 부터 y + 70

	// 스탯 포인트

	swprintf_s(str, L"%d", pPlayer->GetRemainPoint());

	TextOut(hDC, X - 3.f, Y + 41.f, str, lstrlen(str));

	
	// STR
	swprintf_s(str, L"%d", pPlayer->GetStr());

	Y += 68.f;

	TextOut(hDC, X, Y, str, lstrlen(str));

	// DEX

	swprintf_s(str, L"%d", pPlayer->GetDex());

	Y += 18.f;

	TextOut(hDC, X, Y, str, lstrlen(str));


	// INT

	swprintf_s(str, L"%d", pPlayer->GetInt());

	Y += 18.f;

	TextOut(hDC, X, Y, str, lstrlen(str));


	// LUK

	swprintf_s(str, L"%d", pPlayer->GetLuk());

	Y += 18.f;

	TextOut(hDC, X, Y, str, lstrlen(str));





	PEN->FontReturn();











	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

CAbilityPanel * CAbilityPanel::Clone()
{
	return new CAbilityPanel(*this);
}

void CAbilityPanel::MouseOnX(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (CMouse::m_pPanel != this)
		return;


	if (KEYDOWN("MouseLButton"))
	{
		CPlayer* pPlayer = (CPlayer*)m_pObj;

		pPlayer->SetAbility(!pPlayer->GetAbility());

	}
}

void CAbilityPanel::MouseMoveStart(CCollider * pSrc, CCollider * pDest, float fTime)
{
	// 누르고 있던 경우라면, 처리를 하지 말아야 한다.
	// 이것은 끌어당기는 친구들에서 사용이 된다.

}

void CAbilityPanel::MouseMoveStay(CCollider * pSrc, CCollider * pDest, float fTime)
{
}


