#include "CInventoryPanel.h"
#include "../Object/CPlayer.h"
#include "../Collision/CShape.h"
#include "../Core/CInput.h"
#include "../UI/CMouse.h"
#include "../Core/CPencil.h"
#include "../Core/CCreateManager.h"
#include "../Object/CItem.h"
#include "../Core/CSceneManager.h"
#include "../UI/CShopUI.h"

SLOT CInventoryPanel::m_tSlot[6][4];
bool CInventoryPanel::m_bItemMove;

CInventoryPanel::CInventoryPanel() :
	m_iRow(0),
	m_iCol(0)
{
	CShape* pX = new CShape;

	pX->InitColliderShape(_SIZE(11.f, 11.f), GetPoint(151, 8), ST_INVENTORY_UI_BUTTON);

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_STAY, this, &CInventoryPanel::MouseOnX);

	AddCollider(pX);


	pX = new CShape;

	pX->InitColliderShape(_SIZE(171.f, 289.f), GetPoint(0, 1), ST_INVENTORY_UI_BUTTON);

	//pX->InitColliderShape(_SIZE(172.f, 24.f), GetPoint(0, 1), ST_INVENTORY_UI_BUTTON);

	pX->SetObj(this);

	pX->AddCollisionFunc(CS_START, this, &CInventoryPanel::MouseMoveStart);
	pX->AddCollisionFunc(CS_STAY, this, &CInventoryPanel::MouseMoveStay);

	AddCollider(pX);

	// 인벤토리 충돌체 6 * 4

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			// 11 53  x : 37 y : 36
			float x = m_tRenderPos.x + 11.f + 36.f * j + 5.f;
			float y = m_tRenderPos.y + 53.f + 35.f * i + 5.f;

			CShape* pBlock = new CShape;

			pBlock->InitColliderShape(_SIZE(20.f, 20.f), GetPoint(x, y), ST_ITEM_BLOCK);

			pBlock->SetObj(this);

			pBlock->AddCollisionFunc(CS_START, this, &CInventoryPanel::ItemBlockStart);
			pBlock->AddCollisionFunc(CS_STAY, this, &CInventoryPanel::ItemBlockStay);
			pBlock->AddCollisionFunc(CS_END, this, &CInventoryPanel::ItemBlockEnd);


			AddCollider(pBlock);

		}
	}
	
	
}

CInventoryPanel::CInventoryPanel(const CInventoryPanel & panel) :
	CUIPanel(panel)
{
}


CInventoryPanel::~CInventoryPanel()
{
	for (int i = IT_RED_POTION; i < IT_SIZE; ++i)
	{
		SAFE_RELEASE(m_Item[i]);
	}
}

POINT CInventoryPanel::GetBlockIndex()
{
	CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

	POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();

	m_tMousePos.x -= 11.f;
	m_tMousePos.y -= 53.f;

	// 이렇게 하면, 사진으로부터 몇 px만큼 떨어져있는지를 구할 수 있다.


	int row = m_tMousePos.y / 35.f;
	int col = m_tMousePos.x / 36.f;

	return GetPoint(row, col);
}

bool CInventoryPanel::Init()
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

	// 아이템 넣기
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			int eTag = rand() % IT_SIZE;

			m_tSlot[i][j].eTag = (ITEM_TAG)eTag;

			m_tSlot[i][j].m_iCount = 3;
		}
	}


	return true;
}

void CInventoryPanel::Input(float fDeltaTime)
{
}

void CInventoryPanel::Collision(float fDeltaTime)
{

	
}

void CInventoryPanel::Update(float fDeltaTime)
{
	// 일단 press가 끊겼다는 것은 press = false;
	if (KEYUP("MouseLButton"))
	{
		if (CMouse::m_bPress)
		{
			// row col을 잡고 있었다는 이야기. + 바깥에서 up을 한 경우. = 버린다.

			CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

			POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();


			//마우스가 그림 밖에 있는지 검사.

			if (m_tMousePos.x < 0.f || m_tMousePos.x > 172.f ||
				m_tMousePos.y < 0.f || m_tMousePos.y > 294.f)
			{

					// 잘못된 범위인 경우 한번 물어보고, yes인 경우 버림.

					if (MessageBox(NULL, L"아이템을 버리시겠습니까 ?", L"버리기", MB_YESNO) == IDYES)
					{
						m_tSlot[m_iRow][m_iCol].eTag = IT_NONE;
						m_tSlot[m_iRow][m_iCol].m_iCount = 0;
					}


			}



				// 정상 적인 범위에서 (다른 공간) up을 한 경우에는 처리하지 않는다.

		}

		// 뗀다고 아이템의 대한 처리가 끝나는 것은 아니다. 더블 클릭.
		CMouse::m_bPress = false;

		m_bItemMove = false;

	}

	if (!((CPlayer*)m_pObj)->GetInventory())
		return;

	
		// 인벤토리 업데이트

	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

void CInventoryPanel::Render(HDC hDC, float fDeltaTime)
{
	if (!((CPlayer*)m_pObj)->GetInventory())
		return;

	CUI::Render(hDC, fDeltaTime);

	// 인벤토리의 렌더 위치에서 상대적인 위치를 잡아서 보유 메소를 출력한다.

	float mesoX = m_tRenderPos.x + 122.f;
	float mesoY = m_tRenderPos.y + 270.f;

	PEN->FontSetting(FT_UI);



	CPlayer* pPlayer = (CPlayer*)m_pObj;

	int meso = pPlayer->GetGold();

	wchar_t str[30];

	swprintf_s(str, L"%d", meso);

	TextOut(hDC, mesoX - 8 * lstrlen(str), mesoY, str, lstrlen(str));

	PEN->FontReturn();





	// Data Render

	//for (int i = 0; i < 6; ++i)
	//{
	//	for (int j = 0; j < 4; ++j)
	//	{
	//		wstring strSlot;

	//		switch (m_tSlot[i][j].eTag)
	//		{
	//		case IT_RED_POTION:
	//			strSlot = L"RedPotion";

	//			break;

	//		case IT_ORANGE_POTION:
	//			strSlot = L"OrangePotion";

	//			break;

	//		case IT_WHITE_POTION:
	//			strSlot = L"WhitePotion";

	//			break;

	//		case IT_BLUE_POTION:
	//			strSlot = L"BluePotion";

	//			break;

	//		case IT_SPEED_POTION:
	//			strSlot = L"SpeedPotion";


	//			break;

	//		case IT_WORRIOR_POTION:
	//			strSlot = L"WorriorPotion";

	//			break;

	//		case IT_APPLE:
	//			strSlot = L"Apple";

	//			break;

	//		case IT_EGG:
	//			strSlot = L"Egg";

	//			break;

	//		case IT_ORANGE:
	//			strSlot = L"Orange";

	//			break;

	//		case IT_NONE:
	//			strSlot = L"NONE";
	//			break;
	//		}

	//		swprintf_s(str, L" %d", m_tSlot[i][j].m_iCount);

	//		strSlot += str;

	//		TextOut(hDC, 500 + 150 * j, 20 + 20 * i, strSlot.c_str(), strSlot.size());
	//	}
	//}


	// 보유 아이템 Render

	PEN->FontSetting(FT_ITEM);

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			// 11 53  x : 37 y : 36
			if (m_tSlot[i][j].eTag == IT_NONE)
				continue;

			float x = m_tRenderPos.x + 14.f + 36.f * j;
			float y = m_tRenderPos.y + 56.f + 35.f * i;

			m_Item[m_tSlot[i][j].eTag]->Render(hDC, x, y);

			// text 14 79 

			swprintf_s(str, L"%d", m_tSlot[i][j].m_iCount);

			TextOut(hDC, x - 2.f, y + 12.f, str, lstrlen(str));
		}
	}

	PEN->FontReturn();


	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

CInventoryPanel * CInventoryPanel::Clone()
{
	return new CInventoryPanel(*this);
}

void CInventoryPanel::MouseOnX(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (CMouse::m_pPanel != this)
		return;


	if (KEYDOWN("MouseLButton"))
	{
		CPlayer* pPlayer = (CPlayer*)m_pObj;

		pPlayer->SetInventory(!pPlayer->GetInventory());


	}
}

void CInventoryPanel::MouseMoveStart(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void CInventoryPanel::MouseMoveStay(CCollider * pSrc, CCollider * pDest, float fTime)
{
}



void CInventoryPanel::ItemBlockStart(CCollider * pSrc, CCollider * pDest, float fTime)
{


}

void CInventoryPanel::ItemBlockStay(CCollider * pSrc, CCollider * pDest, float fTime)
{


	if (CMouse::m_pPanel != this)
		return;


	// 충돌중에 클릭을 한 경우.
	// 해당 위치에 아이템이 존재하는 경우 그렇지 않은 경우.

	POINT tIndex = GetBlockIndex();

	int row = tIndex.x;
	int col = tIndex.y;


	if (KEYDOWN("MouseLButton") && !m_bClick)
	{
		m_iRow = row;
		m_iCol = col;
		m_bClick = true;

		CMouse::m_bClickCycle = true;
		m_bItemMove = true;

		// press 또한 이 정보를 기억함.
	}
	else if (KEYDOWN("MouseLButton") && m_tSlot[row][col].eTag != IT_NONE) // 이전 클릭 포함.
	{
		// 클릭을 했고, 뭔가 있었다.

		if (m_iRow == row && m_iCol == col) // 더블 클릭 + 아이템이 있었다.
		{
			// 해당 아이템을 사용한다.
			--m_tSlot[row][col].m_iCount;

			m_bClick = false;

			// // 여기서 false를 넣어준다는 것은, 마우스에서 NULL값을 넣도록 한다는 의미.
			//m_bItem = false;

			// 아이템의 사용 처리.

			if (m_tSlot[row][col].m_iCount == 0)
			{
				m_tSlot[row][col].eTag = IT_NONE;
			}
		}
	}


	// 1. 밖으로 가져가서 버린다. ( 이곳으로 오지 않는다. )
	// 2. 다른 칸으로 옮긴다.
	if (KEYUP("MouseLButton") && CMouse::m_bPress)
	{
		// 1. 밖으로 나갔다가 다시 들어와서 UP
		// 2. 밖으로 나갔다가 다른 곳으로 들어가서 UP

		// 두 경우 모두 Swap

		SLOT temp = m_tSlot[m_iRow][m_iCol];

		m_tSlot[m_iRow][m_iCol] = m_tSlot[row][col];

		m_tSlot[row][col] = temp;

		m_bItemMove = false;

	/*	m_bItem = false;*/
		// 끌어오는 행위의 종료 -> 클릭 또한 종료. -> 아이템의 종료.
	}
	
}

void CInventoryPanel::ItemBlockEnd(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (CMouse::m_pPanel != this)
		return;


	CMouse::m_bClickCycle = false;
	m_bClick = false;

	// 끌고온 정보가 none이 아닌 경우에만 press = true
	if (KEYPRESS("MouseLButton") && m_tSlot[m_iRow][m_iCol].eTag != IT_NONE)
	{
		CMouse::m_bPress = true;
		// row col 정보를 기억.
	}
}

