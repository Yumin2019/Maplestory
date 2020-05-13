#pragma once
#include "CObj.h"


class CMoveObj :
	public CObj
{
protected:
	CMoveObj();
	CMoveObj(const CMoveObj& move);
	virtual ~CMoveObj();

protected:
	MOVE_DIRECTION	    m_eDir;

	// Hit, Attack애니메이션이 도는 동안 가만히 있어야 하는 시간
	bool				m_bStop;
	float				m_fStopTime;
	float				m_fStopLimitTime; // 클래스 마다 따로 처리.
	// 특정 애니메이션에 대하여 유동적으로 변한다.

	bool				m_bPhysics;
	float				m_fGravityTime;

	float				m_fJumpTime;
	bool				m_bJump;
	bool				m_bMove;
	bool				m_bFalling;

	float				m_fJumpPx;
	float				m_fJumpLimitPx;

	int					m_iFallingCount;


	int					m_iHP;
	int					m_iHPMax;
	int					m_iMP;
	int					m_iMPMax;

	int					m_iExp;
	int					m_iGold;

	int					m_iAttackMin;
	int					m_iAttackMax;

	int					m_iArmor;

	int					m_iLevel;

	float				m_fSpeed;


	// 넉백

	bool				m_bKnockBack;
	float				m_fKnockBackTime;
	float				m_fKnockBackPx;
	float				m_fKnockBackLimitPx;

	float				m_fKnockBackXDist;


	MOVE_DIRECTION		m_eHitDir;

	/// 넉백이 된다고 바로 Hit모션이 되는 것은 아니기에, 대기시간에 이용할 변수가 필요하다.

	bool				m_bHit;
	float				m_fHitTime;
	float				m_fHitLimitTime; 

public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CMoveObj* Clone() = 0;

	// JUMP
	virtual void ChangeMotion() = 0;
	virtual void ChangeNextMotion() = 0;
	virtual void RenderPosUpdate() = 0;
	virtual void RenderAnimation(HDC hDC, int x, int y) = 0;
	virtual void ChangeHitMotion() = 0;


protected:
	void Jump();
	void JumpEnd();

	// 바닥용
	virtual void CollisionTerrainStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime) = 0;
	virtual void CollisionTerrainStay(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime) = 0;
	virtual void CollisionTerrainEnd(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime) = 0;

public:

	// Armor

	int GetArmor() const
	{
		return m_iArmor;
	}

	void AddArmor(int i)
	{
		m_iArmor += i;
	}

	// 방향

	MOVE_DIRECTION GetDir() const
	{
		return m_eDir;
	}

	void SetDir(MOVE_DIRECTION eDir)
	{
		m_eDir = eDir;
	}
	
	// HIT

	void SetNockBack(bool hit)
	{
		m_bKnockBack = hit;
	}

	bool GetNockBack() const
	{
		return m_bKnockBack;
	}

	void SetJumpPx(float fPx)
	{
		m_fJumpLimitPx = fPx;
	}

	void SetInfo(int iLevel, int iHP, int iMP, int iEXP, int iGold, int iAttackMin, int iAttackMax, float fSpeed);
	
	// HP

	int GetHP() const
	{
		return m_iHP;
	}

	int GetHPMax() const
	{
		return m_iHPMax;
	}

	void AddHP(int a)
	{
		m_iHP += a;

		if (m_iHP > m_iHPMax)
		{
			m_iHP = m_iHPMax;
		}
		
	}

	// MP

	int GetMP() const
	{
		return m_iMP;
	}

	int GetMPMax() const
	{
		return m_iMPMax;
	}

	void AddMP(int a)
	{
		m_iMP += a;

		if (m_iMP > m_iMPMax)
		{
			m_iMP = m_iMPMax;
		}
		
	}

	// Gold

	int GetGold() const
	{
		return m_iGold;
	}

	void AddGold(int a)
	{
		m_iGold += a;

	}

	// EXP 

	int GetExp() const
	{
		return m_iExp;
	}

	// Attack

	int GetAttackMin() const
	{
		return m_iAttackMin;
	}

	int GetAttackMax() const
	{
		return m_iAttackMax;
	}






	// Move 함수

	void MoveX(float fTime)
	{
		m_tPos.x += fTime * m_fSpeed * m_eDir;

		m_bMove = true;
	}

	void MoveY(float fTime) // ...
	{
		m_tPos.y += fTime * m_fSpeed * m_eDir;

		m_bMove = true;

	}

	// 스피드 증가 함수

	void AddSpeed(float f)
	{
		m_fSpeed += f;
	}


	// Speed

	float GetSpeed() const
	{
		return m_fSpeed;
	}

	void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

};

