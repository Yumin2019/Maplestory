#include "CMouse.h"
#include "../Core/CSceneManager.h"
#include "../Collision/CShape.h"
#include "../Core/CInput.h"
#include "../Object/CPlayer.h"
#include "../UI/CUIPanel.h"
#include "../UI/CShopUI.h"
#include "../UI/CInventoryPanel.h"


class CUIPanel* CMouse::m_pPanel = NULL;
bool CMouse::m_bPress;
bool CMouse::m_bCollision;
bool CMouse::m_bClickCycle;

CMouse::CMouse()
{
	m_eObject = OT_MOUSE;
	m_tMove.x = 0.f;
	m_tMove.y = 0.f;
	m_fScrollPx = 0.f;
	m_fClickTime = 0.f;
	m_bAccess = false;
	m_bCollision = false;
}

CMouse::CMouse(const CMouse & mouse) :
	CUI(mouse)
{
}


CMouse::~CMouse()
{
}

bool CMouse::Init()
{
	// ���� : ���콺 �浹ü�� �߰��ص� ���� ����, 
	// ui�� ���� SHAPE �浹ü�� �߰��Ͽ�, ������� ��ġ�� ����ֵ����ϰ�,

	// MOUSE�� ���¸� ���ϴ� ó���� �Ѵ�.


	// �浹ü�� �߰��Ͽ� ó��.
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(4.f, 4.f), GetPoint(0, 0), ST_MOUSE);

	pShape->SetObj(this);

	AddCollider(pShape);

	pShape->AddCollisionFunc(CS_START, this, &CMouse::MouseStart);
	pShape->AddCollisionFunc(CS_STAY, this, &CMouse::MouseStay);
	pShape->AddCollisionFunc(CS_END, this, &CMouse::MouseEnd);


	// ���콺 ���� ��ǥ�� ����


	return true;
}

void CMouse::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);


	POINT tPos;

	GetCursorPos(&tPos);
	ScreenToClient(GET_SINGLE(CSceneManager)->GetHandle(), &tPos);

	m_tMove.x = (float)(tPos.x - m_tRenderPos.x);
	m_tMove.y = (float)(tPos.y - m_tRenderPos.y);
}

void CMouse::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);
}

void CMouse::Update(float fDeltaTime)
{

	POINT tPos;

	GetCursorPos(&tPos);
	ScreenToClient(GET_SINGLE(CSceneManager)->GetHandle(), &tPos);

	m_tRenderPos.x = (float)tPos.x;
	m_tRenderPos.y = (float)tPos.y;


	CUI::Update(fDeltaTime);


	if(m_bClickCycle)
	{
		m_fClickTime += fDeltaTime;

		if (m_fClickTime >= 2.f)
		{
			m_bClickCycle = false;
		}
	}


	// KEYPRESS && m_pPanel -> �����̵���.

	if (KEYPRESS("MouseLButton") && m_pPanel) // tag�� NONE�̶�� �ϴ���, �̹� �����Ϳ��� �ɸ���.
	{
		// scroll�ٴ� �������� �ϴµ�, RenderPos�� ���� ó���� �Ǵ� ���̴�.

		// NONE�� ���� �� ���� : m_pPanel�� ���� NONE ���� �ֱ� ������.

		switch (m_pPanel->GetPanelType())
		{
		default:
			DefaultMove();
			break;

		case PT_INVENTORY:

			if (CInventoryPanel::m_bItemMove)
				break;


			// ������ ������� ���� �����ؾ� �Ѵ�.
				DefaultMove();
			
				break;

		case PT_SCROLL:

			ScrollMove();
			break;
		}
		

	}
	else if (KEYUP("MouseLButton") && m_pPanel) 
	{
		// Key UP && Panel �� ��쿡�� 2���� ��찡 �ִ�.

		if (!m_bClickCycle)
		{
			// 1. ����ͼ� up�� ��� -> NULL (MOVE�� �ϴ� ���̶�, Click Cycle = false)

			m_pPanel = NULL;
			m_fClickTime = 0.f;
		}
		else 
		{
			// 2. ����Ŭ�� ���� ��� -> NULL X ClickCycle = true;

			
		}

	}

	

	if (!m_bCollision)
	{
		// �ƹ��͵� �浹���� �ʾҴٸ�, NULL�� �ƴ϶� false��..
		m_bClickCycle = false;
	}


	

	
}

void CMouse::Render(HDC hDC, float fDeltaTime)
{
	CUI::Render(hDC, fDeltaTime);

	wchar_t str[50];

	swprintf_s(str, L"Mouse : %.1f, %.1f", m_tRenderPos.x, m_tRenderPos.y);
	TextOut(hDC, 1100, 30, str, lstrlen(str));

	swprintf_s(str, L"Mouse Move : %.1f, %.1f", m_tMove.x, m_tMove.y);
	TextOut(hDC, 1100, 200, str, lstrlen(str));

	swprintf_s(str, L"ScrollPx : %.1f", m_fScrollPx);
	TextOut(hDC, 1100, 300, str, lstrlen(str));

	swprintf_s(str, L"MouseColl : %d", m_bCollision);
	TextOut(hDC, 1100, 50, str, lstrlen(str));


	swprintf_s(str, L"ClickCycle : %d", m_bClickCycle);
	TextOut(hDC, 1100, 100, str, lstrlen(str));


	// ���� ã�� ȭ���� ~ !
	wstring strTag = L"Panel : ";


	if (m_pPanel)
	{
		switch (m_pPanel->GetPanelType())
		{
		case	PT_SKILL: strTag += L"Skill"; break;
		case	PT_ABILITY: strTag += L"Abil"; break;
		case	PT_INVENTORY: strTag += L"Inven"; break;
		case	PT_SHOP: strTag += L"Shop"; break;
		case	PT_SCROLL: strTag += L"Scroll"; break;
		case	PT_MINIMAPBORDER: strTag += L"Minimap"; break;
		}

	}
	else
	{
		strTag += L"NONE";
	}

	TextOut(hDC, 1100, 60, strTag.c_str(), strTag.size());


	m_bCollision = false;
}

CMouse * CMouse::Clone()
{
	return new CMouse(*this);
}

void CMouse::MouseEnd(CCollider * pSrc, CCollider * pDest, float fTime)
{
	switch (pDest->GetShape())
	{
			/* UI ZOrder End */
		case ST_SHOP_SCROLL:
		case ST_ABILITY_UI_BUTTON:
		case ST_SHOP_UI_BUTTON:
		case ST_SKILL_UI_BUTTON:
		case ST_INVENTORY_UI_BUTTON:
		case ST_MINIMAP:
		{
			
			if (KEYPRESS("MouseLButton"))
			{
				// �������� ���� ������ �־��ٴ� �Ҹ���.

				// ��� �����ܾ� �Ѵٴ� ���� �ǹ�.

				// shape�� �׳� ����.
			}
			else
			{
				m_pPanel = NULL;
			}

		}
		break;

	}
}

void CMouse::DefaultMove()
{

	POSITION m_tPanel = m_pPanel->GetRenderPos() + m_tMove;
	_SIZE	 m_tPanelSize = m_pPanel->GetRenderSize();

	// Screen ��ǥ �ܺη� ������ ���� ���´�.

	if (m_tPanel.x < 0.f)
	{
		m_tPanel.x = 0.f;
	}

	if (m_tPanel.y < 0.f)
	{
		m_tPanel.y = 0.f;
	}

	if (m_tPanel.x + m_tPanelSize.x > RS_WIDTH)
	{
		m_tPanel.x = RS_WIDTH - m_tPanelSize.x;
	}

	if (m_tPanel.y + m_tPanelSize.y > RS_HEIGHT)
	{
		m_tPanel.y = RS_HEIGHT - m_tPanelSize.y;
	}

	m_pPanel->SetRenderPos(m_tPanel);

}

void CMouse::ScrollMove()
{
	/*
		��ũ���� LeftTop�� �ٲٴ� ������ ó���ؾ� �Ѵ�.
	*/

	// ���콺�� �̵��� ��ǥ


	if (isnan(m_fScrollPx))
		m_fScrollPx = 0.f;


	float m_fMoveY = m_tMove.y;

	if (m_fMoveY >= 0.f)
	{
		// �Ʒ��� �̵�.

		if (CShopUI::m_iSlot != SCROLL_IDX_MAX)
		{
			// ������ ���� �ִ�.
			
			m_fScrollPx += m_fMoveY;

			if (m_fScrollPx >= SCROLL_HEIGHT)
			{
				m_fScrollPx -= SCROLL_HEIGHT;
				++CShopUI::m_iSlot;
			}
		}

		// �� �̻� ������ ���� �����Ƿ� ó������ �ʴ´�

	}
	else
	{
		// ���� �̵�.

		if (CShopUI::m_iSlot != 0)
		{
			// �ö� ���� �ִ�.

			m_fScrollPx += m_fMoveY;

			if (m_fScrollPx <= -SCROLL_HEIGHT)
			{
				m_fScrollPx += SCROLL_HEIGHT;
				--CShopUI::m_iSlot;
			}
		}
		// �ö� ���� ����.
	}


}

void CMouse::MouseStart(CCollider * pSrc, CCollider * pDest, float fTime)
{
	// UI �浹ó�� �߿��� ���콺�� ���� �����ϴ� ���� ����.
	// Ư�� ��ü�� �浹�� ��쿡 ���� �־����� Ȯ���ϰ� ���� ���� �ʾҴٸ� true�� �ش�.
	if (!KEYPRESS("MouseLButton"))
	{
		m_bAccess = true;
	}
	else
	{
		// ���� �־���.
		m_bAccess = false;
	}
}

void CMouse::MouseStay(CCollider * pSrc, CCollider * pDest, float fTime)
{

	// ���콺�� �������� �浹�� �Ǿ��ٸ�..
	m_bCollision = true;

	// ������ access�� Ǯ ����� ������ ���⿡ �����.

	// �浹���ε�, ���� Key UP�� �� ��쿡 ������ ����Ѵ�.

	if (KEYUP("MouseLButton"))
		m_bAccess = true;

	// ������ ���� �־��ٸ�...
	if (!m_bAccess)
		return;



	switch (pDest->GetShape())
	{
	case ST_RECT_NPC:

		// �켱������ ���� �����ִ� ���� ������ ó���� ���´�.
		/*if (m_pPanel)
			return;*/

		if (KEYDOWN("MouseLButton"))
		{
			CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

			pPlayer->SetShop(true);

			m_fScrollPx = 0.f;
			CShopUI::m_iSlot = 0;

			// ������ ���� ����.
			GET_SINGLE(CSceneManager)->SetChange(pPlayer->GetShopUI()->GetZOrder());
		}

		break;

		/* UI ZOrder Check */

		// �� ģ������ ��Ȱ�� �ʿ��ϴ�.
	case ST_INVENTORY_UI_BUTTON:
	case ST_SHOP_UI_BUTTON:

		// ��������
		case ST_ABILITY_UI_BUTTON:
		case ST_SKILL_UI_BUTTON:
		case ST_MINIMAP:
		{


			/*
			�ϴ�, Ŭ���� ���� �� Panel�� ������ �� �ٲٰ� �Ͽ�, 
			���ÿ� ó���Ǵ� ��Ȳ�� ���´�.

			// �ٵ� �� ����� �߸Ŵ�..

			���� Ŭ���� �ϴ� �ð� ���� ó���� ���� �ʵ��� �ϴ� �ͻ����� ��� ���� ��ٸ���...

			�ٽ� Ǯ���� ... ��մ� ��Ȳ�� ������ ���̴�.
			*/

			if (m_bClickCycle)
				return;

			CUIPanel* pPanel = (CUIPanel*)pDest->GetObj();

			if (!m_pPanel)
			{
				m_pPanel = pPanel;


			}
			else
			{
				// ������ �浹�Ǵ� ���� �־���, ���� �̰Ͱ� �浹�ߴ�. -> üũ

				if (pPanel->GetZOrder() > m_pPanel->GetZOrder()) // �̹��� �浹�� ģ���� �� ���� �����Ѵ�. �̰ɷ� ��´�.
				{
					m_pPanel = pPanel;


				}
				
				// Zorder ���� ���� ���� �� ũ��. == �ٲ� �ʿ䰡 ����.
			}


			if (KEYDOWN("MouseLButton"))
			{
				// ������ �浹�� �Ǳ�� ���� ����. �׷� ��Ȳ���� Down�� �����ٴ� ����.
				// �� Panel�� ���� ����ø���� �ǹ�.

				// �Ϻδ� �浹�� ���ƾ� �Ѵ�.

				m_pPanel = pPanel;

				GET_SINGLE(CSceneManager)->SetChange(m_pPanel->GetZOrder());

				// �� ���� �ִ� �ϴ���, Ŭ���� ����. �� ģ���� ����.
			}

		}
		break;
		
	}

	
}
