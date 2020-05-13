#pragma once
#include "CScene.h"


class CInGameScene :
	public CScene
{
private:
	CInGameScene();
	~CInGameScene();

private:
	friend class CSceneManager;

private:
	class CPlayer* m_pPlayer;

public:
	static float l;
	static float t;
	static bool	 m_bCollider;

public:
	class CPlayer* GetPlayer()
	{
		return m_pPlayer;
	}


private:
	POSITION m_tMouseWorldPosSrc;
	POSITION m_tMouseWorldPosDest;
	class CTexture* m_pPixelScreen;

private:
	bool MainInit(class CStage* pMap, list<class CObj*>& ObjList);
	bool Mushroom1Init(class CStage* pMap, list<class CObj*>& ObjList);
	bool Mushroom2Init(class CStage* pMap, list<class CObj*>& ObjList);
	bool GolemInit(class CStage* pMap, list<class CObj*>& ObjList);
	bool TrainingRoomInit(class CStage* pMap, list<class CObj*>& ObjList);


public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

private:
	bool UIInit(MAP_TYPE eMap);
};

