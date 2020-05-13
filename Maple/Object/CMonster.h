#pragma once
#include "CMoveObj.h"
class CMonster :
	public CMoveObj
{
protected:
	CMonster();
	virtual ~CMonster();
	CMonster(const CMonster& move);

protected:
	MONSTER_NAME		m_eName;
	MONSTER_MOTION		m_eMotion;

	float				m_fMoveTime;
	float				m_fMoveLimitTime;


public:
	// ¹Ù´Ú¿ë
	virtual void CollisionTerrainStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime) = 0;
	virtual void CollisionTerrainStay(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime) = 0;
	virtual void CollisionTerrainEnd(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime) = 0;


	// ¾ç ¿· ¸ó½ºÅÍ Obstacle
	virtual void CollisionObstacleStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime) = 0;


public:
	
	MOVE_DIRECTION GetHitDir() const
	{
		return m_eHitDir;
	}

	void SetMonsterName(MONSTER_NAME eName)
	{
		m_eName = eName;
	}

	MONSTER_NAME GetMonsterName() const
	{
		return m_eName;
	}

	MONSTER_MOTION GetMonsterMotion() const
	{
		return m_eMotion;
	}

	void SetMontserMotion(MONSTER_MOTION eMotion)
	{
		m_eMotion = eMotion;
	}
	

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CMonster* Clone() = 0;

	// JUMP
	virtual void ChangeMotion() = 0;
	virtual void ChangeNextMotion() = 0;
	virtual void RenderPosUpdate() = 0;
	virtual void RenderAnimation(HDC hDC, int x, int y) = 0;
	virtual void ChangeHitMotion() = 0;


};

