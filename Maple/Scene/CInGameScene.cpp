#include "CInGameScene.h"
#include "../Texture/CTexture.h"
#include "../Layer/CLayer.h"
#include "../Object/CPlayer.h"
#include "../Object/CStaticObj.h"
#include "../UI/CUIPanel.h"

#include "../Core/CInput.h"
#include "../Core/CPencil.h"
#include "../Core/CSceneManager.h"
#include "../Object/CStage.h"
#include "../Collision/CPixel.h"
#include "../Collision/CShape.h"
#include "../Object/CNormalMonster.h"
#include "../Core/CCreateManager.h"

// UI

#include "../UI/CAbilityPanel.h" 
#include "../UI/CInventoryPanel.h"
#include "../UI/CQuickSlotPanel.h"
#include "../UI/CSkillPanel.h"
#include "../UI/CMinimap.h"
#include "../UI/CShopUI.h"
#include "../UI/CScroll.h"

// ITEM

#include "../Object/CItem.h"

float CInGameScene::l;
float CInGameScene::t;
bool CInGameScene::m_bCollider;

CInGameScene::CInGameScene() :
	m_pPlayer(NULL),
	m_pPixelScreen(NULL)
{
}


CInGameScene::~CInGameScene()
{
	SAFE_RELEASE(m_pPixelScreen);
}



bool CInGameScene::MainInit(CStage * pMap, list<class CObj*>& ObjList)
{
	// 픽셀 정보 추가
	CPixel *pPixel = new CPixel;
	
	if (!pPixel->InitColliderPixel("MainLoad.bmp"))
	{
		SAFE_RELEASE(pPixel);
		return false;
	}

	pMap->AddCollider(pPixel);

	// 포탈추가 

	POINT tPoint;
	tPoint.x = 0;
	tPoint.y = 0;

	// 1
	CShape* pPotal = new CShape;

	if (!pPotal->InitColliderShape(_SIZE(16.f, 16.f), tPoint, ST_RECT_PORTAL))
	{
		SAFE_RELEASE(pPotal);
		return false;
	}

	pPotal->SetPos(135.f, 828.f);

	pMap->AddCollider(pPotal);

	// 2
	pPotal = new CShape;

	if (!pPotal->InitColliderShape(_SIZE(16.f, 16.f), tPoint, ST_RECT_PORTAL))
	{
		SAFE_RELEASE(pPotal);
		return false;
	}

	pPotal->SetPos(635.f, 768.f);

	pMap->AddCollider(pPotal);

	// 3
	pPotal = new CShape;

	if (!pPotal->InitColliderShape(_SIZE(16.f, 16.f), tPoint, ST_RECT_PORTAL))
	{
		SAFE_RELEASE(pPotal);
		return false;
	}

	pPotal->SetPos(1244.f, 828.f);

	pMap->AddCollider(pPotal);

	// NPC
	CShape* pNpc = new CShape;

	if (!pNpc->InitColliderShape(_SIZE(43.f, 65.f), tPoint, ST_RECT_NPC))
	{
		SAFE_RELEASE(pNpc);
		return false;
	}

	pNpc->SetPos(532.f, 743.f);

	pMap->AddCollider(pNpc);

  
	 // 몬스터 추가  순서상 문제점 생기고 있음. ㅋㅋㅋ 수정바람.

 	// 빨간 달팽이
	//CNormalMonster* pRedSnail = FIND(CNormalMonster)->FindObj("BlueSnail")->Clone();

	//pRedSnail->SetPos(200.f, 350.f);

	//pRedSnail->Init();

	////pRedSnail->SetSpeed(0.f);

	//ObjList.push_front(pRedSnail);


	CNormalMonster* pRedSnail = FIND(CNormalMonster)->FindObj("RedSnail")->Clone();

	pRedSnail->SetPos(400.f, 350.f);
 
	pRedSnail->Init();

	pRedSnail->SetSpeed(0.f);

	ObjList.push_front(pRedSnail);



	// 골렘

	//CNormalMonster* pGol = FIND(CNormalMonster)->FindObj("Golem")->Clone();

	//pGol->SetPos(200.f, 250.f);

	//pGol->Init();

	//ObjList.push_front(pGol);


	// 주황 버섯

	//CNormalMonster* pMush = FIND(CNormalMonster)->FindObj("OrangeMushroom")->Clone();

	//pMush->SetPos(730.f, 302.f);
	//
	//pMush->Init();

	//ObjList.push_back(pMush);

	//
	//// 뿔버섯 -> 이미지 크기 달라서 안 나옴... ㅠㅠ
	//pMush = FIND(CNormalMonster)->FindObj("GreenMushroom")->Clone();

	//pMush->SetPos(840.f, 442.f);

	//pMush->Init();

	//ObjList.push_back(pMush);

	//// 파란 달팽이

	//pRedSnail = FIND(CNormalMonster)->FindObj("BlueSnail")->Clone();

	//pRedSnail->SetPos(1100.f, 442.f);

	//pRedSnail->Init();

	//ObjList.push_back(pRedSnail);


	/*
	CNormalMonster* pTree = FIND(CNormalMonster)->FindObj("Tree")->Clone();

	pTree->SetPos(938.f, 527.f);

	pTree->Init();

	ObjList.push_back(pTree);*/
	//938 527

	// 미니맵Border 위에 출력할 맵은 일단 하드코딩 안 하고, 보더의 위치에 따라서 상대적으로 잡을 것이다.
	// 이 값은 map 정보에 따라서 조금 달라질 것이다.
	// 아무튼 나중에 마우스 입력을 받아서 처리할 것이다.
	// 이거는 update를 통해서 위치를 셋팅하도록 한다.
	// 입력은 안 받지만. ㅎ

	
	
	

	return true;
}

bool CInGameScene::Mushroom1Init(CStage * pMap, list<class CObj*>& ObjList)
{
	return true;
}

bool CInGameScene::Mushroom2Init(CStage * pMap, list<class CObj*>& ObjList)
{
	return true;
}

bool CInGameScene::GolemInit(CStage * pMap, list<class CObj*>& ObjList)
{
	return true;
}

bool CInGameScene::TrainingRoomInit(CStage * pMap, list<class CObj*>& ObjList)
{
	return true;
}



bool CInGameScene::Init()
{
	if (!CScene::Init())
		return false;


	CLayer* pDefaultLayer = CreateLayer("DefaultLayer", 1);

	

	// 플레이어를 생성하여 넣는다.

	m_pPlayer = CObj::CreateObj<CPlayer>(pDefaultLayer);

	




	// 플레이어 Hit 

	// 픽셀 충돌 정보에 이용할 친구를 생성한다.

	// 일단 Layer에 데입
	CStage* pMap = CObj::CreateObj<CStage>(pDefaultLayer); 

	pMap->SetMapType(MT_MAIN);

	list<CObj*>& ObjList = pDefaultLayer->GetList();

	bool InitOk;

	switch (pMap->GetMapType())
	{
	case	 MT_MAIN:		 InitOk = MainInit(pMap, ObjList); break;
	case 	MT_MUSHROOM1:	 InitOk = Mushroom1Init(pMap, ObjList); break;
	case 	MT_MUSHROOM2:	 InitOk = Mushroom2Init(pMap, ObjList); break;
	case 	MT_GOLEM:		 InitOk = GolemInit(pMap, ObjList); break;
	case 	MT_TRAININGROOM: InitOk = TrainingRoomInit(pMap, ObjList); break;
	}

	if (!InitOk)
		return false;

	// UI 초기화
	if (!UIInit(pMap->GetMapType()))
		return false;

	// 마우스는 데입.
	if (!MouseInit())
		return false;

	// 스크린 출력

	m_pScreen = CTexture::SetTexture(L"Map/Main.bmp", _SIZE(0.f, 0.f), 255, 255, 1);

	m_pPixelScreen = CTexture::SetTexture(L"Map/MainLoad.bmp", _SIZE(0.f, 0.f), 255, 255, 255);


	//CLayer* pUILayer = FindLayer("UILayer");

	return true;
}

void CInGameScene::Input(float fDeltaTime)
{
	CScene::Input(fDeltaTime);


	

	if (KEYDOWN("Collider"))
	{
		m_bCollider = !m_bCollider;
	}

	
			


		




}

void CInGameScene::Collision(float fDeltaTime)
{
	CScene::Collision(fDeltaTime);

}

void CInGameScene::Update(float fDeltaTime)
{
	CScene::Update(fDeltaTime);

	// 플레이어 위치에 따른 Update

	POSITION m_tCenter = m_pPlayer->GetCenter();

	// l t만 제대로 지정하면 된다.

	// x 5 : 5 y 5 : 5
	// l = m_tCenter.x - RS_WIDTH / 2.f;
	//float r = m_tCenter.x + RS_WIDTH / 2.f;

	// t = m_tCenter.y - RS_HEIGHT / 2.f;
	//float b = m_tCenter.y + RS_HEIGHT / 2.f;


	// x 5 : 5  바텀의 비율을 높여서 점프하여도 스크롤을 ... 막는다.
	l = m_tCenter.x - RS_WIDTH / 2.f;
	float r = m_tCenter.x + RS_WIDTH / 2.f;

	t = m_tCenter.y - RS_HEIGHT * 0.25f;
	float b = m_tCenter.y + RS_HEIGHT * 0.75f;

	if (l < 0)
	{
		l = 0;
		r = RS_WIDTH;
	}

	if (r > m_pScreen->GetWidth())
	{
		r = m_pScreen->GetWidth();
		l = r - RS_WIDTH;
	}

	if (t < 0)
	{
		t = 0;
		b = t + RS_HEIGHT;
	}

	if (b > m_pScreen->GetHeight())
	{
		b = m_pScreen->GetHeight();
		t = b - RS_HEIGHT;
	}

}

void CInGameScene::Render(HDC hDC, float fDeltaTime)
{
	BitBlt(hDC, 0, 0, RS_WIDTH, RS_HEIGHT, m_pScreen->GetDC(), l, t, SRCCOPY);


	//if (m_bCollider)
	//{
	//	TransparentBlt(hDC, 0, 0, RS_WIDTH, RS_HEIGHT, m_pPixelScreen->GetDC(), l, t, RS_WIDTH, RS_HEIGHT, m_pPixelScreen->GetColor());
	//}

	CScene::Render(hDC, fDeltaTime);



	

	// 출력이후에는 Count  = 0;
	CShape::m_iPortal = 0;
	CShape::m_iRect = 0;
}

bool CInGameScene::UIInit(MAP_TYPE eMap)
{
	CLayer* pUILayer = FindLayer("UILayer");

	list<CObj*>& ObjList = pUILayer->GetList();

	// HPMP UI를 만든다.

	CUIPanel* pPanel = new CUIPanel;
												// 10px은 경험치바. 기본적으로 50 띄우고 시작하자.
	pPanel->SetPanel(GetPoint(RS_WIDTH / 2 - 85, 720 - 64 - 50 - 10), CTexture::SetTexture(L"UI/BarUI/BarUI.bmp", POSITION(172.f, 66.f),
		0, 128, 0), PT_HPMPBORDER, NULL);

	ObjList.push_back(pPanel);

	// HPBar 
	pPanel = new CUIPanel; 

	pPanel->SetPanel(GetPoint(BarX, HPBarY),
		CTexture::SetTexture(L"UI/BarUI/HPBar.bmp", POSITION(141.f, 13.f),
			0, 128, 0), PT_HPBAR, m_pPlayer);

	ObjList.push_back(pPanel);

	// MPBar

	pPanel = new CUIPanel;

	pPanel->SetPanel(GetPoint(BarX, MPBarY),
		CTexture::SetTexture(L"UI/BarUI/MPBar.bmp", POSITION(141.f, 13.f),
			0, 128, 0), PT_MPBAR, m_pPlayer);

	ObjList.push_back(pPanel);

	// EXP UI

	pPanel = new CUIPanel;

	pPanel->SetPanel(GetPoint(140, 720 - 50),
		CTexture::SetTexture(L"UI/BarUI/ExpUI.bmp", POSITION(1000.f, 13.f),
			0, 128, 0), PT_EXPBORDER, NULL);

	ObjList.push_back(pPanel);

	  
	// Exp Bar
	pPanel = new CUIPanel;

	pPanel->SetPanel(GetPoint(140 + 24, 720 - 50 + 2),
		CTexture::SetTexture(L"UI/BarUI/ExpBar.bmp", POSITION(974.f, 9.f),
			0, 128, 0), PT_EXPBAR, m_pPlayer);

	ObjList.push_back(pPanel);

	// Name

	pPanel = new CUIPanel;

	pPanel->SetPanel(GetPoint(0, 0),
		CTexture::SetTexture(L"UI/Name.bmp", POSITION(51.f, 15.f),
			0, 128, 0), PT_NAME, m_pPlayer);

	ObjList.push_back(pPanel);


	// 스킬 슬롯
	CQuickSlotPanel* pSlot = new CQuickSlotPanel;

	pSlot->SetPanel(GetPoint(RS_WIDTH / 2 + 95, 720 - 64 - 50 - 10 - 13),
		CTexture::SetTexture(L"UI/QuickSlot.bmp", POSITION(151.f, 80.f),
			0, 128, 0), PT_QUICKSLOT, NULL);

	pSlot->SetBlend(true);
	pSlot->SetiBlend(252);

	ObjList.push_back(pSlot);

	// 스킬 슬롯 알파벳은 그냥 Panel
	pPanel = new CUIPanel;

	pPanel->SetPanel(GetPoint(RS_WIDTH / 2 + 95, 720 - 64 - 50 - 10 - 13),
		CTexture::SetTexture(L"UI/QuickSlotAlpha.bmp", POSITION(151.f, 80.f),
			0, 128, 0), PT_QUICKSLOT_ALPHA, NULL);

	ObjList.push_back(pPanel);



	// Minimap 보더
	CMinimap* pMini = new CMinimap;

	pMini->SetPanel(GetPoint(30, 20), // y축 나중에 수정
		CTexture::SetTexture(L"Minimap/MinimapUI.bmp", POSITION(181, 120.f),
			0, 128, 0), PT_MINIMAPBORDER, m_pPlayer);

	pMini->SetZOrder(0);

	ObjList.push_back(pMini);

	// 미니맵.
	CUIPanel* pUIPanel = new CUIPanel;

	switch (eMap)
	{
	case	 MT_MAIN:	
		pUIPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Main.bmp", POSITION(131.f, 85.f),
				255, 255, 255), PT_MINIMAP_MAIN, pMini);


		ObjList.push_back(pUIPanel);

		// 미니맵 작은 원
		pPanel = new CUIPanel;

		pPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Circle.bmp", POSITION(8.f, 8.f),
				0, 128, 0), PT_MINIMAP_CIRCLE_MAIN, pUIPanel);

		ObjList.push_back(pPanel);

		break;

	case 	MT_TRAININGROOM:
		pUIPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/TrainingRoom.bmp", POSITION(130.f, 85.f),
				255, 255, 255), PT_MINIMAP_TRAININGROOM, pMini);

		ObjList.push_back(pUIPanel);


		pPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Circle.bmp", POSITION(8.f, 8.f),
				0, 128, 0), PT_MINIMAP_CIRCLE_TRAININGROOM, pUIPanel);

		ObjList.push_back(pPanel);

		break;
		

	case 	MT_MUSHROOM1:	
		pUIPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Mushroom1.bmp", POSITION(139.f, 60.f),
				255, 255, 255), PT_MINIMAP_MUSH1, pMini);

		ObjList.push_back(pUIPanel);

		pPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Circle.bmp", POSITION(8.f, 8.f),
				0, 128, 0), PT_MINIMAP_CIRCLE_MUSH1, pUIPanel);

		ObjList.push_back(pPanel);

		break;


	case 	MT_MUSHROOM2:	
		pUIPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Mushroom2.bmp", POSITION(132.f, 69.f),
				255, 255, 255), PT_MINIMAP_MUSH2, pMini);

		ObjList.push_back(pUIPanel);


		pPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Circle.bmp", POSITION(8.f, 8.f),
				0, 128, 0), PT_MINIMAP_CIRCLE_MUSH2, pUIPanel);

		ObjList.push_back(pPanel);

		break;


	case 	MT_GOLEM:		
		pUIPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Golem.bmp", POSITION(132.f, 69.f),
				255, 255, 255), PT_MINIMAP_GOLEM, pMini);

		ObjList.push_back(pUIPanel);


		pPanel->SetPanel(GetPoint(0, 0),
			CTexture::SetTexture(L"Minimap/Circle.bmp", POSITION(8.f, 8.f),
				0, 128, 0), PT_MINIMAP_CIRCLE_GOLEM, pUIPanel);

		ObjList.push_back(pPanel);

		break;
	

	}

	pUIPanel->SetZOrder(0);
	pPanel->SetZOrder(0);



	// 인벤토리
	CInventoryPanel* pInven = new CInventoryPanel;

	pInven->SetPanel(GetPoint(RS_WIDTH / 2 - 85, RS_HEIGHT / 2 - 150),
		CTexture::SetTexture(L"UI/Inventory.bmp", POSITION(172.f, 294.f),
			0, 128, 0), PT_INVENTORY, m_pPlayer);

	pInven->Init();

	pInven->SetZOrder(1);

	ObjList.push_back(pInven);

	// 스킬 스탯 UI
	CSkillPanel* pSkill = new CSkillPanel; // 여기

	pSkill->SetPanel(GetPoint(RS_WIDTH / 2 + 300, RS_HEIGHT / 2 - 125),
		CTexture::SetTexture(L"UI/Skill.bmp", POSITION(158.f, 259.f),
			0, 128, 0), PT_SKILL, m_pPlayer);

	pSkill->SetZOrder(2);

	ObjList.push_back(pSkill);

	// 능력치

	CAbilityPanel* pAbil = new CAbilityPanel;

	pAbil->SetPanel(GetPoint(RS_WIDTH / 2 - 300, RS_HEIGHT / 2 - 150),
		CTexture::SetTexture(L"UI/Ability.bmp", POSITION(214.f, 297.f),
			0, 128, 0), PT_ABILITY, m_pPlayer);

	pAbil->SetZOrder(3);

	ObjList.push_back(pAbil);

	// SHOP

	CShopUI* pShop = new CShopUI;

	pShop->SetPanel(GetPoint(RS_WIDTH / 2 - 250, RS_HEIGHT / 2 - 250),
		CTexture::SetTexture(L"UI/Shop.bmp", POSITION(511.f, 507.f), 0, 128, 0), PT_SHOP, m_pPlayer);
	
	pShop->SetInven(pInven);

	pShop->Init();

	pShop->SetZOrder(4);

	ObjList.push_back(pShop);

	// SHop Scroll 

	CScroll* pScroll = new CScroll;

	pScroll->SetPanel(GetPoint(0, 0),
		CTexture::SetTexture(L"UI/ScrollBar.bmp", POSITION(9.f, 26.f), 0, 128, 0), PT_SCROLL, m_pPlayer);

	pScroll->SetZOrder(4);

	pScroll->SetShopPointer(pShop);

	ObjList.push_back(pScroll);


	// UI Setting
	m_pPlayer->SetUI(pShop, pSkill, pMini, pInven, pAbil);


	return true;
}
