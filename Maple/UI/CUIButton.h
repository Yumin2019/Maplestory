#pragma once
#include "CUI.h"


enum MOUSE_STATE
{
	MS_NONE,
	MS_ON,
	MS_CLICK
};


class CUIButton :
	public CUI
{
public:
	CUIButton();
	CUIButton(const CUI& btn);
	~CUIButton();

private:
	function<void(float)> m_BtnCallback;

public:
	
	template<typename T> // 전역함수
	void SetCallback(T* pObj, void (T::*pFunc)(float))
	{
		m_BtnCallback = bind(pFunc, pObj, placeholders::_1);
	}

	void SetCallback(void(*pFunc)(float)) // 멤버함수
	{
		m_BtnCallback = bind(pFunc, placeholders::_1);

	}
public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CUIButton* Clone();
};

