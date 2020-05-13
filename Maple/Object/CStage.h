#pragma once
#include "CStaticObj.h"


class CStage :
	public CStaticObj
{

public:
	CStage();
	CStage(const CStage& map);
	virtual ~CStage();


private:
	MAP_TYPE m_eMapType;


public:


	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CStage* Clone();



public:
	void SetMapType(MAP_TYPE eType)
	{
		m_eMapType = eType;
	}

	MAP_TYPE GetMapType() const
	{
		return m_eMapType;
	}


};

