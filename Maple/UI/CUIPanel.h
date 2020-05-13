#pragma once

#include "CUI.h"

class CUIPanel :
	public CUI
{
public:
	CUIPanel();
	CUIPanel(const CUIPanel& panel);
	virtual ~CUIPanel();

protected:
	friend class CPlayer;

public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CUIPanel* Clone();

protected:
	PANEL_TYPE m_ePanel;
	class CObj* m_pObj; // 정보를 얻어야 하는 주체

	bool		m_bBool;
	int			m_iZOrder;

private:
	void HPBarUpdate();
	void MPBarUpdate();
	void MonsterHPBorderUpdate();
	void MonsterHPBarUpdate();

	void MinimapUpdate();
	void MinimapCircle();
	
	void NameUpdate();


public:

	void SetZOrder(int i)
	{
		m_iZOrder = i;
	}

	int GetZOrder() const
	{
		return m_iZOrder;
	}

	void SetPanel(POINT tRenderPos, class CTexture* pTexture, PANEL_TYPE ePanel, class CObj* pObj);

	void SetPanelType(PANEL_TYPE eType)
	{
		m_ePanel = eType;
	}

	PANEL_TYPE GetPanelType() const
	{
		return m_ePanel;
	}

	void SetPanelObj(class CObj* pObj);
};

