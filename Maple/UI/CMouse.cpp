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
	// 현재 : 마우스 충돌체만 추가해둔 상태 이제, 
	// ui에 여러 SHAPE 충돌체를 추가하여, 상대적인 위치를 잡아주도록하고,

	// MOUSE와 상태를 비교하는 처리를 한다.


	// 충돌체를 추가하여 처리.
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(4.f, 4.f), GetPoint(0, 0), ST_MOUSE);

	pShape->SetObj(this);

	AddCollider(pShape);

	pShape->AddCollisionFunc(CS_START, this, &CMouse::MouseStart);
	pShape->AddCollisionFunc(CS_STAY, this, &CMouse::MouseStay);
	pShape->AddCollisionFunc(CS_END, this, &CMouse::MouseEnd);


	// 마우스 이전 좌표를 생성


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


	// KEYPRESS && m_pPanel -> 움직이도록.

	if (KEYPRESS("MouseLButton") && m_pPanel) // tag가 NONE이라고 하더라도, 이미 포인터에서 걸린다.
	{
		// scroll바는 끌어당기기는 하는데, RenderPos에 따라서 처리가 되는 것이다.

		// NONE은 들어올 수 없음 : m_pPanel과 같이 NONE 값을 넣기 때문에.

		switch (m_pPanel->GetPanelType())
		{
		default:
			DefaultMove();
			break;

		case PT_INVENTORY:

			if (CInventoryPanel::m_bItemMove)
				break;


			// 아이템 끌어당기는 것을 구분해야 한다.
				DefaultMove();
			
				break;

		case PT_SCROLL:

			ScrollMove();
			break;
		}
		

	}
	else if (KEYUP("MouseLButton") && m_pPanel) 
	{
		// Key UP && Panel 의 경우에는 2가지 경우가 있다.

		if (!m_bClickCycle)
		{
			// 1. 끌고와서 up한 경우 -> NULL (MOVE를 하던 중이라, Click Cycle = false)

			m_pPanel = NULL;
			m_fClickTime = 0.f;
		}
		else 
		{
			// 2. 더블클릭 중인 경우 -> NULL X ClickCycle = true;

			
		}

	}

	

	if (!m_bCollision)
	{
		// 아무것도 충돌되지 않았다면, NULL이 아니라 false를..
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


	// 오류 찾기 화이팅 ~ !
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
				// 떨어지는 순간 누르고 있었다는 소리는.

				// 계속 끌어당겨야 한다는 것을 의미.

				// shape는 그냥 유지.
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

	// Screen 좌표 외부로 나가는 것을 막는다.

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
		스크롤의 LeftTop을 바꾸는 것으로 처리해야 한다.
	*/

	// 마우스의 이동한 좌표


	if (isnan(m_fScrollPx))
		m_fScrollPx = 0.f;


	float m_fMoveY = m_tMove.y;

	if (m_fMoveY >= 0.f)
	{
		// 아래로 이동.

		if (CShopUI::m_iSlot != SCROLL_IDX_MAX)
		{
			// 내려갈 곳이 있다.
			
			m_fScrollPx += m_fMoveY;

			if (m_fScrollPx >= SCROLL_HEIGHT)
			{
				m_fScrollPx -= SCROLL_HEIGHT;
				++CShopUI::m_iSlot;
			}
		}

		// 더 이상 내려갈 곳이 없으므로 처리하지 않는다

	}
	else
	{
		// 위로 이동.

		if (CShopUI::m_iSlot != 0)
		{
			// 올라갈 곳이 있다.

			m_fScrollPx += m_fMoveY;

			if (m_fScrollPx <= -SCROLL_HEIGHT)
			{
				m_fScrollPx += SCROLL_HEIGHT;
				--CShopUI::m_iSlot;
			}
		}
		// 올라갈 곳이 없다.
	}


}

void CMouse::MouseStart(CCollider * pSrc, CCollider * pDest, float fTime)
{
	// UI 충돌처리 중에서 마우스를 끌고서 시작하는 것은 없다.
	// 특정 물체와 충돌한 경우에 끌고 있었는지 확인하고 끌고 있지 않았다면 true를 준다.
	if (!KEYPRESS("MouseLButton"))
	{
		m_bAccess = true;
	}
	else
	{
		// 끌고 있었다.
		m_bAccess = false;
	}
}

void CMouse::MouseStay(CCollider * pSrc, CCollider * pDest, float fTime)
{

	// 마우스가 누군가와 충돌이 되었다면..
	m_bCollision = true;

	// 도저히 access를 풀 방법이 없으니 여기에 만든다.

	// 충돌중인데, 내가 Key UP을 한 경우에 권한을 줘야한다.

	if (KEYUP("MouseLButton"))
		m_bAccess = true;

	// 이전에 끌고 있었다면...
	if (!m_bAccess)
		return;



	switch (pDest->GetShape())
	{
	case ST_RECT_NPC:

		// 우선순위에 먼저 잡혀있는 것이 있으면 처리를 막는다.
		/*if (m_pPanel)
			return;*/

		if (KEYDOWN("MouseLButton"))
		{
			CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

			pPlayer->SetShop(true);

			m_fScrollPx = 0.f;
			CShopUI::m_iSlot = 0;

			// 상점을 위로 띄운다.
			GET_SINGLE(CSceneManager)->SetChange(pPlayer->GetShopUI()->GetZOrder());
		}

		break;

		/* UI ZOrder Check */

		// 이 친구들은 분활이 필요하다.
	case ST_INVENTORY_UI_BUTTON:
	case ST_SHOP_UI_BUTTON:

		// 각개전투
		case ST_ABILITY_UI_BUTTON:
		case ST_SKILL_UI_BUTTON:
		case ST_MINIMAP:
		{


			/*
			일단, 클릭을 했을 때 Panel을 강제로 못 바꾸게 하여, 
			동시에 처리되는 상황을 막는다.

			// 근데 이 방법은 야매다..

			더블 클릭을 하는 시간 동안 처리가 되지 않도록 하는 것뿐이지 어느 정도 기다리면...

			다시 풀리는 ... 재밌는 상황이 전개될 것이다.
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
				// 기존에 충돌되던 것이 있었고, 또한 이것과 충돌했다. -> 체크

				if (pPanel->GetZOrder() > m_pPanel->GetZOrder()) // 이번에 충돌한 친구가 더 위에 존재한다. 이걸로 잡는다.
				{
					m_pPanel = pPanel;


				}
				
				// Zorder 값이 이전 것이 더 크다. == 바꿀 필요가 없다.
			}


			if (KEYDOWN("MouseLButton"))
			{
				// 뭔가가 충돌이 되기는 했을 거임. 그런 상황에서 Down을 눌렀다는 것은.
				// 이 Panel을 위로 끌어올리라는 의미.

				// 일부는 충돌을 막아야 한다.

				m_pPanel = pPanel;

				GET_SINGLE(CSceneManager)->SetChange(m_pPanel->GetZOrder());

				// 안 잡혀 있다 하더라도, 클릭한 순간. 이 친구로 고정.
			}

		}
		break;
		
	}

	
}
