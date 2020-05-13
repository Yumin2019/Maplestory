#include "CPlayer.h"
#include "../Animation/CAnimation.h"
#include "../Core/CInput.h"
#include "../Core/CPencil.h"
#include "../Texture/CTexture.h"
#include "../Scene/CInGameScene.h"
#include "../Collision/CShape.h"
#include "../Core/CSceneManager.h"

#include "../Collision/CPixel.h"
#include "CEffect.h"
#include "../UI/CUIPanel.h"

// Item

#include "../UI/CInventoryPanel.h"
#include "../UI/CShopUI.h"


/*
스킬 1.2초(중간에 끊지 않는데 걸리는 시간)에 기본 모션 1.5초 + 0.2 대기 시간 생각해서 1.5초로 잡았고.
기본공격은 0.75가 모션인데 적어도 0.9초가 걸려서 0.9초를 정지시간으로 잡았다. 대기 시간 x 재사용 대기시간은 나중에 할 거면 하자.

기본에 계속 누르고 있을 때 잡히지 않는 오류는 인풋처리를 중복으로 하고 있던 것을 풀어서 해결했다.
*/

CPlayer::CPlayer()
{
	m_bRopeCycle = false;
	m_bRopeCollision = false;
	m_iExpMax = 0;
	m_iPoint = 0;

	m_fSpeedPotion = 0.f;
	m_fWorriorPotion = 0.f;
	m_iCriticalPercent = 5;
	m_iStancePercent = 0;

	m_bSpeedPotion = false;
	m_bWorriorPotion = false;

	m_pShop 		= NULL;
	m_pSkill		= NULL;
	m_pMinimap		= NULL;
	m_pInventory	= NULL;
	m_pAbility		= NULL;

	m_iStrength = 0;
	m_iIntellect = 0;
	m_iDexterity = 0;
	m_iLucky = 0;
}

CPlayer::CPlayer(const CPlayer & Player) :
	CMoveObj(Player)
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::DoubleJumpEffectLeft()
{
	// 바람
	CEffect* pEffect = FIND(CEffect)->FindObj("Leaf1Left")->Clone();

	pEffect->SetObj(this);

	CAnimation* pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	list<CObj*>& ObjList = GET_SINGLE(CSceneManager)->GetObjList("DefaultLayer");

	ObjList.push_back(pEffect);

	// 마법진 : 플레이어를 잡지 않고 그 자리에 있도록.
	pEffect = FIND(CEffect)->FindObj("Leaf2Left")->Clone();

	pEffect->SetRenderPos(GetCenterX() - 20.f, GetCenterY() - 59.f);

	pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	ObjList.push_back(pEffect);

	// 지팡이

	pEffect = FIND(CEffect)->FindObj("Leaf3Left")->Clone();

	pEffect->SetObj(this);

	pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	ObjList.push_front(pEffect);

}

void CPlayer::DoubleJumpEffectRight()
{
	// 1
	CEffect* pEffect = FIND(CEffect)->FindObj("Leaf1Right")->Clone();

	pEffect->SetObj(this);

	CAnimation* pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	list<CObj*>& ObjList = GET_SINGLE(CSceneManager)->GetObjList("DefaultLayer");

	ObjList.push_back(pEffect);

	// 2
	pEffect = FIND(CEffect)->FindObj("Leaf2Right")->Clone();

	pEffect->SetRenderPos(GetCenterX() - 164.f, GetCenterY() - 59.f);

	pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	ObjList.push_back(pEffect);

	// 3

	pEffect = FIND(CEffect)->FindObj("Leaf3Right")->Clone();

	pEffect->SetObj(this);

	pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	ObjList.push_front(pEffect);

}

void CPlayer::ChangeMotion()
{

	/*
		원래 Default는 Stand인데, 이 시간과 Stop이 같은 swing, stab같은 경우에는
		Stand모션이 아닌, 다른 모션으로 바꾸는 것이다.
		그러니 걱정하지 마라.
		
		내가 원하는 처리로 할 수 있다.
	*/


	string strAni;

	switch (m_ePlayerMotion) // 미리 셋팅
	{
	case PM_ALERT:
		strAni = "Alert";

		break;

	case PM_DIE:
		strAni = "Dead";

		break;

	case PM_HIT:
		strAni = "Hit";


		break;

	case PM_JUMP:
		strAni = "Jump";

		break;

	case PM_PRONE:
		strAni = "Prone";

		break;

	case PM_PRONE_ATTACK:
		strAni = "ProneAttack";

		m_fStopLimitTime = 0.9f;
		m_bStop = true;


		break;

	case PM_ROPE:
		strAni = "Rope";

		break;

	case PM_SIT:
		strAni = "Sit";


		break;

	case PM_STAND:
		strAni = "Stand";


		break;

	case PM_WALK:
		strAni = "Walk";


		break;

	case PM_STAB:
		strAni = "Stab";
		m_fStopLimitTime = 0.9f;

		m_bStop = true;

		break;

	case PM_SWING1:
		strAni = "Swing1";

		m_fStopLimitTime = 0.9f;
		m_bStop = true;

		break;

	case PM_SWING2:
		strAni = "Swing2";
		m_fStopLimitTime = 0.9f;
		m_bStop = true;

		break;

	case PM_SWING3:
		strAni = "Swing3";
		m_fStopLimitTime = 0.9f;
		m_bStop = true;

		break;

	case PM_SLASH:
		strAni = "Slash";
		m_fStopLimitTime = 1.5f; // 
		m_bStop = true;

		break;

	}

	
	// 보고 있는 방향에 따라서 처리한다.
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

	// 공격시에만 생성.
	AttackEffect();
	
}

void CPlayer::ChangeNextMotion()
{
	// 기존에 하고 있던 모션 -> Default
	// 기존에 하고 있던 모션 -> Default가 아닌 모션 (덮어 씌우기의 개념이기 때문에 해당 클립으로 교환 !)

	// 사실 이것에 대한 처리는 Stop과 연계가 되는 친구들만 한정되기 때문에 . . . 처리를 하지 않는 것도 있다.


	string strAni;

	switch (m_ePlayerMotion)
	{
	case PM_WALK:

		if (m_bMove)
		{
			strAni = "Walk";
			m_ePlayerMotion = PM_WALK;
		}
		else
		{
			strAni = "Stand";
			m_ePlayerMotion = PM_STAND;
		}
		break;
	case PM_HIT: // to default


		break;

	case PM_JUMP:

		if (KEYKEY("Jump"))
		{
			strAni = "Jump";
			m_ePlayerMotion = PM_JUMP;
		}
		else
		{
			strAni = "Stand";
			m_ePlayerMotion = PM_STAND;
		}
	

		break;
		
	case PM_PRONE:

		// 지금 상태를 보고, 계속 PRONE을 할지 말지 결정.


		if (KEYKEY("Prone"))
		{
			strAni = "Prone";
			m_ePlayerMotion = PM_PRONE;
		}
		else
		{
			strAni = "Stand";
			m_ePlayerMotion = PM_STAND;
		}

		break;




	

	/*case PM_PRONE_ATTACK:

		if ( (KEYKEY("Prone")) && (KEYKEY("Attack")) )
		{
			strAni = "ProneAttack";
			m_ePlayerMotion = PM_PRONE_ATTACK;

			m_bStop = true;
			m_fStopLimitTime = 0.9f;

		}
		else if (KEYKEY("Prone"))
		{
			strAni = "Prone";
			m_ePlayerMotion = PM_PRONE;
		}
		else
		{
			strAni = "Alert";
			m_bAlert = true;
			m_bProne = false;

			m_ePlayerMotion = PM_ALERT;
		}

		break;*/

	case PM_SLASH:
	case PM_SWING1:
	case PM_SWING2:
	case PM_SWING3:
	case PM_STAB: // alert로 ! 

		strAni = "Alert";
		m_bAlert = true;
		m_bProne = false;

		m_ePlayerMotion = PM_ALERT;

		// 스킬의 경우에는 좀 세분화 되어 나중에 따로 한다.
	}


	// 보고 있는 방향에 따라서 처리한다.
	switch (m_eDir)
	{
	case MD_LEFT:
		strAni += "Left";

		break;

	case MD_RIGHT:
		strAni += "Right";

		break;
	}

	if (strAni.size() == 4 || strAni.size() == 5)
		return;

	m_pAnimation->ChangeClip(strAni);

	// 공격시에만 생성.
	AttackEffect();
}

void CPlayer::ChangeHitMotion()
{
	// 이미 x 축 넉백은 true인 상태다.

	if(!m_bJump) // 점프인 경우 날라감...
	m_bKnockBackY = true;

	m_ePlayerMotion = PM_HIT;
	ChangeMotion();
}

void CPlayer::RenderPosUpdate()
{

	// 애니메이션에 따라서 RenderPos의 위치는 달라진다.
	// 이곳에서 그것을 처리할 것이다.
	// 구체적으로는 tPos를 기준으로 어느 정도를 떨어지게 할지 구한다.

	switch (m_ePlayerMotion)
	{
		case PM_ALERT:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 12.f;
				m_tRenderPos.y = m_tPos.y;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y;

				break;
			}

			break;

		case PM_DIE:

			break;

		case PM_HIT:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x -12.f;
				m_tRenderPos.y = m_tPos.y + 2.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y + 2.f;

				break;
			}

			break;

		case PM_JUMP:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y - 9.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x - 7.f;
				m_tRenderPos.y = m_tPos.y - 9.f;

				break;
			}

			break;

		case PM_PRONE:


			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 28.f;
				m_tRenderPos.y = m_tPos.y + 27.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y + 27.f;

				break;
			}

			break;

		case PM_PRONE_ATTACK:
	
			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 48.f; //
				m_tRenderPos.y = m_tPos.y + 27.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y + 27.f;

				break;
			}

			break;

		case PM_ROPE:

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

		case PM_SIT:
			switch (m_eDir)
			{
			case MD_LEFT:
				break;

			case MD_RIGHT:
				break;
			}

			break;

		case PM_STAND:


			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 9.f;
				m_tRenderPos.y = m_tPos.y;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y;

				break;
			}

		
			break;

		case PM_WALK:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 15.f;
				m_tRenderPos.y = m_tPos.y;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y;

				break;
			}

			break;

		case PM_STAB:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 60.f;
				m_tRenderPos.y = m_tPos.y + 2.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y + 2.f;

				break;
			}

			break;

		case PM_SWING1:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 13.f;
				m_tRenderPos.y = m_tPos.y -4.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x - 15.f;
				m_tRenderPos.y = m_tPos.y - 4.f;
				break;
			}
			break;

		case PM_SWING2:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 53.f;
				m_tRenderPos.y = m_tPos.y - 3.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x - 16.f;
				m_tRenderPos.y = m_tPos.y - 3.f;

				break;
			}

			break;

		case PM_SWING3:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 28.f;
				m_tRenderPos.y = m_tPos.y - 12.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x - 18.f;
				m_tRenderPos.y = m_tPos.y - 12.f;

				break;
			}

			break;

		case PM_SLASH:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 52.f;
				m_tRenderPos.y = m_tPos.y - 13.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y - 13.f;

				break;
			}

			break;

	}
}

void CPlayer::RenderAnimation(HDC hDC, int x, int y)
{
	wstring strAni;

	switch (m_ePlayerMotion) 
	{
	case PM_ALERT:
		strAni = L"Alert";

		break;

	case PM_DIE:
		strAni = L"Dead";

		break;

	case PM_HIT:
		strAni = L"Hit";


		break;

	case PM_JUMP:
		strAni = L"Jump";

		break;

	case PM_PRONE:
		strAni = L"Prone";

		break;

	case PM_PRONE_ATTACK:
		strAni = L"ProneAttack";



		break;

	case PM_ROPE:
		strAni = L"Rope";

		break;

	case PM_SIT:
		strAni = L"Sit";


		break;

	case PM_STAND:
		strAni = L"Stand";


		break;

	case PM_WALK:
		strAni = L"Walk";


		break;

	case PM_STAB:
		strAni = L"Stab";


		break;

	case PM_SWING1:
		strAni = L"Swing1";



		break;

	case PM_SWING2:
		strAni = L"Swing2";

		break;
	case PM_SWING3:
		strAni = L"Swing3";

		break;

	case PM_SLASH:
		strAni = L"Slash";

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

	TextOut(hDC, x, y, strAni.c_str(), strAni.size());
}

void CPlayer::AttackEffect()
{
	string strEffect;

	switch (m_ePlayerMotion)
	{
	case PM_SWING1:


		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "Swing1SlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;
			break;

		case MD_RIGHT:
			strEffect = "Swing1SlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;


			break;
		}

		break;

	case PM_SWING2:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "Swing2SlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "Swing2SlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_SWING3:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "Swing3SlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "Swing3SlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_STAB:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "StabSlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "StabSlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_PRONE_ATTACK:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "ProneSlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "ProneSlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_SLASH:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "SlashBladeLeft";
			m_eAttack = AT_SKILL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "SlashBladeRight";
			m_eAttack = AT_SKILL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_JUMP:
		if (!m_bDoubleJump || m_bDoubleEffect) // 더블점프가 off 거나, 이미 생성한 경우
			return;


		switch (m_eDir)
		{
		case MD_LEFT:
			DoubleJumpEffectLeft();
			break;

		case MD_RIGHT:
			DoubleJumpEffectRight();
			break;
		}

		m_bDoubleEffect = true;

		return;

	default:
		return;
	}


	CEffect* pEffect = FIND(CEffect)->FindObj(strEffect)->Clone();

	pEffect->SetObj(this);

	CAnimation* pAni = pEffect->GetAnimation();

	if (pAni) // 애니메이션이 있다면...
	{
		pAni->SetObj(pEffect);
		pAni->ChangeClip();
	}

	GET_SINGLE(CSceneManager)->GetObjList("DefaultLayer").push_back(pEffect);
}

void CPlayer::CollisionRopeStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	switch (pDest->GetShape())
	{
	case ST_RECT_PORTAL:
		m_bPortal = true;

		break;

	case ST_PIXEL:
		m_bRopeCollision = true;
		m_tRopePos = ((CPixel*)pDest)->GetRopePos();

		break;

	case ST_RECT_MONSTER_ATTACK:
		break;

	}


}

void CPlayer::CollisionRopeStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	switch (pDest->GetShape())
	{
	case ST_RECT_PORTAL:
	case ST_PIXEL:
		break;
	case ST_RECT_MONSTER_ATTACK:

		
		m_bBlend = true;

		m_fBlendTime = 0.f;
		m_fHitTime = 0.f;
		m_bHit = true;

		m_fKnockBackYPx = 0.f;
		m_fKnockBackYTime = 0.f;

		m_fBlendLimitTime = 0.f;

		m_fHitLimitTime = 0.f;

		m_iGold = 50000;


		// 플레이어가 날라가는 속도랑 몬스터의 속도가 다름... 날라가는 속도.. 거리도 다름...
		// 검정 효과 줘야함. 넉백후에 모션 안 끝남.

		m_iBlend = 250;



		// 방향은 몬스터가 바라보는 방향으로 날라간다.
		// 근데 지금 처리가. 몬스터 기준으로 해서, 반대로 넣어야 한다.
		// 몬스터의 방향의 반대 방향

		// 방향 세이브
		// 충돌한 위치를 상대적으로 알아야 한다.

		// 두 충돌체 P 와 M 이 있을 때
		// P.x >= M.x 인 경우는 오른쪽에서 충돌이라고 볼 수 있다.
		// else 는 왼쪽에서 충돌한 것이다.

		// 둘다 Shape 타입으로 포지션을 뽑아올 수 있다.

		float SrcX = ((CShape*)pSrc)->GetPos().x;
		float DestX = ((CShape*)pDest)->GetPos().x;

		// 근데 동시에 반대로 날라간다는 것을 명심해야 한다.
		
		if (SrcX >= DestX) // 오른쪽
		{
			m_eHitDir = MD_LEFT;
		}
		else // 왼쪽
		{
			m_eHitDir = MD_RIGHT;
		}

		break;

	}
}

void CPlayer::CollisionRopeEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	switch (pDest->GetShape())
	{
	case ST_RECT_PORTAL:
		m_bPortal = false;
		break;

	case ST_PIXEL:
		m_bRopeCollision = false;
		m_bRopeCycle = false;
		m_bFalling = true;

		break;

	case ST_RECT_MONSTER_ATTACK:
		break;
	}

}




void CPlayer::CollisionTerrainStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{

	
	// 붙었는데.. Jump 중이면 점프를 끝내지 않는다.


	// 맞아도 계속 올라가다가,  횟수가 끝나면, End

}


void CPlayer::CollisionTerrainStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{

	// 점프중이 아닌데 붙었으면 처리하면 된다.
	if (!m_bJump)
	{
		// Change모션에서 내가 원하는 상황이 아닌데 적용이 되어 애니메이션 스킵이 일어나는 경우가 있다.

		
		if(!m_bStop && !m_bProne)
			ChangeNextMotion();

		// 명확한 처리
		m_bJumpCycle = false;

		m_bDoubleJumpCycle = false;

		m_bFalling = false;

		m_bDoubleJump = false;

		m_bDoubleEffect = false;

	}

	if (m_bRopeCycle)
	{
		m_bRopeCycle = false;
		m_eDir = MD_RIGHT;
		m_ePlayerMotion = PM_STAND;
		ChangeMotion();
	}
}

void CPlayer::CollisionTerrainEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	// 픽셀과 충돌이 끝났다는 것은 언제나 falling on을 의미한다.
	m_bFalling = true;

	// 그렇다는 것은 언제나 fallingCount를 이용한다는 의미
	m_iFallingCount = 0;
	
}

void CPlayer::CollisionLeftStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{

}

void CPlayer::CollisionLeftStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bLeftEnable = false;
}

void CPlayer::CollisionLeftEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bLeftEnable = true;
}

void CPlayer::CollisionRightStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	
}

void CPlayer::CollisionRightStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bRightEnable = false;
}

void CPlayer::CollisionRightEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bRightEnable = true;

}

bool CPlayer::Init()
{

	m_eDir = MD_RIGHT;

	SetInfo(1, 100, 50, 0, 10000, 10, 15, 200.f); // 130 모험가는 느리다. 

	m_ePlayerMotion = PM_STAND;

	m_tPos = POSITION(600.f, 340.f);

	m_eAttack = AT_NONE;

	// 지금 플레이어 애니메이션의 구조상, 사진이 모두 같은 크기는 아니기 때문에, 
	// 애니메이션의 크기로 셋팅을 해두고 돌려야 맞는 것 같다.

	// 구체적으로는 ChangeClip마다 텍스처의 이미지 크기로 RenderSize를 셋팅하면 된다.


	// 애니메이션 예 : 스킬 모션

	// 의 충돌 범위를 잘 구현해야 한다.


	// 플레이어 폴더에서 안 한 것.
	// 스킬, Icon

	
	SetObj(OT_PLAYER, NULL);

	m_pAnimation = CreateAnimation("Player");

	// Alert Left
	m_pAnimation->AddFrameClip(L"Player/Alert/Left/Alert",
		"AlertLeft", AO_TIME_RETURN, 2.4f, 1.2f, 3,
		_SIZE(59.f, 65.f), 0.f, 0.f,
		59.f, 65.f, 0, 128, 0);


	// Alert Right
	m_pAnimation->AddFrameClip(L"Player/Alert/Right/Alert",
		"AlertRight", AO_TIME_RETURN, 2.4f, 1.2f, 3,
		_SIZE(59.f, 65.f), 0.f, 0.f,
		59.f, 65.f, 0, 128, 0);

	// Dead
	m_pAnimation->AddFrameClip(L"Player/Dead/Dead",
		"Dead", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(47.f, 63.f), 0.f, 0.f,
		47.f, 63.f, 0, 128, 0);

	// HIT 또한 잠깐 멈추고 피격. 


	// Hit Left
	m_pAnimation->AddFrameClip(L"Player/Hit/Left/Hit",
		"HitLeft", AO_LOOP, 2.f, 1.f, 3,
		_SIZE(59.f, 65.f), 0.f, 0.f,
		59.f, 65.f, 0, 128, 0);

	// Hit Right
	m_pAnimation->AddFrameClip(L"Player/Hit/Right/Hit",
		"HitRight", AO_LOOP, 2.f, 1.f, 3,
		_SIZE(59.f, 65.f), 0.f, 0.f,
		59.f, 65.f, 0, 128, 0);

	
	// Jump Left 점프는 클립이 바뀌기 전까지는 계속
	m_pAnimation->AddFrameClip(L"Player/Jump/Left/Jump",
		"JumpLeft", AO_LOOP, 0.f, 0.5f, 1,
		_SIZE(54.f, 73.f), 0.f, 0.f,
		54.f, 73.f, 0, 128, 0);

	// Jump Right 
	m_pAnimation->AddFrameClip(L"Player/Jump/Right/Jump",
		"JumpRight", AO_LOOP, 0.f, 0.5f, 1,
		_SIZE(54.f, 73.f), 0.f, 0.f,
		54.f, 73.f, 0, 128, 0);

	// Prone Left
	m_pAnimation->AddFrameClip(L"Player/Prone/Left/Prone",
		"ProneLeft", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(75.f, 40.f), 0.f, 0.f,
		75.f, 40.f, 0, 128, 0);

	// Prone Right
	m_pAnimation->AddFrameClip(L"Player/Prone/Right/Prone",
		"ProneRight", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(75.f, 40.f), 0.f, 0.f,
		75.f, 40.f, 0, 128, 0);

	

	// Rope 의 적용은 조금 생각을 해봐야 한다.
	// 아마 따로, 함수처리를 통해서 시간을 0으로 할 듯.
	// 함수 처리는 나중에
	// 타고 있는데 움직이지 않는 경우에 함수 처리로
	// 진행을 막는다.
	m_pAnimation->AddFrameClip(L"Player/Rope/Rope",
		"Rope", AO_LOOP, 0.f, 1.f, 2,
		_SIZE(47.f, 67.f), 0.f, 0.f,
		47.f, 67.f, 0, 128, 0);

	// Sit Left
	m_pAnimation->AddFrameClip(L"Player/Sit/Left/Sit",
		"SitLeft", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(90.f, 103.f), 0.f, 0.f,
		90.f, 103.f, 0, 128, 0);

	// Sit Right
	m_pAnimation->AddFrameClip(L"Player/Sit/Right/Sit",
		"SitRight", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(90.f, 103.f), 0.f, 0.f,
		90.f, 103.f, 0, 128, 0);

	// SKILL은 따로함.

	

	// Attack 관련 ======================

	// ProneAttack Left
	m_pAnimation->AddFrameClip(L"Player/ProneAttack/Left/ProneAttack",
		"ProneAttackLeft", AO_LOOP, 0.f, 0.75f, 2,
		_SIZE(95.f, 46.f), 0.f, 0.f,
		95.f, 46.f, 0, 128, 0);

	// ProneAttack Right
	m_pAnimation->AddFrameClip(L"Player/ProneAttack/Right/ProneAttack",
		"ProneAttackRight", AO_LOOP, 0.f, 0.75f, 2,
		_SIZE(95.f, 46.f), 0.f, 0.f,
		95.f, 46.f, 0, 128, 0);

	// Swing1 Left
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing1/Left/Swing",
		"Swing1Left", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(75.f, 102.f), 0.f, 0.f,
		75.f, 102.f, 0, 128, 0);

	// Swing1 Right
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing1/Right/Swing",
		"Swing1Right", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(75.f, 102.f), 0.f, 0.f,
		75.f, 102.f, 0, 128, 0);

	// Swing2 Left
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing2/Left/Swing",
		"Swing2Left", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(115.f, 68.f), 0.f, 0.f,
		115.f, 68.f, 0, 128, 0);

	// Swing2 Right
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing2/Right/Swing",
		"Swing2Right", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(115.f, 68.f), 0.f, 0.f,
		115.f, 68.f, 0, 128, 0);

	// Swing3 Left
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing3/Left/Swing",
		"Swing3Left", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(93.f, 78.f), 0.f, 0.f,
		93.f, 78.f, 0, 128, 0);

	// Swing3 Right
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing3/Right/Swing",
		"Swing3Right", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(93.f, 78.f), 0.f, 0.f,
		93.f, 78.f, 0, 128, 0);

	// Stab Left
	m_pAnimation->AddFrameClip(L"Player/Stab/Left/Stab",
		"StabLeft", AO_LOOP, 0.f, 0.75f, 2,
		_SIZE(108.f, 63.f), 0.f, 0.f,
		108.f, 63.f, 0, 128, 0);

	// Stab Right
	m_pAnimation->AddFrameClip(L"Player/Stab/Right/Stab",
		"StabRight", AO_LOOP, 0.f, 0.75f, 2,
		_SIZE(108.f, 63.f), 0.f, 0.f,
		108.f, 63.f, 0, 128, 0);

	// Skill SlashBlast

	// Slash Left
	m_pAnimation->AddFrameClip(L"Player/Slash/Left/Slash",
		"SlashLeft", AO_LOOP, 0.f, 1.5f, 8,
		_SIZE(100.f, 78.f), 0.f, 0.f,
		100.f, 78.f, 0, 128, 0);

	// Slash Right
	m_pAnimation->AddFrameClip(L"Player/Slash/Right/Slash",
		"SlashRight", AO_LOOP, 0.f, 1.5f, 8,
		_SIZE(100.f, 78.f), 0.f, 0.f,
		100.f, 78.f, 0, 128, 0);

	// Attack 관련 =================== 끝
	// Stand Left
	m_pAnimation->AddFrameClip(L"Player/Stand/Left/Stand",
		"StandLeft", AO_LOOP, 0.f, 1.2f, 3,
		_SIZE(56.f, 69.f), 0.f, 0.f,
		56.f, 69.f, 0, 128, 0);

	// Stand Right
	m_pAnimation->AddFrameClip(L"Player/Stand/Right/Stand",
		"StandRight", AO_LOOP, 0.f, 1.2f, 3,
		_SIZE(56.f, 69.f), 0.f, 0.f,
		56.f, 69.f, 0, 128, 0);

	// Walk Left
	m_pAnimation->AddFrameClip(L"Player/Walk/Left/Walk",
		"WalkLeft", AO_LOOP, 0.f, 0.75f, 4,
		_SIZE(62.f, 66.f), 0.f, 0.f,
		62.f, 66.f, 0, 128, 0);

	// Walk Right
	m_pAnimation->AddFrameClip(L"Player/Walk/Right/Walk",
		"WalkRight", AO_LOOP, 0.f, 0.75f, 4,
		_SIZE(62.f, 66.f), 0.f, 0.f,
		62.f, 66.f, 0, 128, 0);

	m_pAnimation->SetObj(this);

	m_pAnimation->SetDefaultClip("StandRight");

	// 시작할 때는 Obj 잡고 ChangeClip
	m_pAnimation->ChangeClip("StandRight");

	m_tRopePos = GetPoint(0, 0);

	m_fKnockBackYPx = 0.f;
	m_fKnockBackYTime = 0.f;

	m_fKnockBackYLimitPx = 65.f;
	m_fKnockBackXDist = 2.5f;
	m_fKnockBackLimitPx = 100.f;

	m_bFalling = true;
	m_bJumpCycle = true; 

	m_bMinimap = true;

	m_bInventory = false;
	m_bAbility = false;
	m_bSkill = false;

	m_bDoubleEffect = false;

	m_bProne = false;

	m_bPortal = false;

	m_bRightEnable = true;

	m_bLeftEnable = true;

	m_bAlert = false;

	m_bSlash = false;

	m_bKnockBackY = false;

	m_bDoubleJump = false;

	m_bDoubleJumpCycle = false;

	m_bShop = false;

	m_fBlendTime = 0.f;

	m_fBlendLimitTime = 0.f;

	m_iExpMax = 100;

	m_fJumpLimitPx = 70.f;

	m_fHitLimitTime = 1.f; // 확인용.


	m_fDoubleJumpPx = 0.f;
	m_fDoubleJumpTime = 0.f;
	m_fDoubleJumpLimitPx = 380; // 350 + 30 * Lv

	m_fDoubleJumpDist = 3.8f; // 0.01s당 가야하는 범위. limitpx / 100.f
	// 캐릭터 기본 크기
	m_tSize.x = 47.f;
	m_tSize.y = 66.f;



	// COLLIDER 

	// ==  왼쪽 장애물
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(3.f, 3.f), GetPoint(0, 61), ST_RECT_OBSTACLE);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CPlayer::CollisionLeftStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionLeftStay);

	pShape->AddCollisionFunc(CS_END, this, &CPlayer::CollisionLeftEnd);

	AddCollider(pShape);



	// 오른쪽 장애물

	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(3.f, 3.f), GetPoint(44, 61), ST_RECT_OBSTACLE);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CPlayer::CollisionRightStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionRightStay);

	pShape->AddCollisionFunc(CS_END, this, &CPlayer::CollisionRightEnd);

	AddCollider(pShape);


	// 바닥용 
	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(24.f, 1.f), GetPoint(14, 66), ST_RECT_TERRAIN);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CPlayer::CollisionTerrainStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionTerrainStay);

	pShape->AddCollisionFunc(CS_END, this, &CPlayer::CollisionTerrainEnd);

	AddCollider(pShape);

	// 일단 NONE
	// 왼쪽 기본공격 사거리 좌우 55

	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(55.f, 96.f), GetPoint(-55, -15), ST_RECT_NORMAL_LEFT);

	pShape->SetObj(this);

	AddCollider(pShape);

	// 오른쪽 기본공격 사거리

	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(55.f, 96.f), GetPoint(47, -15), ST_RECT_NORMAL_RIGHT);

	pShape->SetObj(this);

	AddCollider(pShape);

	// 스킬 사거리  플레이어로부터 70px 만큼 떨어져서 size 150 x 210

	// 스킬 발동 후 잠깐 후에 활성화. 스킬 effect 나가면 비활성화.

	// 오른쪽
	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(130.f, 170.f), GetPoint(117, -40), ST_RECT_SKILL_RIGHT);

	pShape->SetObj(this);

	AddCollider(pShape);


	// 왼쪽
	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(130.f, 170.f), GetPoint(-200, -40), ST_RECT_SKILL_LEFT);

	pShape->SetObj(this);

	AddCollider(pShape);


	
	CShape* pHead = new CShape;

	pHead->InitColliderShape(_SIZE(24.f, 24.f), GetPoint(12, 26), ST_RECT_ROPE);

	pHead->SetObj(this);

	pHead->AddCollisionFunc(CS_START, this, &CPlayer::CollisionRopeStart);

	pHead->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionRopeStay);

	pHead->AddCollisionFunc(CS_END, this, &CPlayer::CollisionRopeEnd);
	
	AddCollider(pHead);

	// ==

	m_bPhysics = true; // 일단 플레이어만.



	return true;
}

void CPlayer::Input(float fDeltaTime)
{



	if (KEYDOWN("Minimap")) // M
	{
		m_bMinimap = !m_bMinimap;

		if (m_bMinimap)
		{
			// 순서 변경
			GET_SINGLE(CSceneManager)->SetChange(((CUIPanel*)m_pMinimap)->GetZOrder());
			
		}
	}

	if (KEYDOWN("Skill")) // K
	{
		m_bSkill = !m_bSkill;

		if (m_bSkill)
		{
			GET_SINGLE(CSceneManager)->SetChange(((CUIPanel*)m_pSkill)->GetZOrder());
		}
	}

	if (KEYDOWN("Ability")) // J
	{
		m_bAbility = !m_bAbility;

		if (m_bAbility)
		{
			GET_SINGLE(CSceneManager)->SetChange(((CUIPanel*)m_pAbility)->GetZOrder());
		}
	}

	if (KEYDOWN("Inventory")) // I
	{
		m_bInventory = !m_bInventory;

		if (m_bInventory)
		{
			GET_SINGLE(CSceneManager)->SetChange(((CUIPanel*)m_pInventory)->GetZOrder());
		}

	}


	if (KEYDOWN("ESC"))
	{
		// ZOrder 우선 순위에 따라서 닫아버림.
		GET_SINGLE(CSceneManager)->ESCKey();

	}


	// 넉백 상태라면 Input을 하지 않는다.
	if (m_bKnockBack)
		return;
		
	if (m_bStop)
		return;


	if (m_bShop)
		return;
	

	if (!m_bDoubleJumpCycle && m_bFalling && KEYDOWN("Jump")) // 더블점프 아직 안 한 상태 && 점프중에 점프를 누른 경우.
	{
		m_bDoubleJump = true;

		m_bDoubleJumpCycle = true;

		m_fJumpPx = 35.f; 

		m_eDoubleDir = m_eDir;

		m_bJump = true;

		m_bDoubleJumpCycle = true;

		m_bDoubleEffect = false;

		m_fDoubleJumpTime = 0.f;

		m_fDoubleJumpPx = 0.f;

		m_ePlayerMotion = PM_JUMP;

		ChangeMotion();
		
	}

	/*
		여담 : 전사는 무언가 던지질 않는다. 그냥 가서 때린다.
		따라서 Angle을 이용하는 것은 사용하지 않는다.
	*/


	// KEYKEY 조심 && 같이 사용 X

	
	 
	if (KEYDOWN("MoveUp") && m_bRopeCollision || KEYPRESS("MoveUp") && m_bRopeCollision)
	{
		// 사다리 타기 시작

		m_bAlert = false;

		m_bRopeCycle = true;

		m_bJumpCycle = false;

		m_bDoubleJump = false;

		m_bDoubleJumpCycle = false;

		JumpEnd();

		m_bMove = false;

		// Rope의 위치로 이동한다.

		m_tPos.x = m_tRopePos.x - 23.f;

		m_ePlayerMotion = PM_ROPE;

		m_pAnimation->ChangeClip("Rope");

	}
	else if (KEYPRESS("Prone") && !m_bJumpCycle && !m_bRopeCycle)
	{
 		
		if (KEYKEY("Jump")) // 아래 + 점프는 아래로 떨구진 않고 위로 점프만 가능하게 해줌. 근데 이건 rope일 때는 막아야 함.
		{

			m_bProne = false;

			m_bAlert = false;

			m_fJumpPx = 0.f;

			Jump();

			m_ePlayerMotion = PM_JUMP;

			m_bJumpCycle = true;

			ChangeMotion();

		}
		else if (KEYKEY("Attack")) // 웅크리기 + 공격
		{
			m_ePlayerMotion = PM_PRONE_ATTACK;

			m_bProne = true;

			m_bAlert = false;

			ChangeMotion();

		
		}
		else  // 그냥 웅크리기
		{
			m_ePlayerMotion = PM_PRONE;

			m_bProne = true;

			m_bAlert = false;

			ChangeMotion();
		}
			

	}
	else if (KEYPRESS("Jump") && !m_bJumpCycle || KEYDOWN("Jump") && !m_bJumpCycle) // 점프를 하고 있는데 점프 버튼을 눌러서 방향키를 씹어버린다면 곤란... 그러니까 막아야 ㅎ마.
	{
		if (m_bRopeCycle)
		{
			// 점프를 하는데 로프를 타고 있었다면 ..

			if (KEYKEY("MoveRight") || KEYKEY("MoveLeft")) // 로프를 타고 그냥 점프는 불가능 좌우 누르면서는 가능.
			{

				m_fJumpPx = 35.f;

				Jump();

				m_bRopeCycle = false;

				m_bAlert = false;

				m_ePlayerMotion = PM_JUMP;

				m_bJumpCycle = true;

				ChangeMotion();
			}
		
		}
		else if (!m_bFalling) // 로프는 안 타고 있는데 falling = false (바닥에 붙음)
		{
			m_fJumpPx = 0.f;

			Jump();

			m_bRopeCycle = false;

			m_bAlert = false;

			m_ePlayerMotion = PM_JUMP;

			m_bJumpCycle = true;

			ChangeMotion();
		}
	}
	else if (KEYDOWN("Attack") && !m_bFalling && !m_bRopeCycle && !m_bProne
		|| KEYPRESS("Attack") && !m_bFalling && !m_bRopeCycle && !m_bProne) // a
	{
		m_bAlert = false;

		// 기본 공격 3개 중에 랜덤으로.
		int idx = rand() % 4;

		switch (idx)
		{
		case 0:
			m_ePlayerMotion = PM_SWING1;
			break;

		case 1:
			m_ePlayerMotion = PM_SWING2;

			break;

		case 2:
			m_ePlayerMotion = PM_SWING3;
			break;


		case 3:
			m_ePlayerMotion = PM_STAB;
			break;

		
		}

		ChangeMotion();

		return;
		
		// 여기서 보내주면, KEY UP으로 인해서 stand모션을 출력할 수 있다.
	
	}
	else if (KEYDOWN("Slash") && !m_bFalling && !m_bRopeCycle && !m_bProne || KEYPRESS("Slash") && !m_bFalling && !m_bRopeCycle && !m_bProne)
	{
		m_bAlert = false;
		m_ePlayerMotion = PM_SLASH;

		m_bSlash = true;

		ChangeMotion();

	}
	

	else if (KEYPRESS("MoveLeft") && !m_bRopeCycle && m_bLeftEnable && !m_bDoubleJump
		|| KEYDOWN("MoveLeft") && !m_bRopeCycle && m_bLeftEnable && !m_bDoubleJump)
	{

		// 점프중인데 자신의 방향과 반대로 가려고 한다면 막아야 한다. 이전에 입력한 값을 가지고 활용하는 것이다.

				switch (m_eDir)
				{
				case MD_RIGHT:

					m_eDir = MD_LEFT;

					m_pAnimation->SetDefaultClip("StandLeft");

					break;

				case MD_LEFT:
					break;
				}


				m_ePlayerMotion = PM_WALK;

				ChangeMotion();

				MoveX(fDeltaTime);

				m_bAlert = false;

	

	}

	else if (KEYPRESS("MoveRight") && !m_bRopeCycle && m_bRightEnable && !m_bDoubleJump
		|| KEYDOWN("MoveRight") && !m_bRopeCycle && m_bRightEnable && !m_bDoubleJump)
	{

			switch (m_eDir)
			{
			case MD_RIGHT:
				break;

			case MD_LEFT:
				m_eDir = MD_RIGHT;
				m_pAnimation->SetDefaultClip("StandRight");

				break;
			}


			m_ePlayerMotion = PM_WALK;

			if (!m_bJumpCycle)
				ChangeMotion();


			MoveX(fDeltaTime);

			m_bAlert = false;

	}


	



	if (KEYPRESS("MoveUp") && m_bRopeCycle) 
	{
		m_tPos.y -= fDeltaTime * m_fSpeed;
		m_bMove = true;
		m_bAlert = false;

	}

	if (KEYPRESS("MoveDown") && m_bRopeCycle) 
	{
		m_tPos.y += fDeltaTime * m_fSpeed;
		m_bMove = true;
		m_bAlert = false;

	}




	if (KEYUP("Prone"))
	{
		if (!m_bJumpCycle && !m_bRopeCycle)
		{
			m_ePlayerMotion = PM_STAND;
			ChangeMotion();
		}

		m_bProne = false;

	}

	if (KEYUP("MoveLeft") && !m_bProne && !m_bJumpCycle && !m_bRopeCycle)
	{
		m_ePlayerMotion = PM_STAND;
		ChangeMotion();
	}

	if (KEYUP("MoveRight") && !m_bProne && !m_bJumpCycle && !m_bRopeCycle)
	{
		m_ePlayerMotion = PM_STAND;
		ChangeMotion();
	}

	

	
}

void CPlayer::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);

}

void CPlayer::Update(float fDeltaTime)
{
	if (m_bWorriorPotion)
	{
		m_fWorriorPotion += fDeltaTime;

		if (m_fWorriorPotion >= 10.f) // 180
		{
			m_bWorriorPotion = false;
			m_fWorriorPotion = 0.f;

			// Return
			m_iAttackMin -= 50; // 5
			m_iAttackMax -= 50;
		}
	}

	if (m_bSpeedPotion)
	{
		m_fSpeedPotion += fDeltaTime;

		if (m_fSpeedPotion >= 10.f)
		{
			m_bSpeedPotion = false;
			m_fSpeedPotion = 0.f;

			// Return
			m_fSpeed -= 100.f; // 20
		}
	}

	if (m_bProne && !KEYPRESS("Prone")) // 근데 여기서 모션 스킵의 가능성이 있다.
	{

		switch(m_ePlayerMotion)
		{
		case PM_PRONE_ATTACK:
		case PM_SWING1:
		case PM_SWING2:
		case PM_SWING3:
		case PM_STAB:

			m_bProne = false;

			break;

		default:
			m_bProne = false;
			m_ePlayerMotion = PM_STAND;
			ChangeMotion();
			break;
		}
	
	}

	if (m_bBlend)
	{
		m_fBlendTime += fDeltaTime;
		m_fBlendLimitTime += fDeltaTime;

		if (m_fBlendTime >= 0.1f)
		{
			m_fBlendTime -= 0.1f;
			
			switch (m_iBlend)
			{
			case 250:
				m_iBlend -= 100;
				break;

			case 150:
				m_iBlend += 100;
				break;
			}
		}

		// 0.1 초 간격으로 희끄무리 찐 희끄무리 찐 반복하다가 false ! 하면 끝.
		if (m_fBlendLimitTime >= 1.5f)
		{
			m_bBlend = false;

			// 스탠드로 (기본)
			m_ePlayerMotion = PM_STAND;

			ChangeMotion();

			// 이것이 기본이고, 나중에 입력받으면 덮어 씌움.
		}
	}

	if (m_bKnockBackY) // 날라가는거 방지
	{

		// 플레이어의 넉백은 y축으로 날라가는 처리까지 하는 것이다.
		// 모션까지는 바꾸지 말고 그냥 hit으로 하되, 넉빽 ! 0.2 동안 x축으로 넉백 + 넉업
		
		m_fKnockBackYTime += fDeltaTime;

		if (m_fKnockBackYTime >= 0.01f)
		{
			m_fKnockBackYTime -= 0.01f; // 0.2 s

			// 방향에 상관없이 y축으로 이동
			m_tPos.y -= 5.f;
			m_fKnockBackYPx += 5.f;

			if (m_fKnockBackYPx >= m_fKnockBackYLimitPx)
			{
				m_fKnockBackYPx -= m_fKnockBackYLimitPx;

				m_tPos.y += m_fKnockBackYPx;

				m_bKnockBackY = false;
			}
			
		}
	}


	if (m_bFalling)
	{
		m_ePlayerMotion = PM_JUMP;
		ChangeMotion();

		if (m_bDoubleJump)
		{
			m_fDoubleJumpTime += fDeltaTime;

			if (m_fDoubleJumpTime >= 0.01f)
			{
 				m_fDoubleJumpTime -= 0.01f; // 0.01s 에 350+30*x 기본 380으로 잡고.
				

				switch (m_eDoubleDir)
				{
				case MD_LEFT:
					if (!m_bLeftEnable)
						break;
					
					// 처음에 날고, 점점 낮아지는 처리.
					
					m_tPos.x -= m_fDoubleJumpDist;
					m_fDoubleJumpPx += m_fDoubleJumpDist;

					break;

				case MD_RIGHT:
					if (!m_bRightEnable)
						break;

					m_tPos.x += m_fDoubleJumpDist;
					m_fDoubleJumpPx += m_fDoubleJumpDist;


					break;
				}
				
			}

			if (m_fDoubleJumpPx >= m_fDoubleJumpLimitPx)
			{
				m_fDoubleJumpPx -= m_fDoubleJumpLimitPx;

				// 여기서 남은 값을 이동거리에서 제외한다.

				switch (m_eDoubleDir)
				{
				case MD_LEFT:
					m_tPos.x += m_fDoubleJumpPx;
					break;

				case MD_RIGHT:
					m_tPos.x -= m_fDoubleJumpPx;

					break;
				}

				m_bDoubleJump = false;

			}


		}


	}

	switch (m_ePlayerMotion)
	{
	case PM_ROPE:
		
		if (!m_bRopeCycle)  // rope인데 cycle이 아니면 잘못된 처리.
		{
			m_ePlayerMotion = PM_STAND;
			ChangeMotion();
			break;
		}

		if (!m_bMove)
			break;

	


		// 움직이는 경우에만 Update
	default:
		CMoveObj::Update(fDeltaTime);
		break;
	}



	
	if (m_bStop)
	{
		m_fStopTime += fDeltaTime;

		if (m_fStopTime >= m_fStopLimitTime)
		{
			m_fStopTime = 0.f;
			m_bStop = false;

			// 이전 모션에 따라서 Default가 아닌 다른 모션으로 돌아가야 하는 친구들을 상대한다.
			ChangeNextMotion();
		}
	}

	// 일단은 플레이어를 따라오도록 해준다. RenderPos가 나중에는 비율에 맞춰서
	// 상황에 따라 띄워주고 다시 Stand의 상황대로 처리

	
	

	RenderPosUpdate();

}

void CPlayer::Render(HDC hDC, float fDeltaTime)
{
	wchar_t strPos[100];


	if (CInGameScene::m_bCollider)
	{

		// 몬스터는 빨간 사각형이 공격 범위이고, 플레이어는 그냥 자기 범위 출력.
		// 기본 크기 사각형

		POSITION tPos;
		tPos.x = m_tPos.x - CInGameScene::l;
		tPos.y = m_tPos.y - CInGameScene::t;

		POSITION tPos2 = tPos + m_tSize;

		PEN->DrawRect(PC_RED, tPos, tPos2);


		////// 웅크리기
		////m_tSize.x = m_tPos.x + 73.f;
		////m_tSize.y = m_tPos.y + 66.f;

		////PEN->DrawRect(PC_RED, GetScreenPos(POSITION(m_tPos.x - 26.f, m_tPos.y + 27.f))
		////	, GetScreenPos(m_tSize));

		swprintf_s(strPos, L"Player (%.1f, %.1f)", m_tPos.x, m_tPos.y);
		TextOut(hDC, 200, 20, strPos, lstrlen(strPos));

		swprintf_s(strPos, L"RopeCollision : (%d)", m_bRopeCollision ? 1 : 0);
		TextOut(hDC, 200, 40, strPos, lstrlen(strPos));

		swprintf_s(strPos, L"PortalCollision : (%d)", m_bPortal ? 1 : 0);
		TextOut(hDC, 200, 60, strPos, lstrlen(strPos));

		// Prone
		swprintf_s(strPos, L"Prone : %d", m_bProne ? 1 : 0);
		TextOut(hDC, 50, 20, strPos, lstrlen(strPos));

		// Jump
		swprintf_s(strPos, L"JumpCycle : %d", m_bJumpCycle ? 1 : 0);
		TextOut(hDC, 50, 40, strPos, lstrlen(strPos));

		// Falling
		swprintf_s(strPos, L"Falling : %d", m_bFalling ? 1 : 0);
		TextOut(hDC, 50, 60, strPos, lstrlen(strPos));

		// Rope
		swprintf_s(strPos, L"RopeCycle : %d", m_bRopeCycle ? 1 : 0);
		TextOut(hDC, 50, 80, strPos, lstrlen(strPos));

		// Left
		swprintf_s(strPos, L"Left Enable : %d", m_bLeftEnable ? 1 : 0);
		TextOut(hDC, 50, 100, strPos, lstrlen(strPos));

		//Right
		swprintf_s(strPos, L"Right Enable : %d", m_bRightEnable ? 1 : 0);
		TextOut(hDC, 50, 120, strPos, lstrlen(strPos));

		//Right
		swprintf_s(strPos, L"Alert : %d", m_bAlert ? 1 : 0);
		TextOut(hDC, 50, 140, strPos, lstrlen(strPos));

		// Slash
		swprintf_s(strPos, L"Slash : %d", m_bSlash ? 1 : 0);
		TextOut(hDC, 50, 160, strPos, lstrlen(strPos));

		// Double Jump
		swprintf_s(strPos, L"Double JumpCycle : %d", m_bDoubleJumpCycle ? 1 : 0);
		TextOut(hDC, 50, 180, strPos, lstrlen(strPos));

		// Hit
		swprintf_s(strPos, L"Hit : %d", m_bKnockBack ? 1 : 0);
		TextOut(hDC, 50, 200, strPos, lstrlen(strPos));

	}

	CMoveObj::Render(hDC, fDeltaTime);



	// 일단. enum문 잡고 거기서 충돌 터지는 걸로 해놨다.
	// 한번 처리 & 충돌중일때 collider의 list를 날려서 다시 처리할 수 있도록 해놓고...
	// Collider 포인터 이용 비활성화를 해둬야 한다. 플레이어의 enum을 none으로. 그러면 더 이상 처리 없고
	// 처음 한번 처리, 이후에 비활성화 처음 상태 그대로. && 다시 공격하면 반복.

	swprintf_s(strPos, L"Blend : %d", m_bBlend ? 1 : 0);
	TextOut(hDC, 50, 220, strPos, lstrlen(strPos));


	m_eAttack = AT_NONE;

	
}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}
