#include "CCreateManager.h"
#include "../Texture/CTexture.h"


#include "../Object/CNormalMonster.h"
#include "../Animation/CAnimation.h"
#include "../Collision/CShape.h"

#include "../Object/CEffect.h"
#include "../UI/CUIPanel.h"
#include "../Object/CItem.h"

DEFINITION_SINGLE(CCreateManager);

CCreateManager::CCreateManager()
{

}


CCreateManager::~CCreateManager()
{
	Safe_Release_Map(m_mapObj);
}

bool CCreateManager::Init(HINSTANCE hInst, HDC hDC)
{

	m_hInst = hInst;
	m_hDC = hDC;


	// 프로토타입을 정의 하는 함수이다.

	/* normal 몬스터는 die hit move stand 모션만 가지고 있는 몬스터이다.
	각 몬스터마다 정보가 다르고 애니메이션이 다르지만, 동일한 애니메이션의 이름을 사용하고 있으며 애니메이션의
	offset 또한 각각 따로 지정할 수 있다 (tag 이용) 

	죽을 때 나오는 아이템 또한 tag를 이용하면 간편하게 생성할 수 있다.
	*/

	// 빨간 달팽이 normal
	if (!RedSnailCreate())
		return false;

	// 파란 달팽이 normal
	if (!BlueSnailCreate())
		return false;


	// 뿔 버섯 normal
	if (!BlueMushroomCreate())
		return false;

	// 초록 버섯 normal
	if (!GreenMushroomCreate())
		return false;

	// 주황 버섯 
	if (!OrangeMushroomCreate())
		return false;

	// 머쉬맘
	if (!MushMomCreate())
		return false;


	// 다크 슬럼프 normal
	if (!TreeCreate())
		return false;

	// 골렘 
	if (!GolemCreate())
		return false;

	// SKILL
	if (!PlayerSkillEffect())
		return false;

	// NORMAL ATTACK
	if (!PlayerNormalAttackEffect())
		return false;
	
	// MonsterHP UI
	if (!MonsterHPUICreate())
		return false;
	
	// ITEM
	if (!ItemCreate())
		return false;

	return true;
}

CObj * CCreateManager::FindObj(const string & strKey)
{
	unordered_map<string, CObj*>::iterator iter = m_mapObj.find(strKey);

	if (iter == m_mapObj.end())
		return NULL;

	return iter->second; 
}

void CCreateManager::InsertObj(const string & strTag, CObj * pObj)
{
	m_mapObj.insert(make_pair(strTag, pObj));
}


CMonster * CCreateManager::CreateMonster(CMonster * pMonster, const string& strMonsterTag, MONSTER_NAME eName, _SIZE tSize, 
	int iLevel, int iHP, int iMP, int iEXP, int iGold, 
	int iAttackMin, int iAttackMax, float fSpeed)
{
	pMonster->SetMonsterName(eName);
	pMonster->SetSize(tSize);
	pMonster->SetInfo(iLevel, iHP, iMP, iEXP, iGold, iAttackMin, iAttackMax, fSpeed);
	pMonster->SetObj(OT_MONSTER, NULL);

	m_mapObj.insert(make_pair(strMonsterTag, pMonster));

	return pMonster;
}

CTexture * CCreateManager::CreateTexture(const wstring & strFileName, _SIZE m_tRenderSize, unsigned int r, unsigned int g, unsigned int b, CObj * pObj)
{
	CTexture* pTexture = CTexture::SetTexture(strFileName, m_tRenderSize, r, g, b);

	pObj->SetTexture(pTexture);
	
	return pTexture;
}

bool CCreateManager::RedSnailCreate()
{

	CNormalMonster* pRed = new CNormalMonster;

	CreateMonster(pRed, "RedSnail", RED_SNAIL,
		_SIZE(35.f, 34.f), 3, 25, 0, 5, 10, 4, 6, 80.f);

	CAnimation* pRedAni = pRed->CreateAnimation("RedSnail");

	// Stand
	pRedAni->AddFrameClip(L"Monster/RedSnail/Stand/Left/Stand", "StandLeft", AO_LOOP,
		0.f, 2.f, 1, _SIZE(35.f, 34.f), 0, 0, 35.f, 34.f, 163, 73, 164);

	pRedAni->AddFrameClip(L"Monster/RedSnail/Stand/Right/Stand", "StandRight", AO_LOOP,
		0.f, 2.f, 1, _SIZE(35.f, 34.f), 0, 0, 35.f, 34.f, 163, 73, 164);

	// Move
	pRedAni->AddFrameClip(L"Monster/RedSnail/Move/Left/Move", "MoveLeft", AO_LOOP,
		0.f, 0.7f, 4, _SIZE(44.f, 34.f), 0, 0, 44.f, 34.f, 163, 73, 164);

	pRedAni->AddFrameClip(L"Monster/RedSnail/Move/Right/Move", "MoveRight", AO_LOOP,
		0.f, 0.7f, 4, _SIZE(44.f, 34.f), 0, 0, 44.f, 34.f, 163, 73, 164);

	// Hit
	pRedAni->AddFrameClip(L"Monster/RedSnail/Hit/Left/Hit", "HitLeft", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(41.f, 39.f), 0, 0, 41.f, 39.f, 163, 73, 164);

	pRedAni->AddFrameClip(L"Monster/RedSnail/Hit/Right/Hit", "HitRight", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(41.f, 39.f), 0, 0, 41.f, 39.f, 163, 73, 164);

	// Die
	pRedAni->AddFrameClip(L"Monster/RedSnail/Die/Left/Die", "DieLeft", AO_ONCE_DESTROY,
		0.f, 2.f, 6, _SIZE(56.f, 32.f), 0, 0, 56.f, 32.f, 163, 73, 164);

	pRedAni->AddFrameClip(L"Monster/RedSnail/Die/Right/Die", "DieRight", AO_ONCE_DESTROY,
		0.f, 2.f, 6, _SIZE(56.f, 32.f), 0, 0, 56.f, 32.f, 163, 73, 164);


	// COLLIDER는 따로.




	return true;
}

bool CCreateManager::BlueSnailCreate()
{

	CNormalMonster* pBlue = new CNormalMonster;

	CreateMonster(pBlue, "BlueSnail", BLUE_SNAIL,
		_SIZE(35.f, 34.f), 2, 20, 0, 4, 10, 3, 5, 80.f);

	CAnimation* pBlueAni = pBlue->CreateAnimation("BlueSnail");

	// Stand
	pBlueAni->AddFrameClip(L"Monster/BlueSnail/Stand/Left/Stand", "StandLeft", AO_LOOP,
		0.f, 2.f, 1, _SIZE(35.f, 34.f), 0, 0, 35.f, 34.f, 163, 73, 164);

	pBlueAni->AddFrameClip(L"Monster/BlueSnail/Stand/Right/Stand", "StandRight", AO_LOOP,
		0.f, 2.f, 1, _SIZE(35.f, 34.f), 0, 0, 35.f, 34.f, 163, 73, 164);

	// Move
	pBlueAni->AddFrameClip(L"Monster/BlueSnail/Move/Left/Move", "MoveLeft", AO_LOOP,
		0.f, 0.7f, 4, _SIZE(44.f, 34.f), 0, 0, 44.f, 34.f, 163, 73, 164);

	pBlueAni->AddFrameClip(L"Monster/BlueSnail/Move/Right/Move", "MoveRight", AO_LOOP,
		0.f, 0.7f, 4, _SIZE(44.f, 34.f), 0, 0, 44.f, 34.f, 163, 73, 164);

	// Hit
	pBlueAni->AddFrameClip(L"Monster/BlueSnail/Hit/Left/Hit", "HitLeft", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(41.f, 39.f), 0, 0, 41.f, 39.f, 163, 73, 164);

	pBlueAni->AddFrameClip(L"Monster/BlueSnail/Hit/Right/Hit", "HitRight", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(41.f, 39.f), 0, 0, 41.f, 39.f, 163, 73, 164);

	// Die
	pBlueAni->AddFrameClip(L"Monster/BlueSnail/Die/Left/Die", "DieLeft", AO_ONCE_DESTROY,
		0.f, 2.f, 6, _SIZE(56.f, 32.f), 0, 0, 56.f, 32.f, 163, 73, 164);

	pBlueAni->AddFrameClip(L"Monster/BlueSnail/Die/Right/Die", "DieRight", AO_ONCE_DESTROY,
		0.f, 2.f, 6, _SIZE(56.f, 32.f), 0, 0, 56.f, 32.f, 163, 73, 164);

	return true;
}

bool CCreateManager::OrangeMushroomCreate()
{
	CNormalMonster* pMonster = new CNormalMonster;

	CreateMonster(pMonster, "OrangeMushroom", ORANGE_MUSHROOM,
		_SIZE(63.f, 58.f), 2, 20, 0, 4, 10, 3, 5, 150.f);

	CAnimation* pMonsterAni = pMonster->CreateAnimation("BlueSnail");

	// Stand
	pMonsterAni->AddFrameClip(L"Monster/OrangeMushroom/Stand/Left/Stand", "StandLeft", AO_LOOP,
		0.f, 0.3f, 2, _SIZE(63.f, 58.f), 0, 0, 63.f, 58.f, 163, 73, 164);

	pMonsterAni->AddFrameClip(L"Monster/OrangeMushroom/Stand/Right/Stand", "StandRight", AO_LOOP,
		0.f, 0.3f, 2, _SIZE(63.f, 58.f), 0, 0, 63.f, 58.f, 163, 73, 164);

	// Move
	pMonsterAni->AddFrameClip(L"Monster/OrangeMushroom/Move/Left/Move", "MoveLeft", AO_LOOP,
		0.f, 0.3f, 3, _SIZE(64.f, 67.f), 0, 0, 64.f, 67.f, 163, 73, 164);

	pMonsterAni->AddFrameClip(L"Monster/OrangeMushroom/Move/Right/Move", "MoveRight", AO_LOOP,
		0.f, 0.3f, 3, _SIZE(64.f, 67.f), 0, 0, 64.f, 67.f, 163, 73, 164);

	// Hit
	pMonsterAni->AddFrameClip(L"Monster/OrangeMushroom/Hit/Left/Hit", "HitLeft", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(62.f, 65.f), 0, 0, 62.f, 65.f, 163, 73, 164);

	pMonsterAni->AddFrameClip(L"Monster/OrangeMushroom/Hit/Right/Hit", "HitRight", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(62.f, 65.f), 0, 0, 62.f, 65.f, 163, 73, 164);

	// Die
	pMonsterAni->AddFrameClip(L"Monster/OrangeMushroom/Die/Left/Die", "DieLeft", AO_ONCE_DESTROY,
		0.f, 2.f, 6, _SIZE(65.f, 59.f), 0, 0, 65.f, 59.f, 163, 73, 164);

	pMonsterAni->AddFrameClip(L"Monster/OrangeMushroom/Die/Right/Die", "DieRight", AO_ONCE_DESTROY,
		0.f, 2.f, 6, _SIZE(65.f, 59.f), 0, 0, 65.f, 59.f, 163, 73, 164);
	

	// 일단 기본 애니메이션만.
	return true;
}

bool CCreateManager::GreenMushroomCreate()
{
	CNormalMonster* pGreen = new CNormalMonster;

	CreateMonster(pGreen, "GreenMushroom", GREEN_MUSHROOM,
		_SIZE(56.f, 52.f), 12, 175, 20, 21, 35, 52, 58, 100);

	CAnimation* pGreenAni = pGreen->CreateAnimation("GreenMushroom");

	// Stand
	pGreenAni->AddFrameClip(L"Monster/GreenMushroom/Stand/Left/Stand", "StandLeft", AO_LOOP,
		0.f, 1.f, 3, _SIZE(56.f, 52.f), 0, 0, 56.f, 52.f, 163, 73, 164);

	pGreenAni->AddFrameClip(L"Monster/GreenMushroom/Stand/Right/Stand", "StandRight", AO_LOOP,
		0.f, 1.f, 3, _SIZE(56.f, 52.f), 0, 0, 56.f, 52.f, 163, 73, 164);

	// Move
	pGreenAni->AddFrameClip(L"Monster/GreenMushroom/Move/Left/Move", "MoveLeft", AO_LOOP,
		0.f, 1.f, 4, _SIZE(56.f, 52.f), 0, 0, 56.f, 52.f, 163, 73, 164);

	pGreenAni->AddFrameClip(L"Monster/GreenMushroom/Move/Right/Move", "MoveRight", AO_LOOP,
		0.f, 1.f, 4, _SIZE(56.f, 52.f), 0, 0, 56.f, 52.f, 163, 73, 164);

	// Hit
	pGreenAni->AddFrameClip(L"Monster/GreenMushroom/Hit/Left/Hit", "HitLeft", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(56.f, 56.f), 0, 0, 56.f, 56.f, 163, 73, 164);

	pGreenAni->AddFrameClip(L"Monster/GreenMushroom/Hit/Right/Hit", "HitRight", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(56.f, 56.f), 0, 0, 56.f, 56.f, 163, 73, 164);

	// Die
	pGreenAni->AddFrameClip(L"Monster/GreenMushroom/Die/Left/Die", "DieLeft", AO_ONCE_DESTROY,
		0.f, 2.f, 7, _SIZE(59.f, 50.f), 0, 0, 59.f, 50.f, 163, 73, 164);

	pGreenAni->AddFrameClip(L"Monster/GreenMushroom/Die/Right/Die", "DieRight", AO_ONCE_DESTROY,
		0.f, 2.f, 7, _SIZE(59.f, 50.f), 0, 0, 59.f, 50.f, 163, 73, 164);

	return true;
}

bool CCreateManager::BlueMushroomCreate()
{
	CNormalMonster* pBlueMush = new CNormalMonster;

	CreateMonster(pBlueMush, "BlueMushroom", BLUE_MUSHROOM,
		_SIZE(56.f, 52), 14, 225, 20, 24, 45, 56, 60, 100);

	CAnimation* pBlueMushAni = pBlueMush->CreateAnimation("BlueMushroom");

	// Stand
	pBlueMushAni->AddFrameClip(L"Monster/BlueMushroom/Stand/Left/Stand", "StandLeft", AO_LOOP,
		0.f, 1.f, 3, _SIZE(56.f, 52.f), 0, 0, 56.f, 52.f, 163, 73, 164);

	pBlueMushAni->AddFrameClip(L"Monster/BlueMushroom/Stand/Right/Stand", "StandRight", AO_LOOP,
		0.f, 1.f, 3, _SIZE(56.f, 52.f), 0, 0, 56.f, 52.f, 163, 73, 164);

	// Move
	pBlueMushAni->AddFrameClip(L"Monster/BlueMushroom/Move/Left/Move", "MoveLeft", AO_LOOP,
		0.f, 2.f, 4, _SIZE(56.f, 52.f), 0, 0, 56.f, 52.f, 163, 73, 164);

	pBlueMushAni->AddFrameClip(L"Monster/BlueMushroom/Move/Right/Move", "MoveRight", AO_LOOP,
		0.f, 2.f, 4, _SIZE(56.f, 52.f), 0, 0, 56.f, 52.f, 163, 73, 164);

	// Hit
	pBlueMushAni->AddFrameClip(L"Monster/BlueMushroom/Hit/Left/Hit", "HitLeft", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(56.f, 56.f), 0, 0, 56.f, 56.f, 163, 73, 164);

	pBlueMushAni->AddFrameClip(L"Monster/BlueMushroom/Hit/Right/Hit", "HitRight", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(56.f, 56.f), 0, 0, 56.f, 56.f, 163, 73, 164);

	// Die
	pBlueMushAni->AddFrameClip(L"Monster/BlueMushroom/Die/Left/Die", "DieLeft", AO_ONCE_DESTROY,
		0.f, 2.f, 7, _SIZE(59.f, 50.f), 0, 0, 59.f, 50.f, 163, 73, 164);

	pBlueMushAni->AddFrameClip(L"Monster/BlueMushroom/Die/Right/Die", "DieRight", AO_ONCE_DESTROY,
		0.f, 2.f, 7, _SIZE(59.f, 50.f), 0, 0, 59.f, 50.f, 163, 73, 164);

	return true;
}

bool CCreateManager::MushMomCreate()
{
	return true;
}

bool CCreateManager::GolemCreate()
{
	// 셋팅은 나중에 히힛!

	CNormalMonster* pMonster = new CNormalMonster;

	CreateMonster(pMonster, "Golem", GOLEM,
		_SIZE(179.f, 158.f), 17, 20, 0, 4, 10, 3, 5, 35.f);

	CAnimation* pMonsterAni = pMonster->CreateAnimation("Golem");

	// Stand
	pMonsterAni->AddFrameClip(L"Monster/Golem/Stand/Left/Stand", "StandLeft", AO_LOOP,
		0.f, 1.5f, 3, _SIZE(179.f, 158.f), 0, 0, 179.f, 158.f, 163, 73, 164);

	pMonsterAni->AddFrameClip(L"Monster/Golem/Stand/Right/Stand", "StandRight", AO_LOOP,
		0.f, 1.5f, 3, _SIZE(179.f, 158.f), 0, 0, 179.f, 158.f, 163, 73, 164);

	// Move
	pMonsterAni->AddFrameClip(L"Monster/Golem/Move/Left/Move", "MoveLeft", AO_LOOP,
		0.f, 1.5f, 4, _SIZE(175.f, 158.f), 0, 0, 175.f, 158.f, 163, 73, 164);

	pMonsterAni->AddFrameClip(L"Monster/Golem/Move/Right/Move", "MoveRight", AO_LOOP,
		0.f, 1.5f, 4, _SIZE(175.f, 158.f), 0, 0, 175.f, 158.f, 163, 73, 164);

	// Hit
	pMonsterAni->AddFrameClip(L"Monster/Golem/Hit/Left/Hit", "HitLeft", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(258.f, 127.f), 0, 0, 258.f, 127.f, 163, 73, 164);

	pMonsterAni->AddFrameClip(L"Monster/Golem/Hit/Right/Hit", "HitRight", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(258.f, 127.f), 0, 0, 258.f, 127.f, 163, 73, 164);

	// Die
	pMonsterAni->AddFrameClip(L"Monster/Golem/Die/Left/Die", "DieLeft", AO_ONCE_DESTROY,
		0.f, 2.f, 12, _SIZE(228.f, 178.f), 0, 0, 228.f, 178.f, 163, 73, 164);

	pMonsterAni->AddFrameClip(L"Monster/Golem/Die/Right/Die", "DieRight", AO_ONCE_DESTROY,
		0.f, 2.f, 12, _SIZE(228.f, 178.f), 0, 0, 228.f, 178.f, 163, 73, 164);


	return true;
}

bool CCreateManager::TreeCreate()
{

	CNormalMonster* pTree = new CNormalMonster;

	CreateMonster(pTree, "Tree", TREE,
		_SIZE(62.f, 51.f), 7, 80, 10, 12, 18, 22, 25, 100);

	CAnimation* pTreeAni = pTree->CreateAnimation("Tree");

	// Stand
	pTreeAni->AddFrameClip(L"Monster/Tree/Stand/Left/Stand", "StandLeft", AO_LOOP,
		0.f, 1.f, 1, _SIZE(62.f, 51.f), 0, 0, 62.f, 51.f, 163, 73, 164);

	pTreeAni->AddFrameClip(L"Monster/Tree/Stand/Right/Stand", "StandRight", AO_LOOP,
		0.f, 1.f, 1, _SIZE(62.f, 51.f), 0, 0, 62.f, 51.f, 163, 73, 164);

	// Move
	pTreeAni->AddFrameClip(L"Monster/Tree/Move/Left/Move", "MoveLeft", AO_LOOP,
		0.f, 2.f, 4, _SIZE(67.f, 54.f), 0, 0, 67.f, 54.f, 163, 73, 164);

	pTreeAni->AddFrameClip(L"Monster/Tree/Move/Right/Move", "MoveRight", AO_LOOP,
		0.f, 2.f, 4, _SIZE(67.f, 54.f), 0, 0, 67.f, 54.f, 163, 73, 164);

	// Hit
	pTreeAni->AddFrameClip(L"Monster/Tree/Hit/Left/Hit", "HitLeft", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(57.f, 58.f), 0, 0, 57.f, 58.f, 163, 73, 164);

	pTreeAni->AddFrameClip(L"Monster/Tree/Hit/Right/Hit", "HitRight", AO_LOOP,
		0.f, 0.5f, 1, _SIZE(57.f, 58.f), 0, 0, 57.f, 58.f, 163, 73, 164);

	// Die
	pTreeAni->AddFrameClip(L"Monster/Tree/Die/Left/Die", "DieLeft", AO_ONCE_DESTROY,
		0.f, 2.f, 7, _SIZE(66.f, 53.f), 0, 0, 66.f, 53.f, 163, 73, 164);

	pTreeAni->AddFrameClip(L"Monster/Tree/Die/Right/Die", "DieRight", AO_ONCE_DESTROY,
		0.f, 2.f, 7, _SIZE(66.f, 53.f), 0, 0, 66.f, 53.f, 163, 73, 164);

	return true;
}

bool CCreateManager::ItemCreate()
{
	// RED
	CItem* pItem = new CItem;

	pItem->SetItemTag(IT_RED_POTION);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/RedPotion.bmp",
		_SIZE(27.f, 27.f), 0, 128, 0));

	InsertObj("RedPotion", pItem);

	// Orange

	pItem = new CItem;

	pItem->SetItemTag(IT_ORANGE_POTION);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/OrangePotion.bmp",
		_SIZE(27.f, 27.f), 0, 128, 0));

	InsertObj("OrangePotion", pItem);

	// White

	pItem = new CItem;

	pItem->SetItemTag(IT_WHITE_POTION);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/WhitePotion.bmp",
		_SIZE(27.f, 27.f), 0, 128, 0));

	InsertObj("WhitePotion", pItem);

	// Blue

	pItem = new CItem;

	pItem->SetItemTag(IT_BLUE_POTION);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/BluePotion.bmp",
		_SIZE(27.f, 27.f), 0, 128, 0));

	InsertObj("BluePotion", pItem);

	// WorriorPotion

	pItem = new CItem;

	pItem->SetItemTag(IT_WORRIOR_POTION);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/WorriorPotion.bmp",
		_SIZE(25.f, 25.f), 0, 128, 0));

	InsertObj("WorriorPotion", pItem);

	// Speed Potion

	pItem = new CItem;

	pItem->SetItemTag(IT_SPEED_POTION);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/SpeedPotion.bmp",
		_SIZE(25.f, 25.f), 0, 128, 0));

	InsertObj("SpeedPotion", pItem);

	// APPLE

	pItem = new CItem;

	pItem->SetItemTag(IT_APPLE);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/Apple.bmp",
		_SIZE(20.f, 25.f), 0, 128, 0));

	InsertObj("Apple", pItem);

	// EGG

	pItem = new CItem;

	pItem->SetItemTag(IT_EGG);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/Egg.bmp",
		_SIZE(27.f, 21.f), 0, 128, 0));

	InsertObj("Egg", pItem);

	// Orange

	pItem = new CItem;

	pItem->SetItemTag(IT_ORANGE);

	pItem->Init();

	pItem->SetTexture(CTexture::SetTexture(L"Item/Icon/Orange.bmp",
		_SIZE(21.f, 20.f), 0, 128, 0));

	InsertObj("Orange", pItem);


	return true;
}

bool CCreateManager::MonsterHPUICreate()
{
	// UI : Monster Panel

	// Collider에서 처리하는 것은 이상하다.
	// UI에서 처리하고 잡혀있는 몬스터의 중심 - size.x / 2를 이용해서 RenderPos를 업데이트 시키면서 
	// 동시에 체력이 감소되는 것을 표현한다.
	// 그러니까 2개 같이 들고 다니는거 ㅇㅇ

	// MONSTER HP BORDER
	CUIPanel* pPanel = new CUIPanel;

	pPanel->SetPanel(GetPoint(0, 0), CTexture::SetTexture(L"UI/BarUI/MonsterBarUI.bmp",
		_SIZE(50.f, 8.f), 0, 128, 0), PT_MONSTERHPBORDER, NULL);

	InsertObj("MonsterBar1", pPanel);

	// MONSTER HPBAR
	pPanel = new CUIPanel;

	pPanel->SetPanel(GetPoint(0, 0), CTexture::SetTexture(L"UI/BarUI/MonsterHPBar.bmp",
		_SIZE(46.f, 4.f), 0, 128, 0), PT_MONSTERHPBAR, NULL);

	InsertObj("MonsterBar2", pPanel);


	// 오브젝트 셋팅, renderPoint, update등은 따로 처리. 
	return true;
}

bool CCreateManager::PlayerNormalAttackEffect()
{

	/*
		공식 : y축을 기준으로 오른쪽을 잡았으면,
		오른쪽모션과 y축은 같다.

		x축은 왼쪽모션에서 y축 대칭을 한 모습을 생각하면 되는데, 여기서 왼쪽 이미지는 왼쪽 상단이 기준이고,
		오른쪽 이미지는 오른쪽 상단이 기준이라는 것이다.
		
		즉, 오른쪽 이미지의 x 크기만큼을 빼야한다.
	*/

	CTexture* pTexture;
	CTexture* pTexture2;

	// Swing1 좌
	CEffect* pEffect = new CEffect;

	pEffect->SetTexture(CTexture::SetTexture(L"Player/AttackEffect/Left/Swing1Slash.bmp", _SIZE(85.f, 75.f), 0, 128, 0));

	pEffect->SetEffect(0.75f, -72, -18, false, 0.55f, ET_SWING1);

	InsertObj("Swing1SlashLeft", pEffect);

	// Swing1 우
	pEffect = new CEffect;

	pEffect->SetTexture(CTexture::SetTexture(L"Player/AttackEffect/Right/Swing1Slash.bmp", _SIZE(85.f, 75.f), 0, 128, 0));

	pEffect->SetEffect(0.75f, -13, -18, false, 0.55f, ET_SWING1);

	InsertObj("Swing1SlashRight", pEffect);


	///////////////////////

	// Swing2 좌
	pEffect = new CEffect;

	pEffect->SetTexture(CTexture::SetTexture(L"Player/AttackEffect/Left/Swing2Slash.bmp", _SIZE(81.f, 42.f), 0, 128, 0));

	pEffect->SetEffect(0.75f, -91, -20, false, 0.55f, ET_SWING2);

	InsertObj("Swing2SlashLeft", pEffect);

	// Swing2 우
	pEffect = new CEffect;

	pEffect->SetTexture(CTexture::SetTexture(L"Player/AttackEffect/Right/Swing2Slash.bmp", _SIZE(81.f, 42.f), 0, 128, 0));

	pEffect->SetEffect(0.75f, 10, -20, false, 0.55f, ET_SWING2);

	InsertObj("Swing2SlashRight", pEffect);

	///////////////////////

	// Swing3 좌
	pEffect = new CEffect;

	pEffect->SetTexture(CTexture::SetTexture(L"Player/AttackEffect/Left/Swing3Slash.bmp", _SIZE(80.f, 60.f), 0, 128, 0));

	pEffect->SetEffect(0.75f, -85, -35, false, 0.55f, ET_SWING3);

	InsertObj("Swing3SlashLeft", pEffect);

	// Swing3 우
	pEffect = new CEffect;

	pEffect->SetTexture(CTexture::SetTexture(L"Player/AttackEffect/Right/Swing3Slash.bmp", _SIZE(80.f, 60.f), 0, 128, 0));

	pEffect->SetEffect(0.75f, 5, -35, false, 0.55f, ET_SWING3);

	InsertObj("Swing3SlashRight", pEffect);


	///////////////////////

	pTexture = CTexture::SetTexture(L"Player/AttackEffect/Left/StabSlash.bmp", _SIZE(40.f, 17.f), 0, 128, 0);

	// Stab 좌
	pEffect = new CEffect;

	pEffect->SetTexture(pTexture);

	pEffect->SetEffect(0.75f, -83, 1, false, 0.55f, ET_STAB);

	InsertObj("StabSlashLeft", pEffect);


	pTexture2 = CTexture::SetTexture(L"Player/AttackEffect/Right/StabSlash.bmp", _SIZE(40.f, 17.f), 0, 128, 0);

	// Stab 우
	pEffect = new CEffect;

	pEffect->SetTexture(pTexture2);

	pEffect->SetEffect(0.75f, 43, 1, false, 0.55f, ET_STAB);

	InsertObj("StabSlashRight", pEffect);
	
	///////////////////////////////////////////////////

	// Prone Attack의 경우에는 Stab과 동일한 이미지를 사용한다.

	// ProneAttack 좌
	pEffect = new CEffect;

	pEffect->SetTexture(pTexture);

	pEffect->SetEffect(0.75f, -50, 20, true, 0.25f, ET_PRONE_ATTACK_LEFT);

	InsertObj("ProneSlashLeft", pEffect);

	// ProneAttack 우
	pEffect = new CEffect;

	pEffect->SetTexture(pTexture2);

	pEffect->SetEffect(0.75f, 10, 20, true, 0.25f, ET_PRONE_ATTACK_RIGHT);

	InsertObj("ProneSlashRight", pEffect);

	pTexture->AddRef();
	pTexture2->AddRef();

	return true;

	/*
	이펙트가 블렌딩 효과도 있지만, 그걸 떠나서, 처음에 대기 시간이 존재하고, 움직이는 친구도 있... -> OFFSET 변경...
	근데 offset 변경이 오른쪽 기준 왼쪽 기준이 달라짐... 슈빨 ?
	
	
	*/
}

bool CCreateManager::PlayerSkillEffect()
{
	// 플레이어의 스킬을 정의하는 함수.

	/*
	메이플에서는 스킬을 쓴 자리가 공격 범위가 되는 것 같은데 나는 그냥 그 자리로 할 것이다.
	애초에 점공을 막을까 생각중이라. 아무튼 ㅋ
	*/

	
	// 참고로 모션은 SlashRight Left 이런 식으로 잡음
	// 이펙트는 SlashBladeLeft Right OK ?
	// 근데 이펙트

	
	// Slash Effect 좌
	CEffect* pEffect = new CEffect;

	pEffect->SetEffect(1.5f, -240, -130, true, 0.5f, ET_SKILL);

	CAnimation* pAni = pEffect->CreateAnimation("SlashBladeLeft");

	pAni->AddFrameClip(L"Player/Skill/SlashBlade/Effect/Left/Effect",
		"SlashBladeLeft", AO_LOOP, 0.f, 1.5f, 9, _SIZE(330.f, 233.f),
		0.f, 0.f, 330.f, 233.f, 0, 128, 0);

	pAni->SetDefaultClip("SlashBladeLeft");

	InsertObj("SlashBladeLeft", pEffect);

	// Slash Effect 우
	pEffect = new CEffect;

	pEffect->SetEffect(1.5f, -90, -130, true, 0.5f, ET_SKILL);

	pAni = pEffect->CreateAnimation("SlashBladeRight");

	pAni->AddFrameClip(L"Player/Skill/SlashBlade/Effect/Right/Effect",
		"SlashBladeRight", AO_LOOP, 0.f, 1.5f, 9, _SIZE(330.f, 233.f),
		0.f, 0.f, 330.f, 233.f, 0, 128, 0);

	pAni->SetDefaultClip("SlashBladeRight");

	InsertObj("SlashBladeRight", pEffect);

	// 오브젝트를 잡고 change clip은 나중에 하자.


	// 리프1 Left   날라가는 바람
	pEffect = new CEffect;

	pEffect->SetEffect(0.9f, 20, -47, true, 0.3f, ET_SKILL_LEAF);

	pAni = pEffect->CreateAnimation("Leaf1Left");

	pAni->AddFrameClip(L"Player/Skill/WorriorLeaf/Effect1/Left/Effect",
		"Leaf1Left", AO_LOOP, 0.f, 0.9f, 3, _SIZE(195.f, 94.f),
		0.f, 0.f, 195.f, 94.f, 0, 128, 0);

	pAni->SetDefaultClip("Leaf1Left");

	InsertObj("Leaf1Left", pEffect);

	// 리프1 Right
	pEffect = new CEffect;

	pEffect->SetEffect(0.9f, -215, -47, true, 0.3f, ET_SKILL_LEAF);

	pAni = pEffect->CreateAnimation("Leaf1Right");

	pAni->AddFrameClip(L"Player/Skill/WorriorLeaf/Effect1/Right/Effect",
		"Leaf1Right", AO_LOOP, 0.f, 0.9f, 3, _SIZE(195.f, 94.f),
		0.f, 0.f, 195.f, 94.f, 0, 128, 0);

	pAni->SetDefaultClip("Leaf1Right");

	InsertObj("Leaf1Right", pEffect);


	// 리프2 Left : 마법진
	pEffect = new CEffect;

	pEffect->SetEffect(0.9f, 0, 0, true, 0.3f, ET_SKILL_LEAF);

	pAni = pEffect->CreateAnimation("Leaf2Left");

	pAni->AddFrameClip(L"Player/Skill/WorriorLeaf/Effect2/Left/Effect",
		"Leaf2Left", AO_LOOP, 0.f, 0.9f, 3, _SIZE(184.f, 119.f),
		0.f, 0.f, 184.f, 119.f, 0, 128, 0);

	pAni->SetDefaultClip("Leaf2Left");

	InsertObj("Leaf2Left", pEffect);

	// 리프2 Right
	pEffect = new CEffect;

	pEffect->SetEffect(0.9f, 0, 0, true, 0.3f, ET_SKILL_LEAF);

	pAni = pEffect->CreateAnimation("Leaf2Right");

	pAni->AddFrameClip(L"Player/Skill/WorriorLeaf/Effect2/Right/Effect",
		"Leaf2Right", AO_LOOP, 0.f, 0.9f, 3, _SIZE(184.f, 119.f),
		0.f, 0.f, 184.f, 119.f, 0, 128, 0);

	pAni->SetDefaultClip("Leaf2Right");

	InsertObj("Leaf2Right", pEffect);


	// 리프3 Left : 지팡이
	pEffect = new CEffect;

	pEffect->SetEffect(0.9f, -100 , -75, true, 0.3f, ET_SKILL_LEAF);

	pAni = pEffect->CreateAnimation("Leaf3Left");

	pAni->AddFrameClip(L"Player/Skill/WorriorLeaf/Effect3/Left/Effect",
		"Leaf3Left", AO_LOOP, 0.f, 0.9f, 3, _SIZE(300.f, 150.f),
		0.f, 0.f, 300.f, 150.f, 0, 128, 0);

	pAni->SetDefaultClip("Leaf3Left");

	InsertObj("Leaf3Left", pEffect);

	// 리프3 Right
	pEffect = new CEffect;

	pEffect->SetEffect(0.9f, -200, -75, true, 0.3f, ET_SKILL_LEAF);

	pAni = pEffect->CreateAnimation("Leaf3Right");

	pAni->AddFrameClip(L"Player/Skill/WorriorLeaf/Effect3/Right/Effect",
		"Leaf3Right", AO_LOOP, 0.f, 0.9f, 3, _SIZE(300.f, 150.f),
		0.f, 0.f, 300.f, 150.f, 0, 128, 0);

	pAni->SetDefaultClip("Leaf3Right");

	InsertObj("Leaf3Right", pEffect);


	return true;
}

