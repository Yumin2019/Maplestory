#include "CSceneManager.h"
#include "../Layer/CLayer.h"
#include "../Scene/CScene.h"

#include "../Scene/CInGameScene.h"
#include "../Scene/CStartScene.h"
#include "../UI/CUIPanel.h"
#include "../Object/CPlayer.h"


// 만들 Scene. h

// StartScene (main용)
// InGameScene 다수. 3 ~ 4개 정도를 만들 예정으로 Flag로 처리하면 충분할 듯 하다.
// EndScene은 없다. 그냥 버튼 처리로 끝. 

DEFINITION_SINGLE(CSceneManager);

CSceneManager::CSceneManager() :
	m_pScene(NULL),
	m_pNextScene(NULL),
	m_bChange(false),
	m_iChange(0),
	m_pMouse(NULL)
{
}


CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pNextScene);
}

list<class CObj*>& CSceneManager::GetList(const string & strLayerName)
{
	return m_pScene->FindLayer(strLayerName)->GetList();
}

bool CSceneManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	CScene* pScene = CreateScene<CInGameScene>(SC_CURRENT);
	//CreateScene<CStartScene>(SC_CURRENT);

	return true;
}

void CSceneManager::Input(float fDeltaTime)
{
	m_pScene->Input(fDeltaTime);
}

void CSceneManager::Collision(float fDeltaTime)
{
	m_pScene->Collision(fDeltaTime);

}

CHANGE_SCENE CSceneManager::Update(float fDeltaTime)
{
	m_pScene->Update(fDeltaTime);

	return ChangeScene();

}

void CSceneManager::Render(HDC hDC, float fDeltaTime)
{
	m_pScene->Render(hDC, fDeltaTime);


	if (m_bChange)
	{
		UISort();

		m_bChange = false;
	}
}

CHANGE_SCENE CSceneManager::ChangeScene()
{
	if (m_pNextScene)
	{
		SAFE_DELETE(m_pScene);
		m_pScene = m_pNextScene;
		m_pNextScene = NULL;

		return CS_CHANGE;
	}

	return CS_NONE;
}

void CSceneManager::UISort()
{
	CLayer* pLayer = m_pScene->FindLayer("UILayer");

	// UILayer에 있는 오브젝트의 ZOrder 값을 바꾼다.

	list<CObj*>& UILayer = pLayer->GetList();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = UILayer.end();

	bool is = false;

	for (iter = UILayer.begin(); iter != iterEnd; ++iter)
	{
		// 일단 입력한 값과 동일한 Panel을 찾는다.

		if (m_iChange == ((CUIPanel*)(*iter))->GetZOrder())
		{
			is = true;
			((CUIPanel*)(*iter))->SetZOrder(UI_MAX_IDX);
		}

		else if (is)
		{
			((CUIPanel*)(*iter))->SetZOrder(((CUIPanel*)(*iter))->GetZOrder() - 1);

		}

	}


	// 셋팅한 ZOrder 값을 바탕으로 Sort => 출력 우선순위가 바뀜.

	pLayer->GetList().sort(CSceneManager::UILayerSort);
}

void CSceneManager::ESCKey()
{
	CPlayer* m_pPlayer = GetPlayer();

	CLayer* pLayer = m_pScene->FindLayer("UILayer");

	list<CObj*>& UILayer = pLayer->GetList();

	list<CObj*>::iterator iter = UILayer.end();
	list<CObj*>::iterator iterEnd = UILayer.begin();

	--iter;

	for (; iter != iterEnd; --iter)
	{
		CUIPanel* pPanel = (CUIPanel*)(*iter);

		switch (pPanel->GetPanelType())
		{
			// 처리 해야 되는 것들.

			case PT_MINIMAP_MAIN :
			case PT_MINIMAP_MUSH1 :
			case PT_MINIMAP_MUSH2 :
			case PT_MINIMAP_TRAININGROOM:
			case PT_MINIMAP_GOLEM  :
			case PT_MINIMAPBORDER :
			case PT_MINIMAP_CIRCLE_MAIN	 :
			case PT_MINIMAP_CIRCLE_MUSH1 :
			case PT_MINIMAP_CIRCLE_MUSH2 :
			case PT_MINIMAP_CIRCLE_GOLEM :
			case PT_MINIMAP_CIRCLE_TRAININGROOM	  :

				if (m_pPlayer->GetMinimap())
				{
					m_pPlayer->SetMinimap(false);
					return;
				}

				break;

			case PT_SKILL						  :

				if (m_pPlayer->GetSkill())
				{
					m_pPlayer->SetSkill(false);
					return;

				}

				break;

			case PT_ABILITY						  :

				if (m_pPlayer->GetAbility())
				{
					m_pPlayer->SetAbility(false);
					return;

				}

				break;

			case PT_INVENTORY					  :

				if (m_pPlayer->GetInventory())
				{
					m_pPlayer->SetInventory(false);
					return;

				}

				break;

			case PT_SHOP						  :

				if (m_pPlayer->GetShop())
				{
					m_pPlayer->SetShop(false);
					return;

				}

				break;
		}


	}



	// 끝쪽부터 panel 타입으로 소재를 파악한다.
	// 해당 놈이 켜져있는지는 플레이어를 통해서 파악한다.
	// 켜져있다면 끈다.
	// 켜져있지 않다면 다음 놈을 찾아서 반복

}

void CSceneManager::InGameScene()
{
	CreateScene<CInGameScene>(SC_NEXT);
}

CPlayer * CSceneManager::GetPlayer()
{
	return ((CInGameScene*)m_pScene)->GetPlayer();
}

CTexture * CSceneManager::GetTexture()
{
	return	m_pScene->GetTexture();
}

list<class CObj*>& CSceneManager::GetObjList(const string & strTag)
{
	return m_pScene->FindLayer(strTag)->GetList();
}

bool CSceneManager::UILayerSort(CObj * pSrc, CObj * pDest)
{
	return ((CUIPanel*)pSrc)->GetZOrder() < ((CUIPanel*)pDest)->GetZOrder();
}

