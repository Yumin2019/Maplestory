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

	// 메소 출력
	PEN->FontSetting(FT_UI);

	CPlayer* pPlayer = (CPlayer*)m_pObj;

	int meso = pPlayer->GetGold();

	wchar_t str[30];

	swprintf_s(str, L"%d", meso);

	TextOut(hDC, mesoX - 8 * lstrlen(str), mesoY, str, lstrlen(str));

	PEN->FontReturn();




	// 아이템 목록 출력
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

		// 그림 출력
		m_Item[CInventoryPanel::m_tSlot[row][col].eTag]->Render(hDC, x, y);


		// 아이템 이름 출력
		wstring strName;

		switch (CInventoryPanel::m_tSlot[row][col].eTag)
		{
			case IT_RED_POTION:
				strName = L"빨간 포션";

				break;

			case IT_ORANGE_POTION:
				strName = L"주황 포션";

				break;

			case IT_WHITE_POTION:
				strName = L"하얀 포션";

				break;

			case IT_BLUE_POTION:
				strName = L"파란 포션";

				break;

			case IT_SPEED_POTION:
				strName = L"속도향상의 물약";


				break;

			case IT_WORRIOR_POTION:
				strName = L"전사의 물약";

				break;

			case IT_APPLE:
				strName = L"사과";

				break;

			case IT_EGG:
				strName = L"달걀";

				break;

			case IT_ORANGE:
				strName = L"오렌지";

				break;
		}

		TextOut(hDC, x + 40.f, y - 2.f, strName.c_str(), strName.size());



		// 남은 개수 출력

		swprintf_s(str, L"%d", CInventoryPanel::m_tSlot[row][col].m_iCount);

		TextOut(hDC, x - 2.f, y + 12.f, str, lstrlen(str));



		// 판매 가격 출력

		swprintf_s(str, L"판매 가격 : %d 메소", m_Item[CInventoryPanel::m_tSlot[row][col].eTag]->GetSell());

		TextOut(hDC, x + 40.f, y + 14.f, str, lstrlen(str));


		++iCnt;
		y += 42.f;

		// 없는 것 출력 넘고, 있는 것만 모아서 ...

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
	// 이 함수는, 우측에 있는 아이템의 상대적인 위치에 따라서
	// 2차원 배열의 인벤토리를 가지고 오는 함수이다.


	/*
	float x = m_tRenderPos.x + 42.f + 244.f;
		float y = m_tRenderPos.y + 125.f + 42.f * i;
	
	*/
	CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

	POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();

	m_tMousePos.x -= 286.f;
	m_tMousePos.y -= 125.f;

	// 일단 선택한 블럭이 몇 번째인지 구한다.

	int iCount = m_tMousePos.y / 42.f;

	// m_tSlot 에서 < + 9개를 출력하고 있으니, 이 데이터를 검사하여 
	// 해당 번째를 파악한다.

	// 다음 칸이 아이템이 있을 거라는 확신이 없다.

	int row;
	int col;

	int cnt = 0;

	for (int i = m_iSlot; i < 24; ++i)
	{
		row = i / 4;
		col = i % 4;

		if (CInventoryPanel::m_tSlot[row][col].eTag == IT_NONE)
			continue;

		// 그림이라면 Count를 센다.

		if (cnt == iCount)
			break;

		++cnt;
	}
	
	// 여기서 구한 row col이 마우스로 클릭한 아이템의 위치라고 할 수 있다.

	
	return GetPoint(row, col);
}

int CShopUI::GetShopBlockIndex()
{
	CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

	POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();

	m_tMousePos.x -= 12.f;
	m_tMousePos.y -= 126.f;

	// 일단 선택한 블럭이 몇 번째인지 구한다.

	int iCount = m_tMousePos.y / 42.f;


	return iCount;
}

void CShopUI::GetItem(int idx)
{
	CItem* pItem = m_Item[idx];

	ITEM_TAG eTag = pItem->GetTag(); // 선택한 아이템의 태그.

	// 해당 아이템이 이미 칸에 있는지 확인한다.

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
				// 인벤토리를 뒤져보면서, 내가 사려고 하는 아이템이 이미 칸에 있는지 검사한다.

				// 여기 걸린 경우 있는 것이다.

				is = true;

				break;

				// i j save
			}

			
		}

		if (is)
			break;
	}

	

	if (is) // 사려는 아이템이 이미 인벤토리에 존재한다. idx를 가지고 처리
	{
		++CInventoryPanel::m_tSlot[row][col].m_iCount; // 단순한 개수 증가.
		GET_SINGLE(CSceneManager)->GetPlayer()->AddGold(-pItem->GetCost()); // 가격을 지불.

		return;
	}
	else // 존재하지 않는다.
	{

		// 처음부터 다시 뒤져가면서, 빈 공간에 tag를 넣고, Count 증가.

		for (int i = 0; i < 6; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (CInventoryPanel::m_tSlot[i][j].eTag == IT_NONE)
				{
					CInventoryPanel::m_tSlot[i][j].eTag = eTag;

					++CInventoryPanel::m_tSlot[i][j].m_iCount;

					GET_SINGLE(CSceneManager)->GetPlayer()->AddGold(-pItem->GetCost()); // 가격을 지불.

					return;
				}
				
			}

		}



	}




		// 여기 까지 내려오면... -> 아이템을 넣지 못했다.
	
		MessageBox(NULL, L"가방이 꽉 찼습니다.", L"구매불가", MB_OK);

		// 가격 지불은 없다.

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
	// 근데 여기서 최상단 UI가 아니라면, 처리를 막아야 한다.

	if (CMouse::m_pPanel != this)
		return;


	CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

	// 더블 클릭에 대한 처리가 왼쪽 충돌체를 누른 경우와 오른쪽 충돌체를 누른 경우로 나뉜다.

	// 그것은 마우스의 상대적인 위치를 가지고 계산한다.

	POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();

	// 그림에서의 마우스의 좌표를 구했다.

	// 이 좌표의 x 값을 가지고, 특정 x값을 비교하여 왼쪽인지 오른쪽인지 구분한다.

	if (m_tMousePos.x < 277.f) // 구매
	{
		// 왼쪽에서 충돌

		LeftCollision();

	}



	else // 판매
	{
		// 오른쪽에서 충돌
		RightCollision();
	}


}

void CShopUI::ItemBlockEnd(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (CMouse::m_pPanel != this)
		return;

	m_bClick = false;
	CMouse::m_bClickCycle = false;

	// 아이템을 더블클릭하는 처리만 하고 있다.


}

void CShopUI::LeftCollision()
{


	// 여기서 구한 인덱스를 그대로 아이템에 적용하면 된다.
	int iCount = GetShopBlockIndex();

	// 더블 클릭을 했는가를 따진다.

	if (KEYDOWN("MouseLButton") && !m_bClick)
	{
		m_iCount = iCount;
		m_bClick = true;
		CMouse::m_bClickCycle = true;

	}
	else if (KEYDOWN("MouseLButton")) // 이전 클릭 포함.
	{
		if (m_iCount == iCount) // 더블 클릭 + 아이템이 있었다.
		{
			// 해당 아이템을 구매한다.

			m_bClick = false;

			// 구매 : 내가 가진 돈에서 해당 아이템의 cost를 지불하고, 아이템을 인벤토리에 넣는 과정.

			int iCost = m_Item[m_iCount]->GetCost(); // 가격을 얻어서.

			CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

			// 돈을 지불할 수 있는지 여부를 확인

			if (pPlayer->GetGold() >= iCost)
			{
				// 내가 가진 돈이 비용보다 크다면, 살 수 있다.

				GetItem(m_iCount); // 인덱스에 해당하는 아이템을 하나 얻는다.
			}

			// 살 수 없다. -> 처리 X
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
	else if (KEYDOWN("MouseLButton") && CInventoryPanel::m_tSlot[row][col].eTag != IT_NONE) // 이전 클릭 포함.
	{
		if (m_iRow == row && m_iCol == col) // 더블 클릭 + 아이템이 있었다.
		{
			// 해당 아이템을 판매한다.
			--CInventoryPanel::m_tSlot[row][col].m_iCount;


			m_bClick = false;
			//m_bItem = false;

			// 판매 : 내가 가진 것의 개수를 하나 빼고, 그것의 돈을 얻는다.

			int iSell = m_Item[CInventoryPanel::m_tSlot[row][col].eTag]->GetSell();

			GET_SINGLE(CSceneManager)->GetPlayer()->AddGold(iSell);
			
			if (CInventoryPanel::m_tSlot[row][col].m_iCount == 0)
			{
				CInventoryPanel::m_tSlot[row][col].eTag = IT_NONE;
			}
		}
	}


}
