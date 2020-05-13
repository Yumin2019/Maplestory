#pragma once
#include "../Object/CStaticObj.h"


class CUI :
	public CStaticObj
{
public:
	CUI();
	CUI(const CUI& ui);
	virtual ~CUI();

	
public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CUI* Clone() = 0;
};

