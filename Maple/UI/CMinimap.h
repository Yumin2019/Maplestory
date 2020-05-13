#pragma once
#include "CUIPanel.h"
class CMinimap :
	public CUIPanel
{
public:
	CMinimap();
	CMinimap(const CMinimap& panel);
	~CMinimap();



public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CMinimap* Clone();

private:
	void MouseMoveStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseMoveStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

