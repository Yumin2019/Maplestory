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

	// �κ��丮 �浹ü 6 * 4

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

	// �̷��� �ϸ�, �������κ��� �� px��ŭ �������ִ����� ���� �� �ִ�.


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

	// ������ �ֱ�
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
	// �ϴ� press�� ����ٴ� ���� press = false;
	if (KEYUP("MouseLButton"))
	{
		if (CMouse::m_bPress)
		{
			// row col�� ��� �־��ٴ� �̾߱�. + �ٱ����� up�� �� ���. = ������.

			CMouse* pMouse = GET_SINGLE(CSceneManager)->GetMouse();

			POSITION m_tMousePos = pMouse->GetRenderPos() - GetRenderPos();


			//���콺�� �׸� �ۿ� �ִ��� �˻�.

			if (m_tMousePos.x < 0.f || m_tMousePos.x > 172.f ||
				m_tMousePos.y < 0.f || m_tMousePos.y > 294.f)
			{

					// �߸��� ������ ��� �ѹ� �����, yes�� ��� ����.

					if (MessageBox(NULL, L"�������� �����ðڽ��ϱ� ?", L"������", MB_YESNO) == IDYES)
					{
						m_tSlot[m_iRow][m_iCol].eTag = IT_NONE;
						m_tSlot[m_iRow][m_iCol].m_iCount = 0;
					}


			}



				// ���� ���� �������� (�ٸ� ����) up�� �� ��쿡�� ó������ �ʴ´�.

		}

		// ���ٰ� �������� ���� ó���� ������ ���� �ƴϴ�. ���� Ŭ��.
		CMouse::m_bPress = false;

		m_bItemMove = false;

	}

	if (!((CPlayer*)m_pObj)->GetInventory())
		return;

	
		// �κ��丮 ������Ʈ

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

	// �κ��丮�� ���� ��ġ���� ������� ��ġ�� ��Ƽ� ���� �޼Ҹ� ����Ѵ�.

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


	// ���� ������ Render

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


	// �浹�߿� Ŭ���� �� ���.
	// �ش� ��ġ�� �������� �����ϴ� ��� �׷��� ���� ���.

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

		// press ���� �� ������ �����.
	}
	else if (KEYDOWN("MouseLButton") && m_tSlot[row][col].eTag != IT_NONE) // ���� Ŭ�� ����.
	{
		// Ŭ���� �߰�, ���� �־���.

		if (m_iRow == row && m_iCol == col) // ���� Ŭ�� + �������� �־���.
		{
			// �ش� �������� ����Ѵ�.
			--m_tSlot[row][col].m_iCount;

			m_bClick = false;

			// // ���⼭ false�� �־��شٴ� ����, ���콺���� NULL���� �ֵ��� �Ѵٴ� �ǹ�.
			//m_bItem = false;

			// �������� ��� ó��.

			if (m_tSlot[row][col].m_iCount == 0)
			{
				m_tSlot[row][col].eTag = IT_NONE;
			}
		}
	}


	// 1. ������ �������� ������. ( �̰����� ���� �ʴ´�. )
	// 2. �ٸ� ĭ���� �ű��.
	if (KEYUP("MouseLButton") && CMouse::m_bPress)
	{
		// 1. ������ �����ٰ� �ٽ� ���ͼ� UP
		// 2. ������ �����ٰ� �ٸ� ������ ���� UP

		// �� ��� ��� Swap

		SLOT temp = m_tSlot[m_iRow][m_iCol];

		m_tSlot[m_iRow][m_iCol] = m_tSlot[row][col];

		m_tSlot[row][col] = temp;

		m_bItemMove = false;

	/*	m_bItem = false;*/
		// ������� ������ ���� -> Ŭ�� ���� ����. -> �������� ����.
	}
	
}

void CInventoryPanel::ItemBlockEnd(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (CMouse::m_pPanel != this)
		return;


	CMouse::m_bClickCycle = false;
	m_bClick = false;

	// ����� ������ none�� �ƴ� ��쿡�� press = true
	if (KEYPRESS("MouseLButton") && m_tSlot[m_iRow][m_iCol].eTag != IT_NONE)
	{
		CMouse::m_bPress = true;
		// row col ������ ���.
	}
}

