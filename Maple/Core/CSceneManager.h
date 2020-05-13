#pragma once

#include "../Game.h"

class CSceneManager
{
	DECLARE_SINGLE(CSceneManager);

private:
	class CScene* m_pScene;
	class CScene* m_pNextScene;

	HWND m_hWnd;

	bool		  m_bChange;
	int			  m_iChange;

	class CMouse* m_pMouse;

public:

	void SetMouse(class CMouse* pMouse)
	{
		m_pMouse = pMouse;
	}

	class CMouse* GetMouse() const
	{
		return m_pMouse;
	}

	// �� �������� ������, i���ڸ� ����ø���, �� �տ� ����� �� ������ sort ���ش�.
	void SetChange(int i)
	{
		m_bChange = true;
		m_iChange = i;
	}

	// ��ȭ�� �Ͼ�� ��츸.
	void UISort();

	void ESCKey();


	// UILayer�� DefaultLayer�� ������ �� �ֵ��� �Ѵ�.

	list<class CObj*>& GetList(const string& strLayerName);



	HWND GetHandle() const
	{
		return m_hWnd;
	}

	bool Init(HWND hWnd);
	void Input(float fDeltaTime);
	void Collision(float fDeltaTime);
	CHANGE_SCENE Update(float fDeltaTime);
	void Render(HDC hDC, float fDeltaTime);
	
private:
	CHANGE_SCENE ChangeScene();


public:
	
	void InGameScene();

	class CScene* GetCurrentScene() const
	{
		return m_pScene;
	}

	class CPlayer* GetPlayer();

	class CTexture* GetTexture();


	list<class CObj*>& GetObjList(const string& strTag);

	static bool UILayerSort(class CObj* pSrc, class CObj* pDest);

public:
	template <typename T>
	T* CreateScene(SCENE_CLIP eClip)
	{
		T* pScene = new T;

		switch (eClip)
		{
		case SC_CURRENT:
			SAFE_DELETE(m_pScene);
			m_pScene = pScene;

			break;

		case SC_NEXT:
			SAFE_DELETE(m_pNextScene);
			m_pNextScene = pScene;
			break;
		}

		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return NULL;
		}

		return pScene;
	}
	
};

