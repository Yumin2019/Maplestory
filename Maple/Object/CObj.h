#pragma once
#include "../CRef.h"

class CObj :
	public CRef
{
protected:
	CObj();
	CObj(const CObj& Obj);
	virtual ~CObj();

protected:
	friend class CLayer;
	friend class CCreateManager;
	friend class CCollisionManager;



protected:
	bool				m_bLife;
	bool				m_bBlend;
	int					m_iBlend;
	
	// Render�� ����ϴ� ��ǥ.	
	POSITION			m_tRenderPos;


	// ��ų ��Ÿ� ��, �⺻ ���� ��Ÿ��� ��쿡�� ���� ����ϴ� ���� �´�. �÷��̾�� ���͵�.
	// Ŭ���� �ٲٸ�, pos���� �������� RenderPos�� ��������, clip�� ���� RenderPos�� �޶�����.
	// RenderPos���� RenderSize ��ŭ�� ����ϵ�, ���� ��ġ�� tPos�� ���̴�.

	// �̹����� �������� RenderSize�� ����.
	// �������� ��쿡�� clip����
	// ��Ʋ���� ��쿡�� clip�� ��ü�� ������ ���� size�� �����ؾ� ������, ������ �׷� �ʿ䰡 ����.

	// ���� �̵��� ����ϴ� ��ǥ
	POSITION			m_tPos;
	_SIZE				m_tSize; // ������Ʈ�� �⺻ũ�⸦ �ǹ��Ѵ�. ���� �簢���� ����� ����. �µθ��ν�.

	OBJECT_TYPE			m_eObject;
	class CTexture*		m_pTexture;
	class CAnimation*	m_pAnimation;

	list<class CCollider*>	m_ColliderList;




public:


	// ������Ʈ �߰�
	template<typename T>
	static T* CreateObj(class CLayer* pLayer)
	{
		T* pObj = new T;

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}

		pLayer->GetList().push_back(pObj);

		return pObj;
	}


	

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CObj* Clone() = 0;

	void SetObj(OBJECT_TYPE			eObject,
				class CTexture*		pTexture);


public:

	void SetiBlend(int i)
	{
		m_iBlend = i;
	}

	void SetBlend(bool blend)
	{
		m_bBlend = blend;
	}

	bool GetBlend() const
	{
		return m_bBlend;
	}

	// POINT ���ϰ� ����

	POINT GetPoint(int x, int y)
	{
		POINT pos;
		pos.x = x;
		pos.y = y;

		return pos;
	}

	// ���� ��ǥ ����

	POSITION GetPos() const
	{
		return m_tPos;
	}

	void SetPos(POSITION tPos)
	{
		m_tPos = tPos;
	}

	void SetPos(float x, float y)
	{
		m_tPos = POSITION(x, y);
	}

	// Collider Use or Not

	bool ColliderUse()
	{
		return !m_ColliderList.empty();
		// ��������� false ������ true
	}

	// Object Type

	void SetObjectType(OBJECT_TYPE eType)
	{
		m_eObject = eType;
	}

	OBJECT_TYPE GetObjectType() const
	{
		return m_eObject;
	}

	// Collider

	list<class CCollider*>& GetColliderList()
	{
		return m_ColliderList;
	}

	void AddCollider(class CCollider* pColl)
	{
		m_ColliderList.push_back(pColl);
	}

	// ���� ��ǥ -> Screen ��ǥ �ٲٴ� �Լ�

	POSITION GetScreenPos(POSITION tPos);

	POINT GetScreenPos(POINT tPos);

	// ���� center -> screen

	POSITION GetCenterScreenPos(POSITION tCenter);


	// Size
	void SetSize(_SIZE tSize)
	{
		m_tSize = tSize;
	}
	
	void SetSize(float x, float y)
	{
		m_tSize = _SIZE(x, y);
	}

	_SIZE GetSize() const
	{
		return m_tSize;
	}

	_SIZE GetRenderSize() const;

	float GetSizeX() const
	{
		return m_tSize.x;
	}

	float GetSizeY() const
	{
		return m_tSize.y;
	}

	// l t r b

	float GetLeft() const
	{
		return m_tPos.x;
	}

	float GetRight() const
	{
		return GetLeft() + m_tSize.x;
	}

	float GetTop() const
	{
		return m_tPos.y;
	}

	float GetBottom() const
	{
		return GetTop() + m_tSize.y;
	}

	// Center

	float GetCenterX() const
	{
		return (GetLeft() + GetRight()) / 2.f;
	}

	float GetCenterY() const
	{
		return (GetTop() + GetBottom()) / 2.f;
	}

	POSITION GetCenter() const
	{
		return POSITION(GetCenterX(), GetCenterY());
	}


	
public:

	// �ִϸ��̼� ����

	class CAnimation* CreateAnimation(const string& strTag);

	
	// map ������Ÿ��

	void SetTexture(class CTexture* pTexture)
	{
		m_pTexture = pTexture;
	}





public:

	class CAnimation* GetAnimation() const
	{
		return m_pAnimation;
	}

	class CTexture* GetTexture() const
	{
		return m_pTexture;
	}


	// Life
	void Die()
	{
		m_bLife = false;
	}


	bool GetLife() const
	{
		return m_bLife;
	}





	// Render Pos

	void SetRenderPos(POSITION& tPos)
	{
		m_tRenderPos = tPos;
	}
	
	void SetRenderPos(float x, float y)
	{
		m_tRenderPos = POSITION(x, y);
	}
	

	POSITION GetRenderPos() const
	{
		return m_tRenderPos;
	}

	


	void SetAnimation();
	void SetAnimation(const string& strTag);

};

