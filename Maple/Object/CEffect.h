#pragma once
#include "CStaticObj.h"

/*
MOVEOBJ에 만든 것이 너무 많은 거의 뭐 몬스터 / 플레이어 전용...

순수하게 이펙트만. 
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

	// 시작후 대기 시간 (보이지 않는 시간)

	float			m_fInvisibleTime;
	float			m_fInvisibleLimitTime;
	bool			m_bVisible;

	// MOVE TIMe 움직이는 친구만 사용
	float			m_fMoveTime;

			 CObj*  m_pObj; // 따라다닐 물체 없으면 NULL
	
			 POINT	m_tLeftTop; // 따라다닐 물체에서의 비율

			 

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

