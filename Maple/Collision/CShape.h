#pragma once
#include "CCollider.h"


class CShape :
	public CCollider
{
public:	
	CShape();
	CShape(const CShape& shape);
	 ~CShape();

private:
	POSITION			m_tPos; // 충돌체의 위치
	_SIZE				m_tSize; // 충돌체의 크기

public:
	static				int m_iPortal;
	static			    int m_iRect;

public:
	bool InitColliderShape(_SIZE tSize, POINT tLeftTop, SHAPE_TYPE eType);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CShape* Clone();
	virtual void RenderText(HDC hDC);

public:

	POSITION GetWorldPos(const POSITION tPos);
	
	// Collider Pos , size, 

	void SetPos(float x, float y)
	{
		m_tPos = POSITION(x, y);
	}

	void SetSize(float sizex, float sizey)
	{
		m_tSize = _SIZE(sizex, sizey);
	}




	void SetPos(POSITION tPos)
	{
		m_tPos = tPos;
	}

	void SetSize(_SIZE tSize)
	{
		m_tSize = tSize;
	}


	POSITION GetPos() const
	{
		return m_tPos;
	}

	_SIZE GetSize() const
	{
		return m_tSize;
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


};

