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

	// ���콺 ���¿� ����, ó���� �ؾ� �Ѵ�.
	// ��ġ�� ������Ʈ�� �Ҽ� ui���� �����̰� ����.


}

void CUIButton::Render(HDC hDC, float fDeltaTime)
{
	CUI::Render(hDC, fDeltaTime);

}

CUIButton * CUIButton::Clone()
{
	return new CUIButton(*this);
}
