#pragma once

#include "../CRef.h"

class CCollider :
	public CRef
{
protected:
	CCollider();
	CCollider(const CCollider& col);
	virtual ~CCollider();

protected:
	friend class CObj;
	friend class CCollisionManager;

protected:
	class CObj*			m_pObj;
	SHAPE_TYPE			m_eShape;
	list<CCollider*>	m_CollisionList;
	POINT				m_tLeftTop; // 오브젝트와 충돌체간의 비율


	// 충돌 상태에 따른 함수 포인터 지정
	list<function<void(CCollider*, CCollider*,float)>> m_pFuncList[CS_SIZE];

public:

	void AddCollisionFunc(COLLISION_STATE eState,
		void(*pFunc)(CCollider*, CCollider*, float))
	{
		function<void(CCollider*, CCollider*, float)> func;

		func = bind(pFunc, placeholders::_1, 
			placeholders::_2, placeholders::_3);

		m_pFuncList[eState].push_back(func);
	}

	template<typename T>
	void AddCollisionFunc(COLLISION_STATE eState, T* pObj,
		void(T::*pFunc)(CCollider*, CCollider*, float))
	{
		function<void(CCollider*, CCollider*, float)> func;

		func = bind(pFunc, pObj, placeholders::_1,
			placeholders::_2, placeholders::_3);

		m_pFuncList[eState].push_back(func);
	}

	void CallFunction(COLLISION_STATE eState,
		CCollider* pDest, float fDeltaTime)
	{
		list<function<void(CCollider*, CCollider*, float)>>::iterator iter;
		list<function<void(CCollider*, CCollider*, float)>>::iterator iterEnd = m_pFuncList[eState].end();

		for (iter = m_pFuncList[eState].begin(); iter != iterEnd; ++iter)
			(*iter)(this, pDest, fDeltaTime);
	}
	

	void AddCollider(CCollider* pCollider)
	{
		m_CollisionList.push_back(pCollider);
	}

	bool CheckPrevCollision(CCollider* pCollider)
	{
		list<CCollider*>::iterator iter;
		list<CCollider*>::iterator iterEnd = m_CollisionList.end();

		for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter) == pCollider)
				return true;
		}

		return false;
	}

	void EraseCollisionList(CCollider* pCollider)
	{
		list<CCollider*>::iterator iter;
		list<CCollider*>::iterator iterEnd = m_CollisionList.end();

		for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter) == pCollider)
			{
				m_CollisionList.erase(iter);
				return;
			}
		}
	}

	void EraseCollisionList()
	{
		m_CollisionList.clear();
	}
	

	void SetLeftTop(int left, int top)
	{
		POINT Pos;
		Pos.x = left;
		Pos.y = top;
		m_tLeftTop = Pos;
	}

	void SetLeftTop(POINT tPos)
	{
		m_tLeftTop = tPos;
	}

public:

	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CCollider* Clone() = 0;
	virtual void RenderText(HDC hDC) = 0;





public:
	// Object Set
	
	void SetObj(class CObj* pObj)
	{
		m_pObj = pObj;
	}

	class CObj* GetObj() const
	{
		return m_pObj;
	}

	
	// Shape

	SHAPE_TYPE GetShape() const
	{
		return m_eShape;
	}

	void SetShape(SHAPE_TYPE eShape)
	{
		m_eShape = eShape;
	}

	
};

