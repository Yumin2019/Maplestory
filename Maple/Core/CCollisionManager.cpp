#include "CCollisionManager.h"
#include "../Object/CObj.h"
#include "../Math.h"
#include "../Collision/CCollider.h"

DEFINITION_SINGLE(CCollisionManager);

CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
}

bool CCollisionManager::Init()
{
	return true;
}

void CCollisionManager::AddObj(CObj * pObj)
{
	// Collider�� ����ϴ� ��쿡�� ����.
	if(pObj->ColliderUse())
	m_CollisionList.push_back(pObj);
}

void CCollisionManager::Collision(float fDeltaTime)
{
	if (m_CollisionList.size() < 2)
	{
		m_CollisionList.clear();
		return;
	}

	// 2�� �̻���ʹ� �浹ó���� �Ѵ�.

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_CollisionList.end();
	
	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		list<CObj*>::iterator m_iter = iter;
		++m_iter;
		list<CObj*>::iterator m_iterEnd = m_CollisionList.end();

		for (; m_iter != m_iterEnd; ++m_iter)
		{
			// �� ������Ʈ �� Type�� ���� �񱳸� ���� ���� ���ؾ� �Ѵ�.

			if (CollisionGo(*iter, *m_iter))
			{
				// �� ��ü�� �浹ü List�� �����ͼ� �ϳ��ϳ� �����ؾ� �Ѵ�.
				
				
				Collision2(*iter, *m_iter, fDeltaTime);


			}
		

		}
	}



	m_CollisionList.clear();
}

bool CCollisionManager::CollisionGo(CObj * pSrc, CObj * pDest)
{

	/*
	OT_NONE,
	OT_MONSTER,
	OT_PLAYER,
	OT_NPC,
	OT_UI,
	OT_MAP
	OT_MOUSE
	/
	*/
	switch(pSrc->GetObjectType())
	{
	case OT_PLAYER:
		switch (pDest->GetObjectType())
		{
		case OT_PLAYER:
			return false;

		case OT_MONSTER:
			return true;

			
		case OT_NPC:
			return false;

		case OT_MAP:
			return true;
		}

		break;

	case OT_MAP:
		switch (pDest->GetObjectType())
		{
		case OT_PLAYER:
			return true;

		case OT_MOUSE:
		case OT_MONSTER:
			return true;


		/*case OT_MAP:
			return true;*/
		}

		break;
	case OT_MONSTER:
		switch (pDest->GetObjectType())
		{
		case OT_PLAYER:
			return true;

		case OT_MONSTER:
			return false;

		case OT_MAP:
			return true;


		case OT_NPC:
			return false;
		}

		break;

	case OT_NPC:
		switch (pDest->GetObjectType())
		{
		case OT_PLAYER:
		case OT_MONSTER:
		case OT_NPC:
			return false;

		}

		break;


	case OT_UI:
		switch (pDest->GetObjectType())
		{
		case OT_MOUSE:
			return true;
		}

		break;

	case OT_MOUSE:
		switch (pDest->GetObjectType())
		{
		case OT_MAP:
		case OT_UI:
			return true;
		}

		break;

	}

	return false;
}

void CCollisionManager::Collision2(CObj * pSrc, CObj * pDest, float fDeltaTime)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = pSrc->GetColliderList().end();

	for (iter = pSrc->GetColliderList().begin(); iter != iterEnd; ++iter)
	{
		list<CCollider*>::iterator m_iter;
		list<CCollider*>::iterator m_iterEnd = pDest->GetColliderList().end();

		for (m_iter = pDest->GetColliderList().begin(); m_iter != m_iterEnd; ++m_iter)
		{
			if (Math::CollisionCheck(*iter, *m_iter))
			{
				if (!(*iter)->CheckPrevCollision(*m_iter))
				{
					// ó�� �浹�� �� ��쿡�� ������ �浹 ����Ʈ�� �߰��Ѵ�.
					(*iter)->AddCollider(*m_iter);
					(*m_iter)->AddCollider(*iter);

					(*iter)->CallFunction(CS_START, *m_iter, fDeltaTime);
					(*m_iter)->CallFunction(CS_START, *iter, fDeltaTime);
				}
				else // ������ �浹�� �� ���� �ִ�. (�浹��)
				{
					(*iter)->CallFunction(CS_STAY, *m_iter, fDeltaTime);
					(*m_iter)->CallFunction(CS_STAY, *iter, fDeltaTime);
				}

			}
			else
			{
				// �浹�� ���� ���� ���

				if ((*iter)->CheckPrevCollision(*m_iter)) // ��� ������.
				{
					// ������ �浹 ����Ʈ���� ����
					(*iter)->EraseCollisionList(*m_iter);
					(*m_iter)->EraseCollisionList(*iter);

					(*iter)->CallFunction(CS_END, *m_iter, fDeltaTime);
					(*m_iter)->CallFunction(CS_END, *iter, fDeltaTime);

				}
				else // �ƹ��͵� �ƴ�
				{

				}

			}
		}
	}

	
}

