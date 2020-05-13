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

	// 1 frame �ð� fAnimationFrameTime

	switch (m_pCurClip->eAnimationOption)
	{
	case AO_TIME_RETURN:
		
		m_pCurClip->fOptionTime += fTime;

		if (m_pCurClip->fOptionTime >= m_pCurClip->fOptionLimitTime)
		{
			m_pCurClip->fOptionTime = 0.f;
			ChangeClip(GetDefaultClip());

				// ���� �ɼ� ����.
				switch (m_pObj->GetObjectType())
				{
				case OT_PLAYER:
					// ����� ���������� �ٲٴ� ��� alert�� false�� ������,
					// ��� �����Ű�� ��� false�� �ڵ����� �� �ȴ�.
					// �� ó���� �÷��̾� ���������� �ϱ⿡�� ���̴ϱ� �ִϸ��̼ǿ��� ó��
					
					// ���� �Դٴ� ���� alert�� �� ���Ҵٴ� �Ҹ�

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
	// Default Ŭ������ ���� �ٸ� ����� ���ϰ� ���ϴ� Ȯ���� ���濡�� ���
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


		// �ٽ� �̿��� ���� ����Ͽ� �̸� �����Ѵ�.
	}


	m_pCurClip = FindClip(strTag);

	// ã�� Clip�� ������Ʈ�� �����Ѵ�.
	
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


		// ���࿡ Stand0 ~ Stand# �̷� ���̸�, Stand�� �Է��ؼ� ���ڿ� ������ �Ѵ�.
		// y ���̿ʹ� ���� ������� x ���̷θ� ó�� �Ѵ�.

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


	// ��Ʋ���� ��쿡�� clip���� size�� �ٽ� ��ƾ� �ϹǷ� �׳� 0,0 �� �д�.

	// ������ �̹� ������Ʈ������ ������ ����.
	pClip->vecTexture.push_back(CTexture::SetTexture(strFileName, _SIZE(0.f, 0.f), r, g, b));

	m_mapClip.insert(make_pair(What, pClip));


	return true;
}



void CAnimation::AtlasUpdate() // �Ⱦ�.
{

	while (m_pCurClip->fAnimationTime >= m_pCurClip->fAnimationFrameTime)
	{
		m_pCurClip->fAnimationTime -= m_pCurClip->fAnimationFrameTime;

		// ���� ����� �����Ѿ� �Ѵ�.

		++m_pCurClip->iFrameX;

		if (m_pCurClip->iFrameX - m_pCurClip->iStartX == m_pCurClip->iLengthX)
		{
			// 0 ���� �����Ͽ� ����.
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


				// LOOP�� ���.


			}
		}
	}
}

void CAnimation::FrameUpdate()
{
	while (m_pCurClip->fAnimationTime >= m_pCurClip->fAnimationFrameTime)
	{
		m_pCurClip->fAnimationTime -= m_pCurClip->fAnimationFrameTime;


		// �������� ��쿡�� ���� Texture�� ����Ű���� �Ѵ�.

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

				// ��Ʋ�󽺴� �׳� �ε����� �ٲٸ� �ٷ� ������ �ȴ�.
				// ������ �������� ��쿡�� ó���� ���� �ٷ� ������ �ϰ� �����ؾ� �Ѵ�.
			case AO_LOOP:


				switch (m_pObj->GetObjectType())
				{
				case OT_PLAYER:
						{
							CPlayer* pPlayer = (CPlayer*)m_pObj;

							switch (pPlayer->GetPlayerMotion())
							{
							case PM_JUMP:
								// ������� �ٴڿ� �پ����� Ȯ���Ѵ�.
								if (!pPlayer->GetFalling()) // �ٴڿ� �پ��µ� ������ ���� �ִ� ���� �߸��� ���̴�.
								{
									// stand ������� �ٲ۴�.
									pPlayer->SetPlayerMotion(PM_STAND);
									pPlayer->ChangeMotion();
								}
								
								break;

							case PM_PRONE_ATTACK: 
								
								pPlayer->SetPlayerMotion(PM_PRONE);

								pPlayer->ChangeMotion();
								// �ϴ� ���� ������ Ǭ��.
								// ��� ������ �ִ� ��� true�� �� ���̸� �� ������ �ִ� ��� ����� ó���� ���̴�.

								break;// check
							case PM_SWING1:
							case PM_SWING2:
							case PM_SWING3:
							case PM_STAB:
								pPlayer->ChangeNextMotion();

								break;

								

							case PM_SLASH:
								// �������� �����ٸ�, bool�� false

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



