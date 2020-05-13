#include "CSceneManager.h"
#include "../Layer/CLayer.h"
#include "../Scene/CScene.h"

#include "../Scene/CInGameScene.h"
#include "../Scene/CStartScene.h"
#include "../UI/CUIPanel.h"
#include "../Object/CPlayer.h"


// ���� Scene. h

// StartScene (main��)
// InGameScene �ټ�. 3 ~ 4�� ������ ���� �������� Flag�� ó���ϸ� ����� �� �ϴ�.
// EndScene�� ����. �׳� ��ư ó���� ��. 

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

	// UILayer�� �ִ� ������Ʈ�� ZOrder ���� �ٲ۴�.

	list<CObj*>& UILayer = pLayer->GetList();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = UILayer.end();

	bool is = false;

	for (iter = UILayer.begin(); iter != iterEnd; ++iter)
	{
		// �ϴ� �Է��� ���� ������ Panel�� ã�´�.

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


	// ������ ZOrder ���� �������� Sort => ��� �켱������ �ٲ�.

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
			// ó�� �ؾ� �Ǵ� �͵�.

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



	// ���ʺ��� panel Ÿ������ ���縦 �ľ��Ѵ�.
	// �ش� ���� �����ִ����� �÷��̾ ���ؼ� �ľ��Ѵ�.
	// �����ִٸ� ����.
	// �������� �ʴٸ� ���� ���� ã�Ƽ� �ݺ�

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

