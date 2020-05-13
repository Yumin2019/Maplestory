#pragma once
#include "CMoveObj.h"


class CPlayer :
	public CMoveObj
{
public:
	CPlayer();
	CPlayer(const CPlayer& Player);
	~CPlayer();

private:
	PLAYER_MOTION m_ePlayerMotion;
	bool		 m_bProne;
	bool		 m_bJumpCycle;

	// ������ Ÿ��������

	POINT		 m_tRopePos; // Rope��ǥ
	bool		 m_bRopeCycle;
	bool		 m_bRopeCollision;

	bool	     m_bPortal;
	bool		 m_bAlert;
	

	// �����̵� �������̵�
	bool		 m_bRightEnable;
	bool		 m_bLeftEnable;

	int			 m_iExpMax;

	bool	     m_bSlash;

	bool		m_bShop; // ��
	bool		m_bMinimap; // �̴ϸ�
	bool		m_bInventory; // �κ��丮
	bool		m_bSkill; // ��ų
	bool		m_bAbility; // �ɷ�ġ

	// ���� ����

	bool			m_bDoubleJumpCycle;
	bool			m_bDoubleJump;

	float			m_fDoubleJumpDist;

	bool			m_bDoubleEffect;

	MOVE_DIRECTION  m_eDoubleDir; // ���������� ����
	float				m_fDoubleJumpPx; 
	float				m_fDoubleJumpLimitPx;
	float			m_fDoubleJumpTime;

	// ���� ���� & �ӵ���� ����

	float			m_fSpeedPotion;
	float			m_fWorriorPotion;
	bool			m_bSpeedPotion;
	bool			m_bWorriorPotion;

	ATTACK_TYPE		m_eAttack;

	//	y�� �̵�

	float			m_fKnockBackYPx;
	float			m_fKnockBackYLimitPx;
	float			m_fKnockBackYTime;
	bool			m_bKnockBackY;

	// ���� �ð�

	float			m_fBlendTime;
	float			m_fBlendLimitTime; // �� ��ø�ð�.

	// ũ��Ƽ�� �⺻ 5%

	int				m_iCriticalPercent;

	// ���Ľ� Ȯ��

	int				m_iStancePercent;


	//str dex int luk

	/*
	STR, DEX, Int LUK
	�� , ��ø, ����, ���
	strength, intellect, Dexterity, Lucky
	
	*/

	int			m_iStrength;
	int			m_iIntellect;
	int			m_iDexterity;
	int			m_iLucky;

	int			m_iPoint;



	class CShopUI* m_pShop;
	class CSkillPanel* m_pSkill;
	class CMinimap* m_pMinimap;
	class CInventoryPanel* m_pInventory;
	class CAbilityPanel* m_pAbility;

private:
	void DoubleJumpEffectLeft();
	void DoubleJumpEffectRight();

public:

	void SetUI(class CShopUI* pShop, class CSkillPanel* pSkill,
		class CMinimap* pMinimap, class CInventoryPanel* pInven,
		class CAbilityPanel* pAbility)
	{
		m_pShop = pShop;
		m_pSkill = pSkill;
		m_pMinimap = pMinimap;
		m_pInventory = pInven;
		m_pAbility = pAbility;
	}

	// Point

	int GetRemainPoint() const
	{
		return m_iPoint;
	}

	void AddRemainPoint(int i)
	{
		m_iPoint += i;
	}

	// Str, Int, Dex, Luk

	int GetStr() const
	{
		return m_iStrength;
	}

	int GetInt() const
	{
		return m_iIntellect;
	}

	int GetDex() const
	{
		return m_iDexterity;
	}

	int GetLuk() const
	{
		return m_iLucky;
	}

	void AddStr(int i)
	{
		m_iStrength += i;
	}

	void AddInt(int i)
	{
		m_iIntellect += i;
	}

	void AddDex(int i)
	{
		m_iDexterity += i;
	}

	void AddLuk(int i)
	{
		m_iLucky += i;
	}
	
	class CShopUI* GetShopUI() const
	{
		return m_pShop;
	}

	// ���ý� Ȯ��

	int GetStancePercent() const
	{
		return m_iStancePercent;
	}

	void AddStandPercent(int i)
	{
		m_iStancePercent += i;

		if (m_iStancePercent > 100)
			m_iStancePercent = 100;
	}

	// ũ��Ƽ�� Ȯ��

	int GetCriticalPercent() const
	{
		return m_iCriticalPercent;
	}

	void AddCriticalPercent(int i)
	{
		m_iCriticalPercent += i;

		if (m_iCriticalPercent > 100)
			m_iCriticalPercent = 100;
	}

	// ���� ���� & ���ǵ� ����
	void SetSpeedPotion()
	{
		if (m_bSpeedPotion)
		{
			m_fSpeed -= 100.f;
			m_fSpeedPotion = 0.f;
		}

		m_bSpeedPotion = true;
		m_fSpeed += 100.f; // 20
	}

	void SetWorriorPotion()
	{

		if (m_bWorriorPotion)
		{
			m_fWorriorPotion = 0.f;
			m_iAttackMin -= 50;
			m_iAttackMax -= 50;
		}

		m_bWorriorPotion = true;
		m_iAttackMin += 50;// 5
		m_iAttackMax += 50;
	}

	void SetInventory(bool Inven)
	{
		m_bInventory = Inven;
	}

	void SetShop(bool shop)
	{
		m_bShop = shop;
	}

	bool GetShop() const
	{
		return m_bShop;
	}

	void SetAbility(bool avi)
	{
		m_bAbility = avi;
	}

	void SetSkill(bool skill)
	{
		m_bSkill = skill;
	}

	void SetMinimap(bool mini)
	{
		m_bMinimap = mini;
	}

	// �ɷ�ġ
	bool GetAbility() const
	{
		return m_bAbility;
	}
	

	// ��ųâ
	bool GetSkill() const
	{
		return m_bSkill;
	}


	// �κ��丮
	bool GetInventory() const
	{
		return m_bInventory;
	}


	// �̴ϸ�
	bool GetMinimap() const
	{
		return m_bMinimap;
	}

	


	void SetAttack(ATTACK_TYPE eType)
	{
		m_eAttack = eType;
	}

	ATTACK_TYPE GetAttack() const
	{
		return m_eAttack;
	}

	void SetSlash(bool slash)
	{
		m_bSlash = slash;
	}

	void SetDoubleJump(bool doubleJump)
	{
		m_bDoubleJump = doubleJump;
	}

	bool GetFalling() const
	{
		return m_bFalling;
	}

	void SetAlert(bool bAlert)
	{
		m_bAlert = bAlert;
	}

	int GetExpMax() const
	{
		return m_iExpMax;
	}

	// �������� ����ġ ���̺� ���� ���� �ɷ�ġ .. �� �÷��̾� �����̴�.

	virtual void ChangeMotion();

	// ���� ��� ���¿� ���� RenderPos ��, ���� �ð�, Ư�� �ִϸ��̼��� �������ִ� �Լ���.
	virtual void ChangeNextMotion();

	virtual void ChangeHitMotion();


private:

	virtual void RenderPosUpdate();

	virtual void RenderAnimation(HDC hDC, int x, int y);

	void AttackEffect();
	

	// �ٴڿ�
	virtual void CollisionTerrainStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	virtual void CollisionTerrainStay(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	virtual void CollisionTerrainEnd(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	
	// ���� ����
	void CollisionLeftStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	void CollisionLeftStay(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	void CollisionLeftEnd(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);

	// ������ ����
	void CollisionRightStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	void CollisionRightStay(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	void CollisionRightEnd(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);

	// Rope
	void CollisionRopeStart(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	void CollisionRopeStay(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);
	void CollisionRopeEnd(class CCollider* pSrc,
		class CCollider* pDest, float fDeltaTime);

public:
	void SetPlayerMotion(PLAYER_MOTION eMotion)
	{
		m_ePlayerMotion = eMotion;
	}

	PLAYER_MOTION GetPlayerMotion() const
	{
		return m_ePlayerMotion;
	}

public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CPlayer* Clone();

};

