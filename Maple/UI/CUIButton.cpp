#include "CUIButton.h"



CUIButton::CUIButton()
{
}

CUIButton::CUIButton(const CUI & btn) :
	CUI(btn)
{
}


CUIButton::~CUIButton()
{
}

bool CUIButton::Init()
{
	return true;
}

void CUIButton::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);
}

void CUIButton::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);

}

void CUIButton::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

	// 마우스 상태에 따라서, 처리를 해야 한다.
	// 위치적 업데이트는 소속 ui에서 움직이게 하자.


}

void CUIButton::Render(HDC hDC, float fDeltaTime)
{
	CUI::Render(hDC, fDeltaTime);

}

CUIButton * CUIButton::Clone()
{
	return new CUIButton(*this);
}
