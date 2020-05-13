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
	// 배경화면을 지정한다.

	//m_pScreen = CTexture::SetTexture(L"Main.bmp", _SIZE(0.f, 0.f), 255, 255, 1);

	CLayer* pUILayer = FindLayer("UILayer");

	// UI Layer에 Start Btn을 추가한다.
	CUIButton* pStartBtn = CObj::CreateObj<CUIButton>(pUILayer);

	// 템플릿인데 꺽쇠를 안 쓴다.
	pStartBtn->SetCallback(this, &CStartScene::StartBtnCallback);

	pStartBtn->SetObjectType(OT_UI);

	POSITION tMiddlePos;
	tMiddlePos.x = RS_WIDTH / 2.f - 75.f;
	tMiddlePos.y = RS_HEIGHT / 2.f - 225.f;
	/*
	pStartBtn->SetObj(tMiddlePos,
		OT_MOUSE, CTexture::SetTexture(L"Btn/GameStartBtn.bmp", _SIZE(150.f, 50.f), 0, 0, 0));
*/

	// 마우스 마우스 마우스 마우스
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
	// Main 화면
	//BitBlt(hDC, 0, 0, RS_WIDTH, RS_HEIGHT, m_pScreen->GetDC(), 0, 0, SRCCOPY);

	CScene::Render(hDC, fDeltaTime);
}

void CStartScene::StartBtnCallback(float fTime)
{
	GET_SINGLE(CSceneManager)->InGameScene();
}


