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
		Create �Ŵ������� ������Ʈ ã�ƿ���, �߰��ϰ� ����� ����� ��Ƽ� �Ѵ�.
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


	// ������ ���� �� �ʿ� ������� �ڵ����� �ϴ� �Լ�.
	class CMonster* CreateMonster(class CMonster* pMonster, const string& strMonsterTag, MONSTER_NAME eName, _SIZE tSize, int iLevel,
		int iHP, int iMP, int iEXP, int iGold,
		int iAttackMin, int iAttackMax, float fSpeed);


	// �ؽ�ó'��' ����
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

