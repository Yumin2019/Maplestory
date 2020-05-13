#pragma once
#include "CObj.h"


class CStaticObj :
	public CObj
{
public:
	CStaticObj();
	CStaticObj(const CStaticObj& StaticObj);
	virtual ~CStaticObj();

	

public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CStaticObj* Clone() = 0;
};

