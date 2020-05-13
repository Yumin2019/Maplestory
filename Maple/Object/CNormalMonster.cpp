#include "CNormalMonster.h"
#include "../Animation/CAnimation.h"
#include "../Collision/CShape.h"
#include "../Object/CPlayer.h"
#include "../Core/CSceneManager.h"
#include "../Core/CCreateManager.h"
#include "../UI/CUIPanel.h"

CNormalMonster::CNormalMonster() :
	m_bHPBar(false),
	m_fHPBarTime(0.f)
{
}


CNormalMonster::~CNormalMonster()
{
}

CNormalMonster::CNormalMonster(const CNormalMonster & mon) :
	CMonster(mon)
{
	m_eName = mon.m_eName;

	m_bFalling = true;
	m_bJump = false;

	m_bStop = true;
	m_fStopLimitTime = 1.f;

	m_fHPBarTime = 0.f;
	m_bHPBar = false;
}

bool CNormalMonster::Init()
{
	int idx = rand() % 2;

	m_eDir = idx ? MD_RIGHT : MD_LEFT;

	m_pAnimation->SetObj(this);

	switch (m_eDir)
	{
	case MD_RIGHT:
		m_pAnimation->SetDefaultClip("MoveRight");

		break;

	case MD_LEFT:
		m_pAnimation->SetDefaultClip("MoveLeft");

		break;
	}

	m_pAnimation->ChangeClip();


	// COLLIDER

	switch (m_eName)
	{
	case RED_SNAIL:
	case BLUE_SNAIL:
		SnailColliderInit();
		break;

	
	case BLUE_MUSHROOM:
	case GREEN_MUSHROOM:
		MushroomColliderInit();
		break;


	case TREE:
		TreeColliderInit();
		break;


	case ORANGE_MUSHROOM:
		OrangeMushroomColliderInit();
		break;

	case GOLEM:
		GolemColliderInit();
		break;
	

	//case MUSHMOM:
	//	break;
	}
	
	// 왼쪽 Turn 
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(3.f, 3.f), GetPoint(0, GetSizeY() / 2), ST_RECT_TURN);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionObstacleStart);

	AddCollider(pShape);

	// 오른쪽 Turn 
	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(3.f, 3.f), GetPoint(GetSizeX(), GetSizeY() / 2), ST_RECT_TURN);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionObstacleStart);

	AddCollider(pShape);
	

	// 모든 몬스터는 공격용으로 자신의 기본 Rect를 들고 다님. 이것과 플레이어 중간 렉트를 충돌시킴.
	pShape = new CShape;

	pShape->InitColliderShape(GetSize(), GetPoint(0, 0), ST_RECT_MONSTER_ATTACK);

	pShape->SetObj(this);

	AddCollider(pShape);

	// 모든 몬스터는 체력 border와 체력바를 가지고 다닌다. SceneManager 를 통해서 ObjList를 들고 온다. 


	list<CObj*>& UIList = GET_SINGLE(CSceneManager)->GetList("UILayer");


	// bar 1

	CUIPanel* pPanel = FIND(CUIPanel)->FindObj("MonsterBar1")->Clone();

	pPanel->SetPanelObj(this);

	UIList.push_back(pPanel);

	// bar 2

	pPanel = FIND(CUIPanel)->FindObj("MonsterBar2")->Clone();

	pPanel->SetPanelObj(this);

	UIList.push_back(pPanel);


	return true;
}

void CNormalMonster::Input(float fDeltaTime)
{
	CMonster::Input(fDeltaTime);
}

void CNormalMonster::Collision(float fDeltaTime)
{
	CMonster::Collision(fDeltaTime);

}

void CNormalMonster::Update(float fDeltaTime)
{
	// 체력바 독립적

	if (m_bHPBar)
	{
		m_fHPBarTime += fDeltaTime;

		if (m_fHPBarTime >= 10.f)
		{
			m_bHPBar = false;
		}
	}
	
	if (!m_bKnockBack)
	{
		if (m_bStop)
		{
			m_fStopTime += fDeltaTime;

			if (m_eMotion != MM_STAND)
			{
				m_eMotion = MM_STAND;
				ChangeMotion();
			}


			if (m_fStopTime >= m_fStopLimitTime)
			{
				m_fStopTime = 0.f;
				m_bStop = false;

			}
		}
		else
		{
			// 자신의 방향에 따라서 움직인다.

			// 중간에 Hit 모션으로 끊기는 경우를 생각을 해줘야 한다.

			// Hit 에서 초기값을 셋팅.

			m_fMoveTime += fDeltaTime;
			MoveX(fDeltaTime);

			if (m_fMoveTime >= m_fMoveLimitTime)
			{
				m_bStop = true;
				m_fMoveTime = 0.f;

				switch (m_eName)
				{
				case RED_SNAIL:
				case BLUE_SNAIL:
					m_fMoveLimitTime = (float)(rand() % 10 + 3) * 0.7f;
					m_fStopLimitTime = (float)(rand() % 3 + 1);
					break;

				default:
					m_fMoveLimitTime = 5.f;
					m_fStopLimitTime = 3.5f;
				}


				m_eMotion = MM_STAND;

				ChangeMotion();

				// 멈춘 이후에 방향 랜덤
				int idx = rand() % 2;
				m_eDir = idx ? MD_RIGHT : MD_LEFT;

			}
			else
			{

				if (m_eMotion != MM_MOVE)
				{
					m_eMotion = MM_MOVE;
					ChangeMotion();
				}


			}
		}
	}


	RenderPosUpdate();

	CMonster::Update(fDeltaTime);

}

void CNormalMonster::Render(HDC hDC, float fDeltaTime)
{
	CMonster::Render(hDC, fDeltaTime);

}

CNormalMonster * CNormalMonster::Clone()
{
	return new CNormalMonster(*this);
}

void CNormalMonster::CollisionObstacleStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	switch (m_eDir)
	{
	case MD_RIGHT:
		m_eDir = MD_LEFT;

		m_eMotion = MM_MOVE;
		ChangeMotion();

		break;

	case MD_LEFT:
		m_eDir = MD_RIGHT;

		m_eMotion = MM_MOVE;
		ChangeMotion();


		break;
	}
}


void CNormalMonster::CollisionTerrainStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
}

void CNormalMonster::CollisionTerrainStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bFalling = false;
}

void CNormalMonster::CollisionTerrainEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
}

void CNormalMonster::ChangeMotion()
{

	string strAni;

	switch (m_eMotion) // 미리 셋팅
	{
	case MM_STAND:
		strAni = "Stand";

		break;

	case MM_HIT:
		strAni = "Hit";
		// 살짝 뒤로 던?져 지는 처리

		


		break;

	case MM_DIE:
		strAni = "Die";

		m_bStop = true;
		m_fStopLimitTime = 3.f;
		

		break;

	case MM_MOVE:
		strAni = "Move";

		break;
	}


	switch (m_eDir)
	{
	case MD_LEFT:
		strAni += "Left";

		break;

	case MD_RIGHT:
		strAni += "Right";

		break;
	}

	m_pAnimation->ChangeClip(strAni);

}

void CNormalMonster::ChangeNextMotion()
{
	// 이전 모션에 따른 처리
	string strAni;

	switch (m_eMotion) 
	{
	case MM_STAND:
		strAni = "Move";
		m_eMotion = MM_MOVE;

		break;

	case MM_HIT:
		strAni = "Stand"; // 플레이어를 향해 따라가는 것도 나중에 만들 예정. rect 범위로 인식 radar
		m_eMotion = MM_STAND;

		break;

	case MM_DIE:  // 이미 죽음
		/*strAni = "Die";

		m_eMotion = MM_STAND;*/

		break;

	case MM_MOVE:
		strAni = "Move";
		m_eMotion = MM_MOVE;

		break;
	}


	switch (m_eDir)
	{
	case MD_LEFT:
		strAni += "Left";

		break;

	case MD_RIGHT:
		strAni += "Right";

		break;
	}

	m_pAnimation->ChangeClip(strAni);
}

void CNormalMonster::RenderPosUpdate()
{
	switch (m_eName)
	{
	case RED_SNAIL:
		RedSnailRenderPos();
		break;

	case BLUE_SNAIL:
		BlueSnailRenderPos();
		break;

	case GREEN_MUSHROOM:
		GreenMushroomRenderPos();
		break;

	case BLUE_MUSHROOM:
		BlueMushroomRenderPos();
		break;

	case TREE:
		TreeRenderPos();
		break;

	case GOLEM:
		GolemRenderPos();
		break;

	case ORANGE_MUSHROOM:
		OrangeMushroomRenderPos();
		break;
	}
}

void CNormalMonster::RenderAnimation(HDC hDC, int x, int y)
{

	wstring strAni; 

	switch (m_eMotion) // 미리 셋팅
	{
	case MM_STAND:
		strAni = L"Stand";

		break;

	case MM_HIT:
		strAni = L"Hit";




		break;

	case MM_DIE:
		strAni = L"Die";



		break;

	case MM_MOVE:
		strAni = L"Move";

		break;
	}


	switch (m_eDir)
	{
	case MD_LEFT:
		strAni += L"Left";

		break;

	case MD_RIGHT:
		strAni += L"Right";

		break;
	}

	wchar_t strPos[100];
	
	// Stop
	swprintf_s(strPos, L"Stop : %d", m_bStop ? 1 : 0);
	TextOut(hDC, x, y + 20, strPos, lstrlen(strPos));

	// Hit
	swprintf_s(strPos, L"KnockBack : %d", m_bKnockBack ? 1 : 0);
	TextOut(hDC, x, y + 40, strPos, lstrlen(strPos));

	// Move
	swprintf_s(strPos, L"Move : %.2f / %.2f", m_fMoveTime, m_fMoveLimitTime);
	TextOut(hDC, x, y + 60, strPos, lstrlen(strPos));

	// Stop
	swprintf_s(strPos, L"StopTime : %.2f / %.2f", m_fStopTime, m_fStopLimitTime);
	TextOut(hDC, x, y + 80, strPos, lstrlen(strPos));

	TextOut(hDC, x, y, strAni.c_str(), strAni.size());

}

void CNormalMonster::CollisionPlayerAttackStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	// 대기 -> hit -> move 사이클 중에서 
	// 무브면 문제가 없고, hit이면 대기 처음 상태로 대기에서 걸려도 다시 대기 처음 상태로 이동한다.
	// 이 로직대로라면, 대기 cycle이 도는 경우 hit모션이 계속 씹힌다. 이걸 확인해보자


	// 몬스터의 방향을 플레이어가 바라보는 방향의 반대 방향으로 옮긴다.
	
	// 이게 지금 모션을 바꿀 것이 아니라, 몇 초 이후에 바꿔야 한다. 미리 받아둘 변수가 필요하다.

	switch (((CPlayer*)pDest->GetObj())->GetDir())
	{
	case MD_RIGHT:
		m_eHitDir = MD_LEFT;
		break;

	case MD_LEFT:
		m_eHitDir = MD_RIGHT;
		break;
	}

	// HIT 모션 대기시간 설정

	// 맞은 충돌체의 따라서 hit 대기 시간이 달라진다.
	
	m_fHitTime = 0.f;
	m_bHit = true;

	switch (pDest->GetShape())
	{
	case ST_RECT_NORMAL_LEFT:
		
		m_fHitLimitTime = 0.7f;

		break;

	case ST_RECT_NORMAL_RIGHT:
		m_fHitLimitTime = 0.7f;

		break;

	case ST_RECT_SKILL_LEFT:
		m_fHitLimitTime = 1.f;

		break;

	case ST_RECT_SKILL_RIGHT:
		m_fHitLimitTime = 1.f;

		break;
	}

	// 대기시간 이후에 KNockBack true


	// 충돌되자마자 몬스터의 체력을 깐다. 체력바 true
	m_bHPBar = true;
	m_fHPBarTime = 0.f;
}



void CNormalMonster::ChangeHitMotion()
{
	// 대기 시간 이후에 플레이어의 반대 방향으로 dir을 셋팅해야 한다.
	// 이유는 때리자마자 방향이 바뀌면 이상함.... ㅇㅇ

	m_eDir = m_eHitDir;
	m_eMotion = MM_HIT;
	ChangeMotion();
}

void CNormalMonster::GreenMushroomRenderPos()
{

	switch (m_eMotion)
	{
			case MM_STAND:

				switch (m_eDir)
				{
				case MD_LEFT:
					m_tRenderPos.x = m_tPos.x;
					m_tRenderPos.y = m_tPos.y;

					break;

				case MD_RIGHT:
					m_tRenderPos.x = m_tPos.x;
					m_tRenderPos.y = m_tPos.y;

					break;
				}

				break;

			case MM_DIE:
				switch (m_eDir)
				{
				case MD_LEFT:
					m_tRenderPos.x = m_tPos.x;
					m_tRenderPos.y = m_tPos.y;

					break;

				case MD_RIGHT:
					m_tRenderPos.x = m_tPos.x;
					m_tRenderPos.y = m_tPos.y;

					break;
				}

				break;

			case MM_MOVE:
				switch (m_eDir)
				{
				case MD_LEFT:
					m_tRenderPos.x = m_tPos.x;
					m_tRenderPos.y = m_tPos.y;

					break;

				case MD_RIGHT:
					m_tRenderPos.x = m_tPos.x;
					m_tRenderPos.y = m_tPos.y;

					break;
				}

				break;


			case MM_HIT:
				switch (m_eDir)
				{
				case MD_LEFT:
					m_tRenderPos.x = m_tPos.x;
					m_tRenderPos.y = m_tPos.y;

					break;

				case MD_RIGHT:
					m_tRenderPos.x = m_tPos.x;
					m_tRenderPos.y = m_tPos.y;

					break;
				}

				break;
	}
}

void CNormalMonster::BlueMushroomRenderPos()
{

}

void CNormalMonster::TreeRenderPos()
{
}

void CNormalMonster::RedSnailRenderPos()
{
	switch (m_eMotion)
	{
	case MM_STAND:

		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;

	case MM_DIE:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;

	case MM_MOVE:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x - 9.f;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;


	case MM_HIT:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;
	}
}

void CNormalMonster::BlueSnailRenderPos()
{
	switch (m_eMotion)
	{
	case MM_STAND:

		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;

	case MM_DIE:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;

	case MM_MOVE:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x - 9.f;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;


	case MM_HIT:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;
	}
}

void CNormalMonster::OrangeMushroomRenderPos()
{
	switch (m_eMotion)
	{
	case MM_STAND:

		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;

	case MM_DIE:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;

	case MM_MOVE:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y - 12.f;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y - 12.f;

			break;
		}

		break;


	case MM_HIT:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;
	}
}

void CNormalMonster::GolemRenderPos()
{

	switch (m_eMotion)
	{
	case MM_STAND:

		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;

	case MM_DIE:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;

	case MM_MOVE:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;


	case MM_HIT:
		switch (m_eDir)
		{
		case MD_LEFT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;

		case MD_RIGHT:
			m_tRenderPos.x = m_tPos.x;
			m_tRenderPos.y = m_tPos.y;

			break;
		}

		break;
	}
}

void CNormalMonster::MushroomColliderInit()
{
	// 바닥용 
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(13.f, 1.f), GetPoint(22, 52), ST_RECT_TERRAIN);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionTerrainStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CNormalMonster::CollisionTerrainStay);
 
	pShape->AddCollisionFunc(CS_END, this, &CNormalMonster::CollisionTerrainEnd);

	AddCollider(pShape);


	// 중심
	CShape* pHead = new CShape;

	pHead->InitColliderShape(_SIZE(22.f, 22.f), GetPoint(15, 20), ST_RECT_MONSTER);

	pHead->SetObj(this);

	pHead->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionPlayerAttackStart);

	AddCollider(pHead);

}

void CNormalMonster::OrangeMushroomColliderInit()
{

	// 바닥용 
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(23.f, 1.f), GetPoint(17, 58), ST_RECT_TERRAIN);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionTerrainStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CNormalMonster::CollisionTerrainStay);

	pShape->AddCollisionFunc(CS_END, this, &CNormalMonster::CollisionTerrainEnd);

	AddCollider(pShape);


	// 중심
	CShape* pHead = new CShape;

	pHead->InitColliderShape(_SIZE(26.f, 26.f), GetPoint(17, 20), ST_RECT_MONSTER);

	pHead->SetObj(this);

	pHead->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionPlayerAttackStart);

	AddCollider(pHead);
}

void CNormalMonster::SnailColliderInit()
{
	// 바닥용 
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(20.f, 1.f), GetPoint(8, 34), ST_RECT_TERRAIN);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionTerrainStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CNormalMonster::CollisionTerrainStay);

	pShape->AddCollisionFunc(CS_END, this, &CNormalMonster::CollisionTerrainEnd);

	AddCollider(pShape);

	

	// 중심
	CShape* pHead = new CShape;

	pHead->InitColliderShape(_SIZE(20.f, 17.f), GetPoint(7, 7), ST_RECT_MONSTER);

	pHead->SetObj(this);

	pHead->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionPlayerAttackStart);

	AddCollider(pHead);


	/*
	현재 중심을 가지고 pixel 충돌을 하여, 끝을 넘어가지 못하도록 해주고 있다.

	근데 이것은 골렘 같은 경우에 자원 낭비가 너무 심하고, 골렘 때문에 추가하더라도, 다른 애들 까지 써줘야 효율적.

	일단 지금은 그냥 쓰고 수정할 것.
	
	*/


	/*
	pHead->AddCollisionFunc(CS_START, this, &CPlayer::CollisionRopeStart);

	pHead->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionRopeStay);

	pHead->AddCollisionFunc(CS_END, this, &CPlayer::CollisionRopeEnd);
	*/


}

void CNormalMonster::TreeColliderInit()
{
}

void CNormalMonster::GolemColliderInit()
{
	// 바닥용 
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(20.f, 1.f), GetPoint(98, 158), ST_RECT_TERRAIN);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionTerrainStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CNormalMonster::CollisionTerrainStay);

	pShape->AddCollisionFunc(CS_END, this, &CNormalMonster::CollisionTerrainEnd);

	AddCollider(pShape);


	// 중심
	CShape* pHead = new CShape;

	pHead->InitColliderShape(_SIZE(65.f, 65.f), GetPoint(58, 53), ST_RECT_MONSTER);

	pHead->SetObj(this);

	pHead->AddCollisionFunc(CS_START, this, &CNormalMonster::CollisionPlayerAttackStart);

	AddCollider(pHead);
}
