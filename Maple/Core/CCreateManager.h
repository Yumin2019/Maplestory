#pragma once

#include "../Game.h"

class CCreateManager
{
	DECLARE_SINGLE(CCreateManager);

private:
	HINSTANCE m_hInst;
	HDC		  m_hDC;

public:
	bool Init(HINSTANCE hInst, HDC hDC);

	HDC GetMemDC() const
	{
		return m_hDC;
	}

	HINSTANCE GetHInst() const
	{
		return m_hInst;
	}

	/*
		Create 매니저에서 오브젝트 찾아오고, 추가하고 잡다한 기능을 모아서 한다.
	*/
	class CObj* FindObj(const string& strKey);

private:
	unordered_map<string, class CObj*> m_mapObj;


private:

	// INSERT 
	void InsertObj(const string& strTag, class CObj* pObj);

	POINT GetPoint(int x, int y)
	{
		POINT pos;
		pos.x = x;
		pos.y = y;

		return pos;
	}


	// 몬스터의 생성 및 맵에 저장까지 자동으로 하는 함수.
	class CMonster* CreateMonster(class CMonster* pMonster, const string& strMonsterTag, MONSTER_NAME eName, _SIZE tSize, int iLevel,
		int iHP, int iMP, int iEXP, int iGold,
		int iAttackMin, int iAttackMax, float fSpeed);


	// 텍스처'만' 지정
	class CTexture* CreateTexture(const wstring& strFileName,
		_SIZE m_tRenderSize, 
		unsigned int r, unsigned int g, unsigned int b, 
		class CObj* pObj);


	// Monster Create

	bool RedSnailCreate();
	bool BlueSnailCreate();

	bool OrangeMushroomCreate();
	bool GreenMushroomCreate();
	bool BlueMushroomCreate();
	bool MushMomCreate();

	bool GolemCreate();
	bool TreeCreate();

	// Item

	bool ItemCreate();

	// MonsterHP UI
	bool MonsterHPUICreate();

	// Player Skill Create 

	bool PlayerNormalAttackEffect();
	bool PlayerSkillEffect();
};

