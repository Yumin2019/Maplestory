#pragma once
#include "CUIPanel.h"
class CSkillPanel :
	public CUIPanel
{
public:
	CSkillPanel();
	CSkillPanel(const CSkillPanel& panel);
	~CSkillPanel();


public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CSkillPanel* Clone();

private:
	void MouseOnX(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseMoveStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseMoveStay(class CCollider* pSrc, class CCollider* pDest, float fTime);

};

