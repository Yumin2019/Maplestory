#pragma once
#include "CUIPanel.h"
class CShopUI :
	public CUIPanel
{
public:
	CShopUI();
	CShopUI(const CShopUI& panel);
	~CShopUI();


public:
	static int					   m_iSlot; // 0 ~ 15까지 움직일 수 있다.


private:
	class CInventoryPanel* m_pInven;
	class CItem* m_Item[IT_SIZE]; 



	int	m_iCount;
	int m_iRow;
	int m_iCol;
	bool m_bClick;

public:

	void SetInven(class CInventoryPanel* pInven)
	{
		m_pInven = pInven;
	}

	void SetShop()
	{
		m_iSlot = 0;
	}

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CShopUI* Clone();

private:

	// 우측 충돌체를 선택한 후에 그것의 순서에 따라서 인벤토리의 데이터의 위치를 가져오는 함수.
	POINT GetBlockIndex();
	int GetShopBlockIndex();

	void GetItem(int idx);

	// 오른

	void MouseOnX(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseMoveStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void MouseMoveStay(class CCollider* pSrc, class CCollider* pDest, float fTime);

	void ItemBlockStart(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ItemBlockStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ItemBlockEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

	void LeftCollision();
	void RightCollision();
};

