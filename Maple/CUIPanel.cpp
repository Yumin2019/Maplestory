#include "UI\CUIPanel.h"
#include "Object/CPlayer.h"
#include "Texture/CTexture.h"
#include "Core/CPencil.h"
#include "Object/CNormalMonster.h"
#include "Core/CSceneManager.h"



CUIPanel::CUIPanel()  :
	m_pObj(NULL),
	m_iZOrder(-1)
{
	m_bBool = false;
}

CUIPanel::CUIPanel(const CUIPanel & panel) :
	CUI(panel)
{
	m_ePanel = panel.m_ePanel;
	m_bBool = false;
	m_iZOrder = panel.m_iZOrder;
}


CUIPanel::~CUIPanel()
{
}

bool CUIPanel::Init()
{
	// 특정 UI에 따라서, Collider 의 shape를 추가할 수 있다.

	

	return true;
}

void CUIPanel::Input(float fDeltaTime)
{
}

void CUIPanel::Collision(float fDeltaTime)
{

}

void CUIPanel::Update(float fDeltaTime)
{
	// UI flag에 따라서 Update처리를 달리한다.

	switch (m_ePanel)
	{
	case PT_HPBAR:

		HPBarUpdate();
		break;

	case PT_MPBAR:

		MPBarUpdate();
		break;


	case PT_HPMPBORDER:
		break;

		// HPBar 가 true일때만 계산하고 출력.

	case PT_MONSTERHPBORDER:
		
		if (((CNormalMonster*)m_pObj)->GetHPBar()) 
			MonsterHPBorderUpdate();
		break;

	case PT_MONSTERHPBAR:

		if (((CNormalMonster*)m_pObj)->GetHPBar())
			MonsterHPBarUpdate();
		break;
		
	case PT_MINIMAPBORDER:



		break;

	case PT_MINIMAP_MAIN:
	case PT_MINIMAP_MUSH1:
	case PT_MINIMAP_MUSH2:
	case PT_MINIMAP_GOLEM:
	case PT_MINIMAP_TRAININGROOM:
		if (GET_SINGLE(CSceneManager)->GetPlayer()->GetMinimap())
			MinimapUpdate();
		break;

	case PT_MINIMAP_CIRCLE_MAIN:
	case PT_MINIMAP_CIRCLE_MUSH1:
	case PT_MINIMAP_CIRCLE_MUSH2:
	case PT_MINIMAP_CIRCLE_GOLEM:
	case PT_MINIMAP_CIRCLE_TRAININGROOM:
		if (GET_SINGLE(CSceneManager)->GetPlayer()->GetMinimap())
			MinimapCircle();
		break;

	case PT_NAME:
		NameUpdate();
		break;

		
	}
}

void CUIPanel::Render(HDC hDC, float fDeltaTime)
{
	

	// Render를 한 이후에 출력해야 할 문구를 처리한다.

	switch (m_ePanel)
	{
	case PT_HPBAR:
		{
			CUI::Render(hDC, fDeltaTime);

			wstring str1 = to_wstring(((CPlayer*)m_pObj)->GetHP());
			wstring str2 = L"/" + to_wstring(((CPlayer*)m_pObj)->GetHPMax());

			wstring str = str1 + str2;

			PEN->FontSetting(FT_UI);

			int i = BarX + 70 - str1.size() * 8;
			int j = HPBarY + 2;

			TextOut(hDC, i, j, str.c_str(), str.size());

			PEN->FontReturn();

			PEN->FontSetting(FT_LV);

			SetTextColor(hDC, RGB(255, 204, 0));

			TextOut(hDC, BarX + 28, HPBarY - 24, L"123", 3);

			PEN->FontReturn();

			SetTextColor(hDC, RGB(0, 0, 0));



		}
		break;

	case PT_MPBAR:
		{
			CUI::Render(hDC, fDeltaTime);

			wstring str1 = to_wstring(((CPlayer*)m_pObj)->GetMP());
			wstring str2 = L"/" + to_wstring(((CPlayer*)m_pObj)->GetMPMax());

			wstring str = str1 + str2;

			PEN->FontSetting(FT_UI);

			TextOut(hDC, BarX + 70 - str1.size() * 8, MPBarY + 2, str.c_str(), str.size());

			PEN->FontReturn();



		}
		break;


	case PT_HPMPBORDER:

		CUI::Render(hDC, fDeltaTime);

		break;


		// 몬스터가 죽으면 같이 사라지도록 구성.

	case PT_MONSTERHPBORDER:
	case PT_MONSTERHPBAR:

		if (!m_pObj->GetLife())
		{
			Die();
		}

		if(((CNormalMonster*)m_pObj)->GetHPBar()) // HPBar = true일 때만 업데이트하고 출력하면 된다.
			CUI::Render(hDC, fDeltaTime);


		break;
		
		// 카메라 좌표 적용해서 출력.
	case PT_NAME:

		CObj::Render(hDC, fDeltaTime); // 

		break;

		// 그냥 Render

	case PT_QUICKSLOT_ALPHA: // 알파벳은 그냥 이쪽에서 처리.
	case PT_EXPBORDER:
	case PT_EXPBAR:
		CUI::Render(hDC, fDeltaTime);

		break;
	case PT_MINIMAP_CIRCLE_MAIN:
	case PT_MINIMAP_CIRCLE_MUSH1:
	case PT_MINIMAP_CIRCLE_MUSH2:
	case PT_MINIMAP_CIRCLE_GOLEM:
	case PT_MINIMAP_CIRCLE_TRAININGROOM:
	
	case PT_MINIMAPBORDER:
	case PT_MINIMAP_MAIN:
	case PT_MINIMAP_MUSH1:
	case PT_MINIMAP_MUSH2:
	case PT_MINIMAP_GOLEM:
	case PT_MINIMAP_TRAININGROOM:
		if (GET_SINGLE(CSceneManager)->GetPlayer()->GetMinimap())
			CUI::Render(hDC, fDeltaTime);

		break;

	

	
	}

}

CUIPanel * CUIPanel::Clone()
{
	return new CUIPanel(*this);
}

void CUIPanel::HPBarUpdate()
{
	// 플레이어의HP를 출력.
	// Bar상태에 따라서 Texture의 RenderSize를 조정한다.

	int iHP = ((CPlayer*)m_pObj)->GetHP();
	int iHPMax = ((CPlayer*)m_pObj)->GetHPMax();

	float fPercent = iHP / (float)iHPMax;

	m_pTexture->SetRenderSize(m_pTexture->GetWidth() * fPercent, m_pTexture->GetHeight());

	
}

void CUIPanel::MPBarUpdate()
{

	int iMP = ((CPlayer*)m_pObj)->GetMP();
	int iMPMax = ((CPlayer*)m_pObj)->GetMPMax();

	float fPercent = iMP / (float)iMPMax;

	m_pTexture->SetRenderSize(m_pTexture->GetWidth() * fPercent, m_pTexture->GetHeight());
}

void CUIPanel::MonsterHPBorderUpdate()
{
	// 몬스터의 좌표를 스크린 좌표로 옮겨서 offset맞춰 출력하면 된다.

	POSITION tMonsterScreenCenter = m_pObj->GetCenterScreenPos(m_pObj->GetCenter());

	POINT tPos;

	tPos.x = tMonsterScreenCenter.x - 25.f;
	tPos.y = tMonsterScreenCenter.y - 50.f;

	m_tRenderPos = tPos;

	// Border는 그냥 출력.
}

void CUIPanel::MonsterHPBarUpdate()
{
	
	POSITION tMonsterScreenCenter = m_pObj->GetCenterScreenPos(m_pObj->GetCenter());

	POINT tPos;

	tPos.x = tMonsterScreenCenter.x - 23.f;
	tPos.y = tMonsterScreenCenter.y - 48.f;

	m_tRenderPos = tPos;

	// 위치를 border에 맞추고, 체력에 맞춰서 일정부분을 출력.


	int iHP = ((CMoveObj*)m_pObj)->GetHP();
	int iHPMax = ((CMoveObj*)m_pObj)->GetHPMax();

	float fPercent = iHP / (float)iHPMax;

	m_pTexture->SetRenderSize(m_pTexture->GetWidth() * fPercent, m_pTexture->GetHeight());
}

void CUIPanel::MinimapUpdate()
{
	// 일단 기준 Obejct의 Render좌표를 가져온다.

	POSITION tBorder = m_pObj->GetRenderPos();

	// 보더의 중심이 이 좌표에서 X 160 Y 108이다.

	// x 중심에서 자신의 x / 2를 빼면 정확히 출력.
	m_tRenderPos.x = tBorder.x + 90.f - m_pTexture->GetWidth() / 2;

	m_tRenderPos.y = tBorder.y + 70.f - m_pTexture->GetHeight() / 2;

}

void CUIPanel::MinimapCircle()
{
	// 일단 현재 텍스처의 크기를 가지고 플레이어의 위치상의 비율을 구한다.

	class CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();
	class CTexture* pTexture = GET_SINGLE(CSceneManager)->GetTexture();

	float tX = pTexture->GetWidth();
	float tY = pTexture->GetHeight();

	POSITION tPos = pPlayer->GetPos();

	float fX = tPos.x / tX;
	float fY = tPos.y / tY;

	// 비율을 가지고 RenderPos를 조정한다.

	POSITION MinimapPos = m_pObj->GetRenderPos();

	CTexture* pMinimap = m_pObj->GetTexture();

	m_tRenderPos.x = MinimapPos.x + fX * pMinimap->GetWidth();
	m_tRenderPos.y = MinimapPos.y + fY * pMinimap->GetHeight();
	 
	// 기본은 이것이다.
	// 하지만 편집과정에서 미니맵이 정확히 맞아떨어지지 않는다.
	// 따라서 offset을 Flag로 조정해야 한다.

	switch (m_ePanel)
	{
	case PT_MINIMAP_CIRCLE_MAIN:
		m_tRenderPos.x -= 2.f;
		m_tRenderPos.y -= 8.f;
		break;

	case PT_MINIMAP_CIRCLE_MUSH1:
		break;

	case PT_MINIMAP_CIRCLE_MUSH2:
		break;

	case PT_MINIMAP_CIRCLE_GOLEM:
		break;

	case PT_MINIMAP_CIRCLE_TRAININGROOM:
		break;
	}
}

void CUIPanel::NameUpdate()
{
	POSITION tPos;

	tPos.x = m_pObj->GetCenterX() - 25.f;
	tPos.y = m_pObj->GetCenterY() + 35.f;

	m_tRenderPos = tPos;
}


void CUIPanel::SetPanel(POINT tRenderPos, CTexture * pTexture, PANEL_TYPE ePanel, class CObj* pObj)
{
	m_pObj = pObj;
	m_tRenderPos = tRenderPos;
	m_pTexture = pTexture;
	m_ePanel = ePanel;
}

void CUIPanel::SetPanelObj(CObj * pObj)
{
	m_pObj = pObj;
}
