#pragma once

#include "../Game.h"

class CCollisionManager
{
	DECLARE_SINGLE(CCollisionManager);




private:
	list<class CObj*> m_CollisionList;
	
public:
	bool Init();

	void AddObj(class CObj* pObj);
	void Collision(float fDeltaTime);

	bool CollisionGo(CObj* pSrc, CObj* pDest);

	void Collision2(CObj* pSrc, CObj* pDest, float fDeltaTime);
};

