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

	// 함수 포인터 지정은 각 몬스터의 Init에서 하도록 하면.. 불러와야 하네 ? 
	// 그치만 몇 개 없으니까 괜찮아...
}


CCollider::~CCollider()
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_CollisionList.end();
	
	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		// 나는 이제 죽었으니, 너네가 가지고 있을 필요가 없어.

		(*iter)->EraseCollisionList(this);
	}

	m_CollisionList.clear();
}



void CCollider::Update(float fDeltaTime)
{
	// 기본적으로 충돌체는 오브젝트를 일정한 비율로 따라다닌다. 
	// Pixel과 Shape로 나뉜다. 

	// Pixel의 경우에도 Object가 잡혀 있는 경우 -> 움직여야 한다.
	//							 X   -> 움직이지 않아야 한다.

	// Shape						 -> 움직인다.

	// 결론 : 잡혀있으면 움직이면 된다.

	/*if (m_pObj)
	{
		내거를 옮겨야 하는데... 내꺼가 .. 없다....

			=> 따로 처리...
	}*/
}

void CCollider::Render(HDC hDC, float fDeltaTime)
{

}


