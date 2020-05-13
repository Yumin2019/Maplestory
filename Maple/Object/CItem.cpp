#include "CItem.h"
#include "../Core/CSceneManager.h"
#include "CPlayer.h"
#include "../Texture/CTexture.h"


CItem::CItem() :
	m_eTag(IT_NONE),
	m_iCost(0),
	m_iSell(0)
{
	m_eObject = OT_ITEM;
}

CItem::CItem(const CItem & item) :
	CStaticObj(item)
{
	m_eTag = item.m_eTag;

	m_iCost = item.m_iCost;
	m_iSell = item.m_iSell;


}


CItem::~CItem()
{
}

void CItem::Use()
{
	CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

	// 서서히 증가되는 처리는 나중에 해보도록.

	switch (m_eTag)
	{
	case IT_RED_POTION:
		pPlayer->AddHP(50);
		break;

	case IT_ORANGE_POTION:
		pPlayer->AddHP(150);
		break;

	case IT_WHITE_POTION:
		pPlayer->AddHP(300);
		break;

	case IT_BLUE_POTION:
		pPlayer->AddMP(100);
		break;

	case IT_SPEED_POTION:
		pPlayer->SetSpeedPotion();
		
		break;

	case IT_WORRIOR_POTION:
		pPlayer->SetWorriorPotion();

		break;

	case IT_APPLE:
		pPlayer->AddHP(30);

		break;

	case IT_EGG:
		pPlayer->AddHP(50);

		break;

	case IT_ORANGE:
		pPlayer->AddMP(50);

		break;
	}
}

bool CItem::Init()
{
	switch (m_eTag)
	{
	case IT_RED_POTION:
		m_iCost = 5;
		m_iSell = 3;

		SetSize(27.f, 27.f);
		break;

	case IT_ORANGE_POTION:
		m_iCost = 48;
		m_iSell = 24;
		SetSize(27.f, 27.f);

		break;

	case IT_WHITE_POTION:
		m_iCost = 96;
		m_iSell = 48;
		SetSize(27.f, 27.f);

		break;

	case IT_BLUE_POTION:
		m_iCost = 19;
		m_iSell = 10;
		SetSize(27.f, 27.f);

		break;

	case IT_SPEED_POTION:
		m_iCost = 400;
		m_iSell = 200;

		SetSize(25.f, 25.f);

		break;

	case IT_WORRIOR_POTION:
		m_iCost = 500;
		m_iSell = 250;
		SetSize(25.f, 25.f);

		break;

	case IT_APPLE:
		m_iCost = 3;
		m_iSell = 2;
		SetSize(20.f, 25.f);

		break;

	case IT_EGG:
		m_iCost = 5;
		m_iSell = 3;
		SetSize(27.f, 21.f);

		break;

	case IT_ORANGE:
		m_iCost = 9;
		m_iSell = 5;
		SetSize(21.f, 20.f);
		break;
	}



	return true;

}

void CItem::Input(float fDeltaTime)
{
}

void CItem::Collision(float fDeltaTime)
{
}

void CItem::Update(float fDeltaTime)
{
}

void CItem::Render(HDC hDC, float fDeltaTime)
{
}

CItem * CItem::Clone()
{
	return new CItem(*this);
}

void CItem::Render(HDC hDC, float x, float y)
{
	TransparentBlt(hDC, x, y, GetSizeX(), GetSizeY(), m_pTexture->GetDC(), 0, 0, GetSizeX(), GetSizeY(),
		m_pTexture->GetColor());
}
