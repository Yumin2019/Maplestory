#pragma once

#include "../Game.h"

class CScene
{
protected:
	CScene();
	virtual ~CScene();


private:
	friend class CSceneManager;


protected:
	list<class CLayer*>  m_LayerList;
	class CTexture*		 m_pScreen;
	SCENE_CLIP			 m_eSceneClip;

public:

	class CTexture* GetTexture()
	{
		return m_pScreen;
	}

	// POINT 편리하게 생성

	POINT GetPoint(int x, int y)
	{
		POINT pos;
		pos.x = x;
		pos.y = y;

		return pos;
	}

	SCENE_CLIP GetSceneClip() const
	{
		return m_eSceneClip;
	}

	void SetSceneClip(SCENE_CLIP eClip)
	{
		m_eSceneClip = eClip;
	}


public:

	bool	MouseInit();
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

	class CLayer* FindLayer(const string& strTag);
	class CLayer* CreateLayer(const string& strTag, int ZOrder);

	
};

