#include "CCollider.h"
#include "../Object/CObj.h"
#include "../Scene/CInGameScene.h"


CCollider::CCollider() :
	m_eShape(ST_NONE),
	m_pObj(NULL)
{
	POINT tPos;
	tPos.x = 0;
	tPos.y = 0;
	m_tLeftTop = tPos;
}

CCollider::CCollider(const CCollider & col)
{
	m_CollisionList.clear();

	m_pObj = NULL;
	m_eShape = col.m_eShape;
	m_tLeftTop = col.m_tLeftTop; 

	// �Լ� ������ ������ �� ������ Init���� �ϵ��� �ϸ�.. �ҷ��;� �ϳ� ? 
	// ��ġ�� �� �� �����ϱ� ������...
}


CCollider::~CCollider()
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_CollisionList.end();
	
	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		// ���� ���� �׾�����, �ʳװ� ������ ���� �ʿ䰡 ����.

		(*iter)->EraseCollisionList(this);
	}

	m_CollisionList.clear();
}



void CCollider::Update(float fDeltaTime)
{
	// �⺻������ �浹ü�� ������Ʈ�� ������ ������ ����ٴѴ�. 
	// Pixel�� Shape�� ������. 

	// Pixel�� ��쿡�� Object�� ���� �ִ� ��� -> �������� �Ѵ�.
	//							 X   -> �������� �ʾƾ� �Ѵ�.

	// Shape						 -> �����δ�.

	// ��� : ���������� �����̸� �ȴ�.

	/*if (m_pObj)
	{
		���Ÿ� �Űܾ� �ϴµ�... ������ .. ����....

			=> ���� ó��...
	}*/
}

void CCollider::Render(HDC hDC, float fDeltaTime)
{

}


