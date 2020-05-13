#include "CInput.h"

DEFINITION_SINGLE(CInput);

CInput::CInput() :
	m_pKey(NULL)
{
}


CInput::~CInput()
{
	Safe_Delete_Map(m_mapKey);
}

void CInput::Update(float fDeltaTime)
{
	unordered_map<string, PKEYINFO>::iterator iter;
	unordered_map<string, PKEYINFO>::iterator iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int iCnt = 0;

		int iSize = iter->second->vecKey.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
				++iCnt;
		}

		if (iSize == iCnt)
		{
			if (!iter->second->m_bDown && !iter->second->m_bPress) // 처을 눌렀다.
			{
				iter->second->m_bDown = true;
				

			}

			else if (iter->second->m_bDown && !iter->second->m_bPress) // 눌린 상태
			{
				iter->second->m_bDown = false;
				iter->second->m_bPress = true;
			}

		}

		else
		{
			if (iter->second->m_bDown || iter->second->m_bPress) // 방금 떼었다.
			{
				iter->second->m_bDown = false;
				iter->second->m_bPress = false;
				iter->second->m_bUp = true;
			}

			else if (iter->second->m_bUp)
				iter->second->m_bUp = false;

		}
	}


}

bool CInput::Init()
{
	// 커맨드 추가
	
	// ctrl 기본공격 alt 점프..
	// 움직이기 vk l r t b

	AddKey(VK_RIGHT, "MoveRight");
	AddKey(VK_LEFT, "MoveLeft");
	AddKey(VK_DOWN, "MoveDown");
	AddKey(VK_UP, "MoveUp");

	AddKey('M', "Minimap");
	AddKey('I', "Inventory");
	AddKey('K', "Skill");
	AddKey('J', "Ability");

	// 키셋팅 나중에.

	AddKey('Q', "Slash");
	AddKey('X', "Sit");
	AddKey(VK_DOWN, "Prone");


	AddKey(VK_ESCAPE, "ESC");
	AddKey('A', "Attack");
	AddKey(VK_SPACE, "Jump"); 

	// alt + down = 내려가기 (커맨드가 많은 편) 히히
	
	AddKey(VK_F1, "Collider");

	AddKey(VK_LBUTTON, "MouseLButton");
	AddKey(VK_RBUTTON, "MouseRButton");



	AddKey('N', "SlotUp");
	AddKey('M', "SlotDown");

	return true;
}

PKEYINFO CInput::FindKey(const string & strKey)
{
	unordered_map<string, PKEYINFO>::iterator iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}

bool CInput::KeyDown(const string & strKey)
{
	PKEYINFO pKey = FindKey(strKey);

	return pKey->m_bDown;
}

bool CInput::KeyUp(const string & strKey)
{
	PKEYINFO pKey = FindKey(strKey);

	return pKey->m_bUp;
}

bool CInput::KeyPress(const string & strKey)
{
	PKEYINFO pKey = FindKey(strKey);

	return pKey->m_bPress;
}
