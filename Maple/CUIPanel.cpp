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
	// Ư�� UI�� ����, Collider �� shape�� �߰��� �� �ִ�.

	

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
	// UI flag�� ���� Updateó���� �޸��Ѵ�.

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

		// HPBar �� true�϶��� ����ϰ� ���.

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
	

	// Render�� �� ���Ŀ� ����ؾ� �� ������ ó���Ѵ�.

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


		// ���Ͱ� ������ ���� ��������� ����.

	case PT_MONSTERHPBORDER:
	case PT_MONSTERHPBAR:

		if (!m_pObj->GetLife())
		{
			Die();
		}

		if(((CNormalMonster*)m_pObj)->GetHPBar()) // HPBar = true�� ���� ������Ʈ�ϰ� ����ϸ� �ȴ�.
			CUI::Render(hDC, fDeltaTime);


		break;
		
		// ī�޶� ��ǥ �����ؼ� ���.
	case PT_NAME:

		CObj::Render(hDC, fDeltaTime); // 

		break;

		// �׳� Render

	case PT_QUICKSLOT_ALPHA: // ���ĺ��� �׳� ���ʿ��� ó��.
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
	// �÷��̾���HP�� ���.
	// Bar���¿� ���� Texture�� RenderSize�� �����Ѵ�.

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
	// ������ ��ǥ�� ��ũ�� ��ǥ�� �Űܼ� offset���� ����ϸ� �ȴ�.

	POSITION tMonsterScreenCenter = m_pObj->GetCenterScreenPos(m_pObj->GetCenter());

	POINT tPos;

	tPos.x = tMonsterScreenCenter.x - 25.f;
	tPos.y = tMonsterScreenCenter.y - 50.f;

	m_tRenderPos = tPos;

	// Border�� �׳� ���.
}

void CUIPanel::MonsterHPBarUpdate()
{
	
	POSITION tMonsterScreenCenter = m_pObj->GetCenterScreenPos(m_pObj->GetCenter());

	POINT tPos;

	tPos.x = tMonsterScreenCenter.x - 23.f;
	tPos.y = tMonsterScreenCenter.y - 48.f;

	m_tRenderPos = tPos;

	// ��ġ�� border�� ���߰�, ü�¿� ���缭 �����κ��� ���.


	int iHP = ((CMoveObj*)m_pObj)->GetHP();
	int iHPMax = ((CMoveObj*)m_pObj)->GetHPMax();

	float fPercent = iHP / (float)iHPMax;

	m_pTexture->SetRenderSize(m_pTexture->GetWidth() * fPercent, m_pTexture->GetHeight());
}

void CUIPanel::MinimapUpdate()
{
	// �ϴ� ���� Obejct�� Render��ǥ�� �����´�.

	POSITION tBorder = m_pObj->GetRenderPos();

	// ������ �߽��� �� ��ǥ���� X 160 Y 108�̴�.

	// x �߽ɿ��� �ڽ��� x / 2�� ���� ��Ȯ�� ���.
	m_tRenderPos.x = tBorder.x + 90.f - m_pTexture->GetWidth() / 2;

	m_tRenderPos.y = tBorder.y + 70.f - m_pTexture->GetHeight() / 2;

}

void CUIPanel::MinimapCircle()
{
	// �ϴ� ���� �ؽ�ó�� ũ�⸦ ������ �÷��̾��� ��ġ���� ������ ���Ѵ�.

	class CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();
	class CTexture* pTexture = GET_SINGLE(CSceneManager)->GetTexture();

	float tX = pTexture->GetWidth();
	float tY = pTexture->GetHeight();

	POSITION tPos = pPlayer->GetPos();

	float fX = tPos.x / tX;
	float fY = tPos.y / tY;

	// ������ ������ RenderPos�� �����Ѵ�.

	POSITION MinimapPos = m_pObj->GetRenderPos();

	CTexture* pMinimap = m_pObj->GetTexture();

	m_tRenderPos.x = MinimapPos.x + fX * pMinimap->GetWidth();
	m_tRenderPos.y = MinimapPos.y + fY * pMinimap->GetHeight();
	 
	// �⺻�� �̰��̴�.
	// ������ ������������ �̴ϸ��� ��Ȯ�� �¾ƶ������� �ʴ´�.
	// ���� offset�� Flag�� �����ؾ� �Ѵ�.

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
