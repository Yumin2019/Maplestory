#include "Object\CStage.h"
#include "Collision/CPixel.h"


CStage::CStage() 
{
	m_eObject = OT_MAP;
}

CStage::CStage(const CStage & map) :
	CStaticObj(map)
{
	m_eMapType = map.m_eMapType;
}


CStage::~CStage()
{
}


bool CStage::Init()
{
	return true;
}

void CStage::Input(float fDeltaTime)
{
}

void CStage::Collision(float fDeltaTime)
{

}

void CStage::Update(float fDeltaTime)
{
}

void CStage::Render(HDC hDC, float fDeltaTime)
{
	CStaticObj::Render(hDC, fDeltaTime);
}

CStage * CStage::Clone()
{
	return new CStage(*this);
}
