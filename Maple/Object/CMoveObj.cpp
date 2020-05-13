#include "CMoveObj.h"
#include "../Scene/CInGameScene.h"



CMoveObj::CMoveObj() :
	m_fSpeed(0.f),
	m_eDir(MD_NONE),
	m_bStop(false),
	m_fStopLimitTime(0.f),
	m_fStopTime(0.f),
	m_fGravityTime(0.f),
	m_bPhysics(true),
	m_bMove(false),
	m_bFalling(false),
	m_bJump(false)
{
	m_iArmor = 0;
	m_fJumpTime = 0;
	m_fJumpLimitPx = 0.f;
	m_fJumpPx = 0.f;
	m_iExp = 0;
	m_iGold = 0;
	m_iHP = 0;
	m_iHPMax = 0;
	m_iMP = 0;
	m_iMPMax = 0;
	m_iAttackMin = 0;
	m_iAttackMax = 0; 
	m_iLevel = 1;
	m_iFallingCount = 0;

	m_bKnockBack = false;
	m_fKnockBackTime = 0.f;
	m_fKnockBackPx = 0.f;
	m_fKnockBackLimitPx = 40.f;

	m_bHit = false;
	m_fHitTime = 0.f;
	m_eHitDir = MD_NONE;
	m_fHitLimitTime = 0.f;

	m_fKnockBackXDist = 1.5f;
}

CMoveObj::CMoveObj(const CMoveObj & move) :
	CObj(move)
{
	m_eHitDir = MD_NONE;
	m_eDir = move.m_eDir; // 위치에 따른 생성.
	m_fStopTime = 0.f;
	m_fGravityTime = 0.f;
	m_fJumpTime = 0.f;
	m_iFallingCount = 0;

	m_bFalling = true;
	m_bPhysics = true;

	m_bMove = true;
	m_bStop = false;
	m_bJump = false;
	m_fJumpLimitPx = 0.f;
	m_fJumpPx = 0.f;
	m_fJumpLimitPx = move.m_fJumpLimitPx;

	m_fStopLimitTime = 0.f;
	m_iArmor = move.m_iArmor;

	m_iHP = move.m_iHP;
	m_iHPMax = m_iHP;
	m_iMP = move.m_iMP;
	m_iMPMax = m_iMP;
	m_iExp = move.m_iExp;
	m_iGold = move.m_iGold;
	m_iAttackMin = move.m_iAttackMin;
	m_iAttackMax = move.m_iAttackMax;
	m_iLevel = move.m_iLevel;
	m_fSpeed = move.m_fSpeed;


	m_bKnockBack = false;
	m_fKnockBackTime = 0.f;
	m_fKnockBackPx = 0.f;
	m_fKnockBackLimitPx = 30.f; // 플레이어는 더 많이

	// 속도 동일 최대 px만 조정. 

	// 속도 및 거리 나쁘지 않다. 이제 플레이어 충돌체 or 충돌 오류 해결하자. 
	// 가장 쉬운 방법은 연속적으로 쓰지 못하게 하는 것.

	m_bHit = false;
	m_fHitTime = 0.f;
	m_fHitLimitTime = 0.f;

	m_fKnockBackXDist = 1.5f;

}


CMoveObj::~CMoveObj()
{
}

bool CMoveObj::Init()
{
	return true;
}

void CMoveObj::Input(float fDeltaTime)
{
	CObj::Input(fDeltaTime);
}

void CMoveObj::Collision(float fDeltaTime)
{
	CObj::Collision(fDeltaTime);

}

void CMoveObj::Update(float fDeltaTime)
{
	/*
		Hit과 넉백은 같이 true일 수 없다.
		중간에 처리가 되지 않는 것 또한, 이것 때문일지 모른다.
	
	*/

	if (m_bHit)
	{
		m_fHitTime += fDeltaTime;

		if (m_fHitTime >= m_fHitLimitTime)
		{
			// Hit Time 은 맞고 나서 바로 0
			m_bHit = false;

			// 대기 시간이 지나면, Hit은 false 넉백 true
			m_bKnockBack = true;

			// 넉백이 true가 되면, 모션을 바꿔야한다.

			ChangeHitMotion();
			// 일단 플레이어는 hit이 안 오고, 몬스터만 Hit모션으로 바뀌고, 동시에 넉백, 이후에 move 다시 cycle
		}
	}
	// 넉백이 on이라면, update에서 날라가야 한다.


	// 몬스터는 뜨지 않고, 플레이어만 잠깐 뜬다.

	else if (m_bKnockBack)
	{
		// 반대 방향으로 x 축 이동. && 위로 살짝 날라가야 한다.

		m_fKnockBackTime += fDeltaTime;

		if (m_fKnockBackTime >= 0.01f) // 0.01f에 0.8 px 을 이동하면 0.5s에 40px을 이동하며, 0.5초 동안 hit모션.
		{
			m_fKnockBackTime -= 0.01f;

			m_fKnockBackPx += m_fKnockBackXDist;

				switch (m_eHitDir)
				{
				case MD_RIGHT:
					m_tPos.x -= m_fKnockBackXDist;


					if (m_fKnockBackPx >= m_fKnockBackLimitPx)
					{
						m_fKnockBackPx -= m_fKnockBackLimitPx;

						// 여기서 남은 만큼 반대 방향으로 
						m_tPos.x += m_fKnockBackPx;

						// LimitPx까지 진행했으므로 NockBack을 off
						m_bKnockBack = false;
						m_fKnockBackPx = 0.f;
					}

					break;

				case MD_LEFT:
					m_tPos.x += m_fKnockBackXDist;

					if (m_fKnockBackPx >= m_fKnockBackLimitPx)
					{

						m_fKnockBackPx -= m_fKnockBackLimitPx;

						m_tPos.x -= m_fKnockBackPx;

						m_bKnockBack = false;
						m_fKnockBackPx = 0;
					}

					break;
				}

			

		}


	}



	if (m_bPhysics)
	{
		// 떨어지는 처리.

		// 위에서 아래로 떨어지는 것도 falling, 점프로 떨어지는 것도 falling

		

		switch(m_bFalling)
		{
			case true: // 점프 ON

				switch (m_bJump)
				{
				case true: // falling on jump on == 점프 했다.

					m_fGravityTime += fDeltaTime;

					if (m_fGravityTime >= 0.009f)
					{
						m_fGravityTime -= 0.009f;
						m_tPos.y -= 2.2f;
						m_fJumpPx += 2.2f;

						if (m_fJumpPx >= m_fJumpLimitPx)
						{
							m_bJump = false;
							// falling = true 떨어지는거.

							// 넘어간 범위만큼 다시 내린다.

							m_fJumpPx -= m_fJumpLimitPx;

							m_tPos.y += m_fJumpPx;
							
						}
					

					}
					break;

				case false: // falling on jump false == 점프가 끊나서 떨궈야 한다.

						// 떨궈야 한다. 

						m_fGravityTime += fDeltaTime;


						if (m_fGravityTime >= 0.009f)
						{
							++m_iFallingCount;

							m_fGravityTime -= 0.009f;

							if (m_iFallingCount <= 10)
								m_tPos.y += 2.2f;
							else if (m_iFallingCount <= 20)
								m_tPos.y += 2.5f;
							else
								m_tPos.y += 3.f;
						}

					break;
				}

			case false: // 바닥에 붙은 경우 or 플레이어의 경우 Prone Jump 

				break;
		}



	}


	CObj::Update(fDeltaTime);

}

void CMoveObj::Render(HDC hDC, float fDeltaTime)
{
	CObj::Render(hDC, fDeltaTime);

	// Animation Print
	RenderAnimation(hDC, m_tPos.x -CInGameScene::l, m_tPos.y - CInGameScene::t - 100.f);


	m_bMove = false;
}


void CMoveObj::Jump()
{
	// 떨어지고 있는 경우에 무시 예외 ( 더블점프 on )
	if (!m_bFalling)
	{
		m_bFalling = true;
		m_bJump = true;

	}
}

void CMoveObj::JumpEnd()
{
	// 바닥에 붙음
	m_bJump = false;
	m_bFalling = false;


}

void CMoveObj::SetInfo(int iLevel, int iHP, int iMP, int iEXP, int iGold, int iAttackMin, int iAttackMax, float fSpeed)
{
	m_iLevel = iLevel;

	m_iHP = iHP;
	m_iHPMax = iHP;
	
	m_iMP = iMP;
	m_iMPMax = iMP;

	m_iExp = iEXP;
	m_iGold = iGold;
	m_iAttackMin = iAttackMin;
	m_iAttackMax = iAttackMax;

	m_fSpeed = fSpeed;
}
