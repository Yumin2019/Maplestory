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
	
	// Render에 사용하는 좌표.	
	POSITION			m_tRenderPos;


	// 스킬 사거리 및, 기본 공격 사거리의 경우에는 따로 출력하는 것이 맞다. 플레이어든 몬스터든.
	// 클립을 바꾸면, pos값을 기준으로 RenderPos가 정해지며, clip에 따라서 RenderPos도 달라진다.
	// RenderPos에서 RenderSize 만큼을 출력하되, 실제 위치는 tPos인 것이다.

	// 이미지와 프레임은 RenderSize를 지정.
	// 프레임의 경우에는 clip에서
	// 아틀라스의 경우에는 clip을 교체할 때마다 새로 size를 지정해야 하지만, 지금은 그럴 필요가 없다.

	// 실제 이동에 사용하는 좌표
	POSITION			m_tPos;
	_SIZE				m_tSize; // 오브젝트의 기본크기를 의미한다. 작은 사각형을 출력할 예정. 태두리로써.

	OBJECT_TYPE			m_eObject;
	class CTexture*		m_pTexture;
	class CAnimation*	m_pAnimation;

	list<class CCollider*>	m_ColliderList;




public:


	// 오브젝트 추가
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

	// POINT 편리하게 생성

	POINT GetPoint(int x, int y)
	{
		POINT pos;
		pos.x = x;
		pos.y = y;

		return pos;
	}

	// 실제 좌표 관련

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
		// 비어있으면 false 있으면 true
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

	// 월드 좌표 -> Screen 좌표 바꾸는 함수

	POSITION GetScreenPos(POSITION tPos);

	POINT GetScreenPos(POINT tPos);

	// 월드 center -> screen

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

	// 애니메이션 생성

	class CAnimation* CreateAnimation(const string& strTag);

	
	// map 프로토타입

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

