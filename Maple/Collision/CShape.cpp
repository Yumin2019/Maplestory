#include "CShape.h"
#include "../Object/CObj.h"
#include "../Scene/CInGameScene.h"
#include "../Core/CPencil.h"

int CShape::m_iPortal;
int CShape::m_iRect;

CShape::CShape() :
	m_tPos(0.f, 0.f),
	m_tSize(0.f, 0.f)
{
}

CShape::CShape(const CShape & shape) :
	CCollider(shape)
{
	m_tPos = shape.m_tPos;
	m_tSize = shape.m_tSize;
}


CShape::~CShape()
{
}

bool CShape::InitColliderShape(_SIZE tSize, POINT tLeftTop, SHAPE_TYPE eType)
{
	m_tSize = tSize;

	m_tLeftTop = tLeftTop;

	m_eShape = eType;

	return true;
}

void CShape::Update(float fDeltaTime)
{
	switch(m_eShape)
	{
	case ST_RECT_PORTAL:
		return;

	case ST_RECT_NORMAL_LEFT:
	case ST_RECT_NORMAL_RIGHT:
	case ST_RECT_SKILL_LEFT:
	case ST_RECT_SKILL_RIGHT:
	case ST_RECT_MONSTER:

		// 이 친구들은 1fps 계산 이후에 바로 충돌처리 하고, 없애야 하는 친구들이다.
		// 이전에 만난 흔적은 다음의 약속을 방해한다. 따라서 player input -> collision -> update(삭제) 과정을 거쳐서 문제없이 돌아가도록 한다.
		// 이 과정에서 값이 변하는지 텀을 생각해줘야 한다.
		// 그것을 nockBack으로 처리한다.
			{
				
				m_CollisionList.clear();
			}


		break;
	case ST_QUICKSLOT_UI:

		break;

	case ST_ITEM_BLOCK:
	case ST_SHOP_UI_BUTTON:
	case ST_MINIMAP:
	case ST_SKILL_UI_BUTTON:
	case ST_ABILITY_UI_BUTTON:
	case ST_INVENTORY_UI_BUTTON:
	case ST_SHOP_SCROLL:
		m_tPos = m_pObj->GetRenderPos() + m_tLeftTop;

		return;

	case ST_MOUSE:
		m_tPos = m_pObj->GetRenderPos();
		return;
	}

	CCollider::Update(fDeltaTime);

	m_tPos = m_pObj->GetPos() + m_tLeftTop;
}

void CShape::Render(HDC hDC, float fDeltaTime)
{
	// UI 충돌에 이용하는 것.
	switch (m_eShape)
	{

	case ST_SHOP_SCROLL:
	case ST_ITEM_BLOCK:
	case ST_SHOP_UI_BUTTON:
	case ST_MINIMAP:
	case ST_SKILL_UI_BUTTON:
	case ST_ABILITY_UI_BUTTON:
	case ST_INVENTORY_UI_BUTTON:

	case ST_MOUSE:
		PEN->DrawRect(PC_RED, m_tPos, m_tPos + m_tSize);
		return;
	}
	CCollider::Render(hDC, fDeltaTime);


	// world -> Screen

	float l = GetLeft() - CInGameScene::l;
	float t = GetTop() - CInGameScene::t;
	float r = l + m_tSize.x;
	float b = t + m_tSize.y;

	switch (m_eShape)
	{

	case ST_RECT_NORMAL_LEFT:
	case ST_RECT_NORMAL_RIGHT:
	case ST_RECT_SKILL_LEFT:
	case ST_RECT_SKILL_RIGHT:
		PEN->DrawRect(PC_BLACK, POSITION(l, t), POSITION(r, b));

		break;

	case ST_RECT_MONSTER_ATTACK:
		PEN->DrawRect(PC_RED, POSITION(l, t), POSITION(r, b));

			break;

	case ST_RECT_NPC:
	case ST_RECT_TURN:
	case ST_RECT_MONSTER:
	case ST_RECT_OBSTACLE:
	case ST_RECT_PORTAL:
	case ST_RECT_TERRAIN:
	case ST_RECT_ROPE:
		Rectangle(hDC, l, t, r, b);
		break;

	case ST_CIRCLE:
		Ellipse(hDC, l, t, r, b);

		break;
	}


	
}

CShape * CShape::Clone()
{
	return new CShape(*this);
}

void CShape::RenderText(HDC hDC)
{
	wchar_t strShape[50];

	switch (m_eShape)
	{

	case ST_RECT_OBSTACLE:
		swprintf_s(strShape, L" Rect%d : (%.2f , %.2f)", ++m_iRect, m_tPos.x, m_tPos.y);
		TextOut(hDC, 500, m_iRect * 20, strShape, lstrlen(strShape));

		break;
	case ST_RECT_TERRAIN:

		swprintf_s(strShape, L"(%.2f , %.2f)", m_tPos.x, m_tPos.y);
		TextOut(hDC, m_tPos.x - CInGameScene::l - 60.f, 
			m_tPos.y - CInGameScene::t + 40.f,	strShape, lstrlen(strShape));

		break;

	case ST_RECT_MONSTER:

		swprintf_s(strShape, L"(%.2f , %.2f)", m_tPos.x, m_tPos.y);
		TextOut(hDC, m_tPos.x - CInGameScene::l - 80.f,
			m_tPos.y - CInGameScene::t -30.f, strShape, lstrlen(strShape));

		break;

	case ST_RECT_ROPE:

		swprintf_s(strShape, L"중간 Rect : (%.2f , %.2f)", m_tPos.x, m_tPos.y);
		TextOut(hDC, m_tPos.x - CInGameScene::l - 80.f, m_tPos.y - CInGameScene::t - 50.f, 
			strShape, lstrlen(strShape));

		break;

	case ST_RECT_PORTAL:

		swprintf_s(strShape, L"포탈%d : (%.f , %.f)", ++m_iPortal, m_tPos.x, m_tPos.y);
		TextOut(hDC, m_tPos.x - CInGameScene::l - 60.f, m_tPos.y - CInGameScene::t + 50.f,
			strShape, lstrlen(strShape));

		break;
	}


}

POSITION CShape::GetWorldPos(const POSITION tPos)
{
	return POSITION(tPos.x +CInGameScene::l, tPos.y + CInGameScene::t);
}
