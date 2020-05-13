#include "CScene.h"
#include "../Layer/CLayer.h"
#include "../Texture/CTexture.h"
#include "../Core/CSceneManager.h"

#include "../Core/CInput.h"
#include "../Core/CPencil.h"
#include "../UI/CMouse.h"


CScene::CScene() :
	m_pScreen(NULL),
	m_eSceneClip(SC_NONE)
{

}


CScene::~CScene()
{
	Safe_Delete_VecList(m_LayerList);
	SAFE_RELEASE(m_pScreen);
}


bool CScene::MouseInit()
{
	// UI Layer�� ���콺�� �ִ´�.
	CMouse* pMouse = new CMouse;

	// ���콺�� �浹ü�� �ִ´�.

	pMouse->Init();

	FindLayer("UILayer")->GetList().push_back(pMouse);

	GET_SINGLE(CSceneManager)->SetMouse(pMouse);

	return true;
}

bool CScene::Init()
{
	// ��� Scene���� ó������ �͵�

	// Layer�� �߰��Ѵ�.
	CreateLayer("UILayer", INT_MAX);
	// �� ��ư & UI �� ��� Scene���� ó��.

	
	
	
	return true;
}

void CScene::Input(float fDeltaTime)
{
	
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fDeltaTime);
	}
}

void CScene::Collision(float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Collision(fDeltaTime);
	}
}

void CScene::Update(float fDeltaTime)
{
	

	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

void CScene::Render(HDC hDC, float fDeltaTime)
{


	// ���ȭ���� Scene����

	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}

}

CLayer * CScene::FindLayer(const string & strTag)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
			return (*iter);
	}

	return NULL;
}

CLayer * CScene::CreateLayer(const string & strTag, int ZOrder)
{
	CLayer* pLayer = new CLayer;

	pLayer->SetTag(strTag);

	pLayer->SetZOrder(ZOrder);

	m_LayerList.push_back(pLayer);

	if (m_LayerList.size() >= 2)
	{
		m_LayerList.sort(CLayer::LayerSort);
		// sort
	}

	return pLayer;
}
