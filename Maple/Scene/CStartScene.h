#pragma once
#include "CScene.h"


class CStartScene :
	public CScene
{
private:
	CStartScene();
	~CStartScene();


private:
	friend class CSceneManager;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

private:
	void StartBtnCallback(float fTime);
	// ���� ��ư��.
	// UI -> Button bind functional	
};

