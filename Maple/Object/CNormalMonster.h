#pragma once
#include "CMonster.h"
class CNormalMonster :
	public CMonster
{
public:
	CNormalMonster();
	~CNormalMonster();
	CNormalMonster(const CNormalMonster& mon);


private:
	float		m_fHPBarTime;
	bool		m_bHPBar;


public:

	bool GetHPBar() const
	{
		return m_bHPBar;
	}

	void SetHPBar(bool bHp)
	{
		m_bHPBar = bHp;
	}


public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CNormalMonster* Clone();

	// 바닥용
	virtual void CollisionTerrainStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	virtual void CollisionTerrainStay(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	virtual void CollisionTerrainEnd(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);

	// 중심
	virtual void CollisionObstacleStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime);

private:
	virtual void ChangeMotion();
	virtual void ChangeNextMotion();
	virtual void RenderPosUpdate();
	virtual void RenderAnimation(HDC hDC, int x, int y);

	// 몬스터가 맞은 경우에 대한 처리

	void CollisionPlayerAttackStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);


	virtual void ChangeHitMotion();

	void GreenMushroomRenderPos();
	void BlueMushroomRenderPos();
	void TreeRenderPos();
	void RedSnailRenderPos();
	void BlueSnailRenderPos();
	void OrangeMushroomRenderPos();
	void GolemRenderPos();

	void MushroomColliderInit();
	void OrangeMushroomColliderInit();
	void SnailColliderInit();
	void TreeColliderInit();
	void GolemColliderInit();


};

