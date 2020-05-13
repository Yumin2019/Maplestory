#pragma once
#include "CUIPanel.h"
class CInventoryPanel :
	public CUIPanel
{
public:
	CInventoryPanel();
	CInventoryPanel(const CInventoryPanel& panel);
	~CInventoryPanel();


public:
	static SLOT	m_tSlot[6][4];
	static bool		m_bItemMove;


private:
	class CItem* m_Item[IT_SIZE];


	int m_iRow;
	int m_iCol;
	bool		m_bClick;

public:

	POINT GetBlockIndex();

public:

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CInventoryPanel* Clone();

private:

	void MouseOnX(class CCollider* pSrc, class CCollider* pDest, float fTime);

	void MouseMoveStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseMoveStay(class CCollider* pSrc, class CCollider* pDest, float fTime);

	void ItemBlockStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ItemBlockStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ItemBlockEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

};

