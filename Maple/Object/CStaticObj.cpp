#include "CStaticObj.h"



CStaticObj::CStaticObj()
{
}

CStaticObj::CStaticObj(const CStaticObj & StaticObj) :
	CObj(StaticObj)
{
}


CStaticObj::~CStaticObj()
{
}

bool CStaticObj::Init()
{
	return true;
}

void CStaticObj::Input(float fDeltaTime)
{
	CObj::Input(fDeltaTime);
}

void CStaticObj::Collision(float fDeltaTime)
{
	CObj::Collision(fDeltaTime);

}

void CStaticObj::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);

}

void CStaticObj::Render(HDC hDC, float fDeltaTime)
{
	CObj::Render(hDC, fDeltaTime);



}
