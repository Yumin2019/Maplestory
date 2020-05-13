#include "CShopUI.h"
#include "../Object/CPlayer.h"
#include "../Core/CInput.h"
#include "CMouse.h"
#include "../Collision/CShape.h"
#include "../Core/CPencil.h"
#include "../Object/CItem.h"
#include "../Core/CCreateManager.h"
#include "CInventoryPanel.h"
#include "../Core/CSceneManager.h"


int CShopUI::m_iSlot;

CShopUI::CShopUI() :
	m_iCount(0),
	m_iRow(0),
	m_iCol(0),
	m_bClick(false)
{
	m_pInven = NULL;

	CShape* pX = new CShape;

	pX->InitColliderShape(_SIZE(64.f, 16.f), GetPoint(202, 55), ST_SHOP_UI_BUTTON);

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_STAY, this, &CShopUI::MouseOnX);

	AddCollider(pX);


	pX = new CShape;

	pX->InitColliderShape(_SIZE(506.f, 503.f), GetPoint(1, 1), ST_SHOP_UI_BUTTON);

	//pX->InitColliderShape(_SIZE(505.f, 93.f), GetPoint(1, 1), ST_SHOP_UI_BUTTON);

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_START, this, &CShopUI::MouseMoveStart);
	pX->AddCollisionFunc(CS_STAY, this, &CShopUI::MouseMoveStay);

	AddCollider(pX);


	// 17 124 x : 245  y : 42
	for (int i = 0; i < 9; ++i)
	{
		float x = m_tRenderPos.x + 12.f;
		float y = m_tRenderPos.y + 125.f + 42.f * i;

		CShape* pBlock = new CShape;

		pBlock->InitColliderShape(_SIZE(243.f, 34.f), GetPoint(x, y), ST_ITEM_BLOCK);

		pBlock->SetObj(this);

		pBlock->AddCollisionFunc(CS_START, this, &CShopUI::ItemBlockStart);
		pBlock->AddCollisionFunc(CS_STAY, this, &CShopUI::ItemBlockStay);
		pBlock->AddCollisionFunc(CS_END, this, &CShopUI::ItemBlockEnd);


		AddCollider(pBlock);
	}


	for (int i = 0; i < 9; ++i)
	{
		float x = m_tRenderPos.x + 42.f + 244.f;
		float y = m_tRenderPos.y + 125.f + 42.f * i;

		CShape* pBlock = new CShape;

		pBlock->InitColliderShape(_SIZE(200.f, 34.f), GetPoint(x, y), ST_ITEM_BLOCK);

		pBlock->SetObj(this);

		pBlock->AddCollisionFunc(CS_START, this, &CShopUI::ItemBlockStart);
		pBlock->AddCollisionFunc(CS_STAY, this, &CShopUI::ItemBlockStay);
		pBlock->AddCollisionFunc(CS_END, this, &CShopUI::ItemBlockEnd);


		AddCollider(pBlock);
	}
}

CShopUI::CShopUI(const CShopUI & panel) :
	CUIPanel(panel)
{

}


CShopUI::~CShopUI()
{
	for (int i = IT_RED_POTION; i < IT_SIZE; ++i)
	{
		SAFE_RELEASE(m_Item[i]);
	}
}

bool CShopUI::Init()
{
	string strTag;

	for (int i = IT_RED_POTION; i < IT_SIZE; ++i)
	{
		switch (i)
		{
		case IT_RED_POTION:
			strTag = "RedPotion";

			break;

		case IT_ORANGE_POTION:
			strTag = "OrangePotion";

			break;

		case IT_WHITE_POTION:
			strTag = "WhitePotion";

			break;

		case IT_BLUE_POTION:
			strTag = "BluePotion";

			break;

		case IT_SPEED_POTION:
			strTag = "SpeedPotion";


			break;

		case IT_WORRIOR_POTION:
			strTag = "WorriorPotion";

			break;

		case IT_APPLE:
			strTag = "Apple";

			break;

		case IT_EGG:
			strTag = "Egg";

			break;

		case IT_ORANGE:
			strTag = "Orange";

			break;
		}

		m_Item[i] = FIND(CItem)->FindObj(strTag)->Clone();
	}

	return true;
}

void CShopUI::Input(float fDeltaTime)
{
	if (KEYDOWN("SlotUp"))
	{
		m_iSlot++;

		if (m_iSlot > 15)
			m_iSlot = 15;
	}

	if (KEYDOWN("SlotDown"))
	{
		--m_iSlot;

		if (m_iSlot < 0)
			m_iSlot = 0;
	}
}

void CShopUI::Collision(float fDeltaTime)
{
}

void CShopUI::Update(float fDeltaTime)
{
	/*if (!KEYPRESS("MouseLButton"))
	{
		m_bPress = false;

	}*/

	if (!((CPlayer*)m_pObj)->GetShop())
		return;

	
	

	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

void CShopUI::Render(HDC hDC, float fDeltaTime)
{
	if (!((CPlayer*)m_pObj)->GetShop())
		return;

	CUI::Render(hDC, fDeltaTime);


	float mesoX = m_tRenderPos.x + 493.f;
	float mesoY = m_tRenderPos.y + 58.f;

	// �޼� ���
	PEN->FontSetting(FT_UI);

	CPlayer* pPlayer = (CPlayer*)m_pObj;

	int meso = pPlayer->GetGold();

	wchar_t str[30];

	swprintf_s(str, L"%d", meso);

	TextOut(hDC, mesoX - 8 * lstrlen(str), mesoY, str, lstrlen(str));

	PEN->FontReturn();




	// ������ ��� ���
	PEN->FontSetting(FT_ITEM);

	float x = m_tRenderPos.x + 290.f;
	float y = m_tRenderPos.y + 129.f;
		// y 42

	int iCnt = 0;

	for (int i = m_iSlot; i < 24; ++i)
	{
		int row = i / 4;
		int col = i % 4;

		if (CInventoryPanel::m_tSlot[row][col].eTag == IT_NONE)
			continue;

		// �׸� ���
		m_Item[CInventoryPanel::m_tSlot[row][col].eTag]->Render(hDC, x, y);


		// ������ �̸� ���
		wstring strName;

		switch (CInventoryPanel::m_tSlot[row][col].eTag)
		{
			case IT_RED_POTION:
				strName = L"���� ����";

				break;

			case IT_ORANGE_POTION:
				strName = L"��Ȳ ����";

				break;

			case IT_WHITE_POTION:
				strName = L"�Ͼ� ����";

				break;

			case IT_BLUE_POTION:
				strName = L"�Ķ� ����";

				break;

			case IT_SPEED_POTION:
				strName = L"�ӵ������ ����";


				break;

			case IT_WORRIOR_POTION:
				strName = L"������ ����";

				break;

			case IT_APPLE:
				strName = L"���";

				break;

			case IT_EGG:
				strName = L"�ް�";

				break;

			case IT_ORANGE:
				strName = L"������";

				break;
		}

		TextOut(hDC, x + 40.f, y - 2.f, strName.c_str(), strName.size());



		// ���� ���� ���

		swprintf_s(str, L"%d", CInventoryPanel::m_tSlot[row][col].m_iCount);

		TextOut(hDC, x - 2.f, y + 12.f, str, lstrlen(str));



		// �Ǹ� ���� ���

		swprintf_s(str, L"�Ǹ� ���� : %d �޼�", m_Item[CInventoryPanel::m_tSlot[row][col].eTag]->GetSell());

		TextOut(hDC, x + 40.f, y + 14.f, str, lstrlen(str));


		++iCnt;
		y += 42.f;

		// ���� �� ��� �Ѱ�, �ִ� �͸� ��Ƽ� ...

		if (iCnt == 9)
			break;
	}

	PEN->FontReturn();




	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

CShopUI * CShopUI::Clone()
{
	return new CShopUI(*this);
}

POINT CShopUI::GetBlockIndex()
{
	// �� �Լ���, ������ �ִ� �������� ������� ��ġ�� ����
	// 2���� �迭�� �κ��丮�� ������ ���� �Լ��̴�.


	/*
	float x = m_tRenderPos.x + 42.f + 244.f;
		float y = m_tRenderPos.y + 125.f + 42.f * i;
	
	*/
	CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

	POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();

	m_tMousePos.x -= 286.f;
	m_tMousePos.y -= 125.f;

	// �ϴ� ������ ���� �� ��°���� ���Ѵ�.

	int iCount = m_tMousePos.y / 42.f;

	// m_tSlot ���� < + 9���� ����ϰ� ������, �� �����͸� �˻��Ͽ� 
	// �ش� ��°�� �ľ��Ѵ�.

	// ���� ĭ�� �������� ���� �Ŷ�� Ȯ���� ����.

	int row;
	int col;

	int cnt = 0;

	for (int i = m_iSlot; i < 24; ++i)
	{
		row = i / 4;
		col = i % 4;

		if (CInventoryPanel::m_tSlot[row][col].eTag == IT_NONE)
			continue;

		// �׸��̶�� Count�� ����.

		if (cnt == iCount)
			break;

		++cnt;
	}
	
	// ���⼭ ���� row col�� ���콺�� Ŭ���� �������� ��ġ��� �� �� �ִ�.

	
	return GetPoint(row, col);
}

int CShopUI::GetShopBlockIndex()
{
	CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

	POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();

	m_tMousePos.x -= 12.f;
	m_tMousePos.y -= 126.f;

	// �ϴ� ������ ���� �� ��°���� ���Ѵ�.

	int iCount = m_tMousePos.y / 42.f;


	return iCount;
}

void CShopUI::GetItem(int idx)
{
	CItem* pItem = m_Item[idx];

	ITEM_TAG eTag = pItem->GetTag(); // ������ �������� �±�.

	// �ش� �������� �̹� ĭ�� �ִ��� Ȯ���Ѵ�.

	bool is = false;

	int row;
	int col;

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{

			row = i;
			col = j;

			if (CInventoryPanel::m_tSlot[i][j].eTag == eTag)
			{
				// �κ��丮�� �������鼭, ���� ����� �ϴ� �������� �̹� ĭ�� �ִ��� �˻��Ѵ�.

				// ���� �ɸ� ��� �ִ� ���̴�.

				is = true;

				break;

				// i j save
			}

			
		}

		if (is)
			break;
	}

	

	if (is) // ����� �������� �̹� �κ��丮�� �����Ѵ�. idx�� ������ ó��
	{
		++CInventoryPanel::m_tSlot[row][col].m_iCount; // �ܼ��� ���� ����.
		GET_SINGLE(CSceneManager)->GetPlayer()->AddGold(-pItem->GetCost()); // ������ ����.

		return;
	}
	else // �������� �ʴ´�.
	{

		// ó������ �ٽ� �������鼭, �� ������ tag�� �ְ�, Count ����.

		for (int i = 0; i < 6; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (CInventoryPanel::m_tSlot[i][j].eTag == IT_NONE)
				{
					CInventoryPanel::m_tSlot[i][j].eTag = eTag;

					++CInventoryPanel::m_tSlot[i][j].m_iCount;

					GET_SINGLE(CSceneManager)->GetPlayer()->AddGold(-pItem->GetCost()); // ������ ����.

					return;
				}
				
			}

		}



	}




		// ���� ���� ��������... -> �������� ���� ���ߴ�.
	
		MessageBox(NULL, L"������ �� á���ϴ�.", L"���źҰ�", MB_OK);

		// ���� ������ ����.

}

void CShopUI::MouseOnX(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (CMouse::m_pPanel != this)
		return;

	if (KEYDOWN("MouseLButton"))
	{
		CPlayer* pPlayer = (CPlayer*)m_pObj;

		pPlayer->SetShop(false);

		m_iSlot = 0;

	}
}

void CShopUI::MouseMoveStart(CCollider * pSrc, CCollider * pDest, float fTime)
{
	
}

void CShopUI::MouseMoveStay(CCollider * pSrc, CCollider * pDest, float fTime)
{


}

void CShopUI::ItemBlockStart(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CShopUI::ItemBlockStay(CCollider * pSrc, CCollider * pDest, float fTime)
{
	// �ٵ� ���⼭ �ֻ�� UI�� �ƴ϶��, ó���� ���ƾ� �Ѵ�.

	if (CMouse::m_pPanel != this)
		return;


	CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

	// ���� Ŭ���� ���� ó���� ���� �浹ü�� ���� ���� ������ �浹ü�� ���� ���� ������.

	// �װ��� ���콺�� ������� ��ġ�� ������ ����Ѵ�.

	POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();

	// �׸������� ���콺�� ��ǥ�� ���ߴ�.

	// �� ��ǥ�� x ���� ������, Ư�� x���� ���Ͽ� �������� ���������� �����Ѵ�.

	if (m_tMousePos.x < 277.f) // ����
	{
		// ���ʿ��� �浹

		LeftCollision();

	}



	else // �Ǹ�
	{
		// �����ʿ��� �浹
		RightCollision();
	}


}

void CShopUI::ItemBlockEnd(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (CMouse::m_pPanel != this)
		return;

	m_bClick = false;
	CMouse::m_bClickCycle = false;

	// �������� ����Ŭ���ϴ� ó���� �ϰ� �ִ�.


}

void CShopUI::LeftCollision()
{


	// ���⼭ ���� �ε����� �״�� �����ۿ� �����ϸ� �ȴ�.
	int iCount = GetShopBlockIndex();

	// ���� Ŭ���� �ߴ°��� ������.

	if (KEYDOWN("MouseLButton") && !m_bClick)
	{
		m_iCount = iCount;
		m_bClick = true;
		CMouse::m_bClickCycle = true;

	}
	else if (KEYDOWN("MouseLButton")) // ���� Ŭ�� ����.
	{
		if (m_iCount == iCount) // ���� Ŭ�� + �������� �־���.
		{
			// �ش� �������� �����Ѵ�.

			m_bClick = false;

			// ���� : ���� ���� ������ �ش� �������� cost�� �����ϰ�, �������� �κ��丮�� �ִ� ����.

			int iCost = m_Item[m_iCount]->GetCost(); // ������ ��.

			CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

			// ���� ������ �� �ִ��� ���θ� Ȯ��

			if (pPlayer->GetGold() >= iCost)
			{
				// ���� ���� ���� ��뺸�� ũ�ٸ�, �� �� �ִ�.

				GetItem(m_iCount); // �ε����� �ش��ϴ� �������� �ϳ� ��´�.
			}

			// �� �� ����. -> ó�� X
		}


	}
}

void CShopUI::RightCollision()
{
	POINT tIndex = GetBlockIndex();

	int row = tIndex.x;
	int col = tIndex.y;

	if (KEYDOWN("MouseLButton") && !m_bClick)
	{
		m_iRow = row;
		m_iCol = col;
		m_bClick = true;
		CMouse::m_bClickCycle = true;

	}
	else if (KEYDOWN("MouseLButton") && CInventoryPanel::m_tSlot[row][col].eTag != IT_NONE) // ���� Ŭ�� ����.
	{
		if (m_iRow == row && m_iCol == col) // ���� Ŭ�� + �������� �־���.
		{
			// �ش� �������� �Ǹ��Ѵ�.
			--CInventoryPanel::m_tSlot[row][col].m_iCount;


			m_bClick = false;
			//m_bItem = false;

			// �Ǹ� : ���� ���� ���� ������ �ϳ� ����, �װ��� ���� ��´�.

			int iSell = m_Item[CInventoryPanel::m_tSlot[row][col].eTag]->GetSell();

			GET_SINGLE(CSceneManager)->GetPlayer()->AddGold(iSell);
			
			if (CInventoryPanel::m_tSlot[row][col].m_iCount == 0)
			{
				CInventoryPanel::m_tSlot[row][col].eTag = IT_NONE;
			}
		}
	}


}
