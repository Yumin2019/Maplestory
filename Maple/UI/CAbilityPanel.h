#pragma once
#include "CUIPanel.h"
class CAbilityPanel :
	public CUIPanel
{
public:
	CAbilityPanel();
	CAbilityPanel(const CAbilityPanel& panel);
	~CAbilityPanel();




public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CAbilityPanel* Clone();


private:
	void MouseOnX(class CCollider* pSrc, class CCollider* pDest, float fTime);

	void MouseMoveStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseMoveStay(class CCollider* pSrc, class CCollider* pDest, float fTime);

};

