
#include "CStaticObj.h"
class CItem :
	public CStaticObj
{
public:
	CItem();
	CItem(const CItem& item);
	~CItem();

private:
	ITEM_TAG m_eTag;
	int		 m_iCost;
	int		 m_iSell;

public:

	int GetCost() const
	{
		return m_iCost;
	}

	int GetSell() const
	{
		return m_iSell;
	}

	ITEM_TAG GetTag() const
	{
		return m_eTag;
	}


	void SetItem(ITEM_TAG eTag, int iCost, int iSell)
	{
		m_eTag = eTag;
		m_iCost = iCost;
		m_iSell = iSell;

	}

	void SetItemTag(ITEM_TAG eTag)
	{
		m_eTag = eTag;
	}

	void SetCost(int iCost, int iSell)
	{
		m_iCost = iCost;
		m_iSell = iSell;
	}
public:
	
	void Use();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CItem* Clone();

	void Render(HDC hDC, float x, float y);
};
