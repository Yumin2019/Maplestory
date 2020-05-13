#include "CMonster.h"



CMonster::CMonster()
{
	m_fMoveTime = 0.f;
	m_fMoveLimitTime = 0.f;
}


CMonster::~CMonster()
{
}

CMonster::CMonster(const CMonster & move) :
	CMoveObj(move)
{
	m_eMotion = MM_STAND;
	m_eName = move.m_eName;

	m_fMoveTime = 0.f;
	m_fMoveLimitTime = 0.f;
}



bool CMonster::Init()
{
	return false;
}

void CMonster::Input(float fDeltaTime)
{
	CMoveObj::Input(fDeltaTime);
}

void CMonster::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);
}

void CMonster::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);

	

}

void CMonster::Render(HDC hDC, float fDeltaTime)
{
	CMoveObj::Render(hDC, fDeltaTime);


}
