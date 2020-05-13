#include "CEffect.h"



CEffect::CEffect()
{
	m_eObject = OT_EFFECT;
	m_pObj = NULL;

	m_fDeathLimitTime = 0.f;
	m_fDeathTime = 0.f;

	m_fInvisibleTime = 0.f;

	m_bBlend = true;
	m_iBlend = 0;

	m_eEffect = ET_NONE;

	m_fMoveTime = 0.f;

}

CEffect::CEffect(const CEffect & effect) :
	CStaticObj(effect)
{
	m_eObject = OT_EFFECT;
	m_pObj = NULL;

	m_fDeathLimitTime = effect.m_fDeathLimitTime;
	m_fDeathTime = 0.f;

	m_tLeftTop = effect.m_tLeftTop;

	m_bVisible = effect.m_bVisible;
	m_fInvisibleLimitTime = effect.m_fInvisibleLimitTime;
	m_fInvisibleTime = 0.f;

	m_bBlend = true;
	

	m_eEffect = effect.m_eEffect;

	m_fMoveTime = 0.f;


	switch(m_eEffect)
	{
		case ET_SKILL:
			m_iBlend = 150;
			break;

		case ET_SKILL_LEAF:
			m_iBlend = 250;

			break;

		default:
			m_iBlend = 210;

	}

}


CEffect::~CEffect()
{
}

void CEffect::SetEffect(float fDeathTime, int left, int top, bool bVisible, float fInvisibleLimitTime, EFFECT_TYPE eType)
{
	m_fDeathLimitTime = fDeathTime;

	m_eEffect = eType;

	m_tLeftTop = GetPoint(left, top);

	m_bVisible = bVisible; 

	m_fInvisibleLimitTime = fInvisibleLimitTime;
}

bool CEffect::Init()
{
	return true;
}

void CEffect::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

void CEffect::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);

}

void CEffect::Update(float fDeltaTime)
{
	OffsetUpdate(fDeltaTime);

	m_fDeathTime += fDeltaTime;

	if (m_fDeathTime >= m_fDeathLimitTime)
	{
		Die();
	}

	CStaticObj::Update(fDeltaTime);

	

}

void CEffect::Render(HDC hDC, float fDeltaTime)
{


	// 위치 업데이트 : zorder 에 따라서 update를 하다보면, 이전 위치를 기준으로 잡는 경우가 생긴다.
	// 따라서 모든 오브젝트의 업데이트를 끝낸 후에 renderpos를 계산한다.

	if (m_pObj)
	{
		POSITION tPos = m_pObj->GetCenter();

		tPos.x += m_tLeftTop.x;
		tPos.y += m_tLeftTop.y;

		m_tRenderPos = GetPoint(tPos.x, tPos.y);
	}





	if (m_bVisible)
	{
		m_fInvisibleTime += fDeltaTime;

		if (m_fInvisibleTime >= m_fInvisibleLimitTime)
		{
			m_fInvisibleTime -= m_fInvisibleLimitTime; 
		

			switch (m_eEffect)
			{

			case ET_SKILL: // 150부터 시작. 1.5초 0.3 x 5 중간이 강함.
				
				switch (m_iBlend)
				{
				case 150:
				case 200:
					m_iBlend += 50;
					break;

				default:
					m_iBlend -= 50;
				}

				break;

			case ET_SKILL_LEAF: // 강 중 하

					m_iBlend -= 70;

				break;



			default:

				switch (m_iBlend)
				{

				case 210:
					m_iBlend += 40;

					break;

				case 250:
					m_iBlend -= 40;

					break;

				}
			}
				

		}

	

		CStaticObj::Render(hDC, fDeltaTime);
	}
	else
	{
		m_fInvisibleTime += fDeltaTime;

		if (m_fInvisibleTime >= m_fInvisibleLimitTime)
		{
			m_bVisible = true;
			m_fInvisibleTime = 0.f; // 이후에 이 변수는 alpha타임을 위해 쓰인다.
			m_fInvisibleLimitTime = 0.066f;
		}
	}

}

CEffect * CEffect::Clone()
{
	return new CEffect(*this);
}

void CEffect::OffsetUpdate(float fTime)
{
	switch (m_eEffect)
	{
	case ET_SKILL:
	case ET_SWING1:
	case ET_SWING2:
	case ET_SWING3:
	case ET_STAB:
		break;

		// 일정 시간이 지났는지 확인하고 지났다면, 오프셋을 바꾼다.
	case ET_PRONE_ATTACK_LEFT:


		m_fMoveTime += fTime;

		if (m_fMoveTime >= 0.35f) 
		{
			m_tLeftTop.x -= 20;
			m_fMoveTime -= 1.f;
		}
		break;

	case ET_PRONE_ATTACK_RIGHT:

		m_fMoveTime += fTime;

		if (m_fMoveTime >= 0.35f) // 2 모션중 1모션이 끝났다면. 오른쪽 20 이동
		{
			m_tLeftTop.x += 20;
			m_fMoveTime -= 1.f;
		}
		break;
	}
}
