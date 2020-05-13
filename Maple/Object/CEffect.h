#pragma once
#include "CStaticObj.h"

/*
MOVEOBJ�� ���� ���� �ʹ� ���� ���� �� ���� / �÷��̾� ����...

�����ϰ� ����Ʈ��. 
*/
class CEffect :
	public CStaticObj
{
public:
	CEffect();
	CEffect(const CEffect& effect);
	~CEffect();

private:

	EFFECT_TYPE		m_eEffect;
	float			m_fDeathTime;
	float			m_fDeathLimitTime;

	// ������ ��� �ð� (������ �ʴ� �ð�)

	float			m_fInvisibleTime;
	float			m_fInvisibleLimitTime;
	bool			m_bVisible;

	// MOVE TIMe �����̴� ģ���� ���
	float			m_fMoveTime;

			 CObj*  m_pObj; // ����ٴ� ��ü ������ NULL
	
			 POINT	m_tLeftTop; // ����ٴ� ��ü������ ����

			 

public:

	void SetEffect(float fDeathTime, int left, int top, bool bVisible, float fInvisibleLimitTime, EFFECT_TYPE eType);



	void SetObj(CObj* pObj)
	{
		m_pObj = pObj;
	}

	CObj* GetObj() const
	{
		return m_pObj;
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

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CEffect* Clone();

private:
	void OffsetUpdate(float fTime);
};

