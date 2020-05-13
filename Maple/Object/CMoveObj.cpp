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
	m_eDir = move.m_eDir; // ��ġ�� ���� ����.
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
	m_fKnockBackLimitPx = 30.f; // �÷��̾�� �� ����

	// �ӵ� ���� �ִ� px�� ����. 

	// �ӵ� �� �Ÿ� ������ �ʴ�. ���� �÷��̾� �浹ü or �浹 ���� �ذ�����. 
	// ���� ���� ����� ���������� ���� ���ϰ� �ϴ� ��.

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
		Hit�� �˹��� ���� true�� �� ����.
		�߰��� ó���� ���� �ʴ� �� ����, �̰� �������� �𸥴�.
	
	*/

	if (m_bHit)
	{
		m_fHitTime += fDeltaTime;

		if (m_fHitTime >= m_fHitLimitTime)
		{
			// Hit Time �� �°� ���� �ٷ� 0
			m_bHit = false;

			// ��� �ð��� ������, Hit�� false �˹� true
			m_bKnockBack = true;

			// �˹��� true�� �Ǹ�, ����� �ٲ���Ѵ�.

			ChangeHitMotion();
			// �ϴ� �÷��̾�� hit�� �� ����, ���͸� Hit������� �ٲ��, ���ÿ� �˹�, ���Ŀ� move �ٽ� cycle
		}
	}
	// �˹��� on�̶��, update���� ���󰡾� �Ѵ�.


	// ���ʹ� ���� �ʰ�, �÷��̾ ��� ���.

	else if (m_bKnockBack)
	{
		// �ݴ� �������� x �� �̵�. && ���� ��¦ ���󰡾� �Ѵ�.

		m_fKnockBackTime += fDeltaTime;

		if (m_fKnockBackTime >= 0.01f) // 0.01f�� 0.8 px �� �̵��ϸ� 0.5s�� 40px�� �̵��ϸ�, 0.5�� ���� hit���.
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

						// ���⼭ ���� ��ŭ �ݴ� �������� 
						m_tPos.x += m_fKnockBackPx;

						// LimitPx���� ���������Ƿ� NockBack�� off
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
		// �������� ó��.

		// ������ �Ʒ��� �������� �͵� falling, ������ �������� �͵� falling

		

		switch(m_bFalling)
		{
			case true: // ���� ON

				switch (m_bJump)
				{
				case true: // falling on jump on == ���� �ߴ�.

					m_fGravityTime += fDeltaTime;

					if (m_fGravityTime >= 0.009f)
					{
						m_fGravityTime -= 0.009f;
						m_tPos.y -= 2.2f;
						m_fJumpPx += 2.2f;

						if (m_fJumpPx >= m_fJumpLimitPx)
						{
							m_bJump = false;
							// falling = true �������°�.

							// �Ѿ ������ŭ �ٽ� ������.

							m_fJumpPx -= m_fJumpLimitPx;

							m_tPos.y += m_fJumpPx;
							
						}
					

					}
					break;

				case false: // falling on jump false == ������ ������ ���ž� �Ѵ�.

						// ���ž� �Ѵ�. 

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

			case false: // �ٴڿ� ���� ��� or �÷��̾��� ��� Prone Jump 

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
	// �������� �ִ� ��쿡 ���� ���� ( �������� on )
	if (!m_bFalling)
	{
		m_bFalling = true;
		m_bJump = true;

	}
}

void CMoveObj::JumpEnd()
{
	// �ٴڿ� ����
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
