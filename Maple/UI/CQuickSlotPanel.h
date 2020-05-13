#pragma once
#include "CUIPanel.h"
class CQuickSlotPanel :
	public CUIPanel
{
public:
	CQuickSlotPanel();
	CQuickSlotPanel(const CQuickSlotPanel& panel);
	~CQuickSlotPanel();


public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CQuickSlotPanel* Clone();
};

