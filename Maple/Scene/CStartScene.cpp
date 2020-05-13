#include "CStartScene.h"
#include "../Texture/CTexture.h"
#include "../Core/CSceneManager.h"
#include "../UI/CUIButton.h"
#include "../Layer/CLayer.h"


CStartScene::CStartScene()
{
}


CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	if (!CScene::Init())
	{
		return false;
	}
	// ���ȭ���� �����Ѵ�.

	//m_pScreen = CTexture::SetTexture(L"Main.bmp", _SIZE(0.f, 0.f), 255, 255, 1);

	CLayer* pUILayer = FindLayer("UILayer");

	// UI Layer�� Start Btn�� �߰��Ѵ�.
	CUIButton* pStartBtn = CObj::CreateObj<CUIButton>(pUILayer);

	// ���ø��ε� ���踦 �� ����.
	pStartBtn->SetCallback(this, &CStartScene::StartBtnCallback);

	pStartBtn->SetObjectType(OT_UI);

	POSITION tMiddlePos;
	tMiddlePos.x = RS_WIDTH / 2.f - 75.f;
	tMiddlePos.y = RS_HEIGHT / 2.f - 225.f;
	/*
	pStartBtn->SetObj(tMiddlePos,
		OT_MOUSE, CTexture::SetTexture(L"Btn/GameStartBtn.bmp", _SIZE(150.f, 50.f), 0, 0, 0));
*/

	// ���콺 ���콺 ���콺 ���콺
	return true;
}

void CStartScene::Input(float fDeltaTime)
{
	CScene::Input(fDeltaTime);

}

void CStartScene::Collision(float fDeltaTime)
{
	CScene::Collision(fDeltaTime);

}

void CStartScene::Update(float fDeltaTime)
{
	CScene::Update(fDeltaTime);

}

void CStartScene::Render(HDC hDC, float fDeltaTime)
{
	// Main ȭ��
	//BitBlt(hDC, 0, 0, RS_WIDTH, RS_HEIGHT, m_pScreen->GetDC(), 0, 0, SRCCOPY);

	CScene::Render(hDC, fDeltaTime);
}

void CStartScene::StartBtnCallback(float fTime)
{
	GET_SINGLE(CSceneManager)->InGameScene();
}


