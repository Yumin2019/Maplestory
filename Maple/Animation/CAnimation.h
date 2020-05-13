#pragma once

#include "../Game.h"

class CAnimation
{
private:
	CAnimation();
	CAnimation(const CAnimation& anim);
	~CAnimation();

private:
	friend class CObj; // 객체가 죽으면 같이 사라짐.


public:
	bool Init();
	void Update(float fTime);

	CAnimation* Clone();

private:
	unordered_map<string, PANIMATIONCLIP> m_mapClip;

	PANIMATIONCLIP m_pCurClip; // 지금 돌리고 있는 것.

	string m_strCurClip; // 현재 돌리고 있는 클립의 이름

	string m_strDefaultClip; // 돌리다가 끝나면 다시 돌아갈 클립의 이름

	class CObj* m_pObj; // 소속 Obj;

	string m_strTag; // Who

private:
	PANIMATIONCLIP FindClip(const string& strTag);


public:
	void SetObj(CObj* pObj);

	void ChangeClip();
	void ChangeClip(const string& strTag);
	void SetDefaultClip(const string& strTag);
	void SetCurrentClip(const string& strTag);
	


	// Stand0 ~ Stand# 이면 strFileName에는 Stand를 입력
	bool AddFrameClip(const wstring& strFileName, const string& What,
		ANIMATION_OPTION eAniOption, float fOptionLimitTime, 
		float fAnimationLimitTime, int iLengthX, _SIZE m_tRenderSize,
		float fOffsetX, float fOffsetY,
		float fFrameSizeX, float fFrameSizeY, 
		unsigned int r, unsigned int g, unsigned int b);

	bool AddAtlasClip(const wstring& strFileName, const string& What, 
		ANIMATION_OPTION eAniOption, float fOptionLimitTime, 
		float fAnimationLimitTime, 
		int iStartX, int iStartY,
		int iLengthX, int iLengthY,
		float fOffsetX, float fOffsetY,
		float fFrameSizeX, float fFrameSizeY,
		unsigned int r, unsigned int g, unsigned int b
		);


public:
	// Tag

	void SetTag(const string& strTag)
	{
		m_strTag = strTag;
	}

	PANIMATIONCLIP GetCurrentClip() const
	{
		return m_pCurClip;
	}


	string GetDefaultClip() const
	{
		return m_strDefaultClip;
	}


private:
	void AtlasUpdate();
	void FrameUpdate();
};

