#include "CLayer.h"
#include "../Object/CObj.h"
#include "../Core/CCollisionManager.h"

// Panel

#include "../UI/CUIPanel.h"

CLayer::CLayer()
{
}


CLayer::~CLayer()
{
	Safe_Release_VecList(m_ObjList);
}


bool CLayer::Init()
{

	return true;
}

void CLayer::Input(float fDeltaTime)
{


	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{

		

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Input(fDeltaTime);
		++iter;
	}



}

void CLayer::Collision(float fDeltaTime) 
{

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Collision(fDeltaTime);

		// 죽지 않은 상태인 친구들만 ColliderList에 넣는다
		GET_SINGLE(CCollisionManager)->AddObj((*iter));
		++iter;
	}
	
}

void CLayer::Update(float fDeltaTime)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Update(fDeltaTime);
		++iter;
	}

}

void CLayer::Render(HDC hDC, float fDeltaTime)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	int cnt = 0;

	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{

		//// Layer 잘못되었나 확인. 
		//if (GetTag() == "UILayer")
		//{
		//	wchar_t str[50];
		//	swprintf_s(str, L"ZOrder : %d", ((CUIPanel*)(*iter))->GetZOrder());

		//	TextOut(hDC, 300, 40 + 20 * cnt++, str, lstrlen(str));
		//}




		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Render(hDC, fDeltaTime);
		++iter;
	}

}

bool CLayer::LayerSort(CLayer * pL1, CLayer * pL2)
{
	return pL1->GetZOrder() < pL2->GetZOrder();
}
