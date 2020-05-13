#pragma once

#include "../Game.h"

class CAnimation
{
private:
	CAnimation();
	CAnimation(const CAnimation& anim);
	~CAnimation();

private:
	friend class CObj; // ��ü�� ������ ���� �����.


public:
	bool Init();
	void Update(float fTime);

	CAnimation* Clone();

private:
	unordered_map<string, PANIMATIONCLIP> m_mapClip;

	PANIMATIONCLIP m_pCurClip; // ���� ������ �ִ� ��.

	string m_strCurClip; // ���� ������ �ִ� Ŭ���� �̸�

	string m_strDefaultClip; // �����ٰ� ������ �ٽ� ���ư� Ŭ���� �̸�

	class CObj* m_pObj; // �Ҽ� Obj;

	string m_strTag; // Who

private:
	PANIMATIONCLIP FindClip(const string& strTag);


public:
	void SetObj(CObj* pObj);

	void ChangeClip();
	void ChangeClip(const string& strTag);
	void SetDefaultClip(const string& strTag);
	void SetCurrentClip(const string& strTag);
	


	// Stand0 ~ Stand# �̸� strFileName���� Stand�� �Է�
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

