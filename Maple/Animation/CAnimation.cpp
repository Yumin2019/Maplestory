#include "CAnimation.h"
#include "../Texture/CTexture.h"
#include "../Core/CSceneManager.h"
#include "../Object/CPlayer.h"
#include "../Object/CMonster.h"

CAnimation::CAnimation() :
	m_pCurClip(NULL),
	m_pObj(NULL)
{
}

CAnimation::CAnimation(const CAnimation & anim)
{

	m_mapClip.clear();

	unordered_map<string, PANIMATIONCLIP>::const_iterator iter;
	unordered_map<string, PANIMATIONCLIP>::const_iterator iterEnd = anim.m_mapClip.end();

	for (iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PANIMATIONCLIP pClip = new ANIMATIONCLIP;

		*pClip = *iter->second; 

		for (size_t i = 0; i < iter->second->vecTexture.size(); ++i)
		{
			pClip->vecTexture[i]->AddRef();
		}

		m_mapClip.insert(make_pair(iter->first, pClip));

	}

	m_pCurClip = NULL;
	m_pObj = NULL;
	m_strCurClip = "";
	m_strDefaultClip = anim.m_strDefaultClip;
	m_strTag = anim.m_strTag;
}


CAnimation::~CAnimation()
{

	unordered_map<string, PANIMATIONCLIP>::iterator iter;
	unordered_map<string, PANIMATIONCLIP>::iterator iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		for (size_t i = 0; i < iter->second->vecTexture.size(); ++i)
		{
			SAFE_RELEASE(iter->second->vecTexture[i]);
		}
	
		SAFE_DELETE(iter->second);
	}

	m_mapClip.clear();
}

bool CAnimation::Init()
{
	return true;
}

void CAnimation::Update(float fTime)
{
	m_pCurClip->fAnimationTime += fTime;

	// 1 frame 시간 fAnimationFrameTime

	switch (m_pCurClip->eAnimationOption)
	{
	case AO_TIME_RETURN:
		
		m_pCurClip->fOptionTime += fTime;

		if (m_pCurClip->fOptionTime >= m_pCurClip->fOptionLimitTime)
		{
			m_pCurClip->fOptionTime = 0.f;
			ChangeClip(GetDefaultClip());

				// 현재 옵션 셋팅.
				switch (m_pObj->GetObjectType())
				{
				case OT_PLAYER:
					// 모션을 고의적으로 바꾸는 경우 alert가 false가 되지만,
					// 계속 실행시키는 경우 false가 자동으로 안 된다.
					// 이 처리를 플레이어 내부적으로 하기에는 꼬이니까 애니메이션에서 처리
					
					// 여기 왔다는 것은 alert를 다 돌았다는 소리

					((CPlayer*)m_pObj)->SetAlert(false);
					((CPlayer*)m_pObj)->SetPlayerMotion(PM_STAND);

					break;

				case OT_MONSTER:
					break;
				}


		}
		break;

	case AO_TIME_DESTROY:

		m_pCurClip->fOptionTime += fTime;

		if (m_pCurClip->fOptionTime >= m_pCurClip->fOptionLimitTime)
		{
			m_pObj->Die();
		}
		break;
	}


	switch (m_pCurClip->eImageOption)
	{
	case IO_ATLAS:
		AtlasUpdate();
		break;

	case IO_FRAME:
		FrameUpdate();
		break;
	}
	
		

}

CAnimation * CAnimation::Clone()
{
	return new CAnimation(*this);
}

PANIMATIONCLIP CAnimation::FindClip(const string & strTag)
{
	unordered_map<string, PANIMATIONCLIP>::iterator iter = m_mapClip.find(strTag);

	if (iter == m_mapClip.end())
		return NULL;

	return iter->second;
}

void CAnimation::SetObj(CObj * pObj)
{
	m_pObj = pObj;
}

void CAnimation::ChangeClip()
{
	// Default 클립으로 변경 다른 모션을 취하고 변하는 확실한 변경에서 사용
	m_strCurClip = m_strDefaultClip;

	if (m_pCurClip)
	{
		m_pCurClip->iFrameX = m_pCurClip->iStartX;
		m_pCurClip->iFrameY = m_pCurClip->iStartY;
		m_pCurClip->fAnimationTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;

	}

	m_pCurClip = FindClip(m_strCurClip);

	m_pObj->SetTexture(m_pCurClip->vecTexture[0]);
}

void CAnimation::ChangeClip(const string & strTag)
{

	if (strTag == m_strCurClip)
		return;

	m_strCurClip = strTag;

	if (m_pCurClip)
	{
		m_pCurClip->iFrameX = m_pCurClip->iStartX;
		m_pCurClip->iFrameY = m_pCurClip->iStartY;
		m_pCurClip->fAnimationTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;


		// 다시 이용할 것을 대비하여 미리 셋팅한다.
	}


	m_pCurClip = FindClip(strTag);

	// 찾은 Clip을 오브젝트가 셋팅한다.
	
	m_pObj->SetTexture(m_pCurClip->vecTexture[0]); 
   
}

void CAnimation::SetDefaultClip(const string & strTag)
{
	m_strDefaultClip = strTag;
}

void CAnimation::SetCurrentClip(const string & strTag)
{
	m_strCurClip = strTag;
}

bool CAnimation::AddFrameClip(const wstring & strFileName, const string & What,
	ANIMATION_OPTION eAniOption, float fOptionLimitTime, 
	float fAnimationLimitTime,
	int iLengthX, _SIZE m_tRenderSize,
	float fOffsetX, float fOffsetY,
	float fFrameSizeX, float fFrameSizeY,
	unsigned int r, unsigned int g, unsigned int b)
{
	PANIMATIONCLIP pClip = new ANIMATIONCLIP;

	pClip->eAnimationOption = eAniOption;
	pClip->eImageOption = IO_FRAME;
	pClip->fAnimationLimitTime = fAnimationLimitTime;
	pClip->iStartX = 0;
	pClip->iStartY = 0;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = 0;
	pClip->fOptionLimitTime = fOptionLimitTime;
	pClip->fAnimationFrameTime = fAnimationLimitTime / iLengthX;



	pClip->fAnimationTime = 0.f;
	pClip->iFrameX = 0;
	pClip->iFrameY = 0;
	pClip->fOptionTime = 0.f;


	pClip->fOffsetX = fOffsetX;
	pClip->fOffsetY = fOffsetY;
	pClip->fFrameSizeX = fFrameSizeX;
	pClip->fFrameSizeY = fFrameSizeY;


		// 만약에 Stand0 ~ Stand# 이런 식이면, Stand를 입력해서 문자열 덧셈을 한다.
		// y 길이와는 전혀 상관없이 x 길이로만 처리 한다.

		for (size_t i = 0; i < iLengthX; ++i)
		{
			wstring strImage = strFileName + to_wstring(i) + L".bmp";

			pClip->vecTexture.push_back(CTexture::SetTexture(strImage, m_tRenderSize, r, g, b));

		}

	m_mapClip.insert(make_pair(What, pClip));

	return true;
}



bool CAnimation::AddAtlasClip(const wstring & strFileName, const string & What,
	ANIMATION_OPTION eAniOption, float fOptionLimitTime,
	float fAnimationLimitTime, 
	int iStartX, int iStartY,
	int iLengthX, int iLengthY,
	float fOffsetX, float fOffsetY,
	float fFrameSizeX, float fFrameSizeY,
	unsigned int r, unsigned int g, unsigned int b)
{

	PANIMATIONCLIP pClip = new ANIMATIONCLIP;

	pClip->eAnimationOption = eAniOption;
	pClip->eImageOption = IO_ATLAS;
	pClip->fAnimationLimitTime = fAnimationLimitTime;
	pClip->iStartX = iStartX;
	pClip->iStartY = iStartY;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->fOptionLimitTime = fOptionLimitTime;
	pClip->fAnimationFrameTime = fAnimationLimitTime / (iLengthX * iLengthY);



	pClip->fAnimationTime = 0.f;
	pClip->iFrameX = iStartX; // startx
	pClip->iFrameY = iStartY; // starty
	pClip->fOptionTime = 0.f;


	pClip->fOffsetX = fOffsetX;
	pClip->fOffsetY = fOffsetY;
	pClip->fFrameSizeX = fFrameSizeX;
	pClip->fFrameSizeY = fFrameSizeY;


	// 아틀라스의 경우에는 clip별로 size를 다시 잡아야 하므로 그냥 0,0 을 둔다.

	// 더욱이 이번 프로젝트에서는 비중이 없음.
	pClip->vecTexture.push_back(CTexture::SetTexture(strFileName, _SIZE(0.f, 0.f), r, g, b));

	m_mapClip.insert(make_pair(What, pClip));


	return true;
}



void CAnimation::AtlasUpdate() // 안씀.
{

	while (m_pCurClip->fAnimationTime >= m_pCurClip->fAnimationFrameTime)
	{
		m_pCurClip->fAnimationTime -= m_pCurClip->fAnimationFrameTime;

		// 다음 장면을 가리켜야 한다.

		++m_pCurClip->iFrameX;

		if (m_pCurClip->iFrameX - m_pCurClip->iStartX == m_pCurClip->iLengthX)
		{
			// 0 부터 시작하여 센다.
			m_pCurClip->iFrameX = m_pCurClip->iStartX;
			++m_pCurClip->iFrameY;

			if (m_pCurClip->iFrameY - m_pCurClip->iStartY == m_pCurClip->iLengthY)
			{
				m_pCurClip->iFrameY = m_pCurClip->iStartY;

				switch (m_pCurClip->eAnimationOption)
				{
				case AO_ONCE_RETURN:

					ChangeClip(m_strDefaultClip);


					break;

				case AO_ONCE_DESTROY:


					m_pObj->Die();


				}


				return;


				// LOOP는 계속.


			}
		}
	}
}

void CAnimation::FrameUpdate()
{
	while (m_pCurClip->fAnimationTime >= m_pCurClip->fAnimationFrameTime)
	{
		m_pCurClip->fAnimationTime -= m_pCurClip->fAnimationFrameTime;


		// 프레임의 경우에는 다음 Texture를 가리키도록 한다.

		if (++m_pCurClip->iFrameX < m_pCurClip->iLengthX)
		m_pObj->SetTexture(m_pCurClip->vecTexture[m_pCurClip->iFrameX]);

		if (m_pCurClip->iFrameX == m_pCurClip->iLengthX)
		{
			m_pCurClip->iFrameX = 0;

			switch (m_pCurClip->eAnimationOption)
			{
			case AO_ONCE_RETURN:

				ChangeClip(m_strDefaultClip);

				switch (m_pObj->GetObjectType())
				{
				case OT_PLAYER:
					//((CPlayer*)m_pObj)->SetPlayerMotion(PM_STAND);
					break;

				case OT_MONSTER:
					break;
				}


				break;

			case AO_ONCE_DESTROY:


				m_pObj->Die();

				break;

				// 아틀라스는 그냥 인덱스만 바꾸면 바로 적용이 된다.
				// 하지만 프레임의 경우에는 처음과 같이 바로 셋팅을 하고 시작해야 한다.
			case AO_LOOP:


				switch (m_pObj->GetObjectType())
				{
				case OT_PLAYER:
						{
							CPlayer* pPlayer = (CPlayer*)m_pObj;

							switch (pPlayer->GetPlayerMotion())
							{
							case PM_JUMP:
								// 점프라면 바닥에 붙었는지 확인한다.
								if (!pPlayer->GetFalling()) // 바닥에 붙었는데 점프가 돌고 있는 경우는 잘못된 것이다.
								{
									// stand 모션으로 바꾼다.
									pPlayer->SetPlayerMotion(PM_STAND);
									pPlayer->ChangeMotion();
								}
								
								break;

							case PM_PRONE_ATTACK: 
								
								pPlayer->SetPlayerMotion(PM_PRONE);

								pPlayer->ChangeMotion();
								// 일단 프론 어택을 푼다.
								// 계속 누르고 있는 경우 true가 될 것이며 안 누르고 있는 경우 제대로 처리할 것이다.

								break;// check
							case PM_SWING1:
							case PM_SWING2:
							case PM_SWING3:
							case PM_STAB:
								pPlayer->ChangeNextMotion();

								break;

								

							case PM_SLASH:
								// 슬래쉬가 끝났다면, bool을 false

								pPlayer->SetSlash(false);

								break;

							default:
								m_pObj->SetTexture(m_pCurClip->vecTexture[0]);

							}
						}

					break;

				case OT_MONSTER:

					{

							CMonster* pMonster = (CMonster*)m_pObj;

							switch (pMonster->GetMonsterMotion())
							{
							default:
								m_pObj->SetTexture(m_pCurClip->vecTexture[0]);
							}




					}






					break;
				}
				break;

			}


			return;
		}
	}
}



