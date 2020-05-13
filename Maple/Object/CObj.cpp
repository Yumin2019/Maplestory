#include "CObj.h"
#include "../Texture/CTexture.h"
#include "../Animation/CAnimation.h"
#include "../Scene/CInGameScene.h"
#include "../Collision/CShape.h"
#include "../Collision/CPixel.h"
#include "../Core/CPencil.h"

CObj::CObj() :
	m_bLife(true),
	m_tSize(0.f, 0.f),
	m_pTexture(NULL),
	m_pAnimation(NULL),
	m_bBlend(false),
	m_eObject(OT_NONE)
{
	m_tRenderPos = GetPoint(0, 0);
	m_iBlend = 0;
}


CObj::CObj(const CObj & Obj)
{
	*this = Obj;

	if (Obj.m_pAnimation)
	{
		m_pAnimation = Obj.m_pAnimation->Clone();

	}
	else if (Obj.m_pTexture)
	{
		Obj.m_pTexture->AddRef();
	}

	m_ColliderList.clear();

	list<CCollider*>::const_iterator iter;
	list<CCollider*>::const_iterator iterEnd = Obj.m_ColliderList.end();

	for (iter = Obj.m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		CCollider* pColl = (*iter)->Clone();

		m_ColliderList.push_back(pColl);

		pColl->SetObj(this);
	}

}


CObj::~CObj()
{
	if (m_pAnimation)
	{
		SAFE_DELETE(m_pAnimation);
	}
	else if (m_pTexture)
		SAFE_RELEASE(m_pTexture);

	Safe_Release_VecList(m_ColliderList);
}




bool CObj::Init()
{
	return true;
}

void CObj::Input(float fDeltaTime)
{
}

void CObj::Collision(float fDeltaTime)
{
}

void CObj::Update(float fDeltaTime)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}


	if (m_pAnimation)
	{
		m_pAnimation->Update(fDeltaTime);
	}

	
}

void CObj::Render(HDC hDC, float fDeltaTime)
{

	

	/*

	�浹ü �����ؼ� On Off Render ���� ���� ó���Ѵ�.
	���⼭�� Animation / Texture ������.

	*/

	// ���� ��ǥ -> ��ũ�� ��ǥ
	POSITION tRenderPos;
	tRenderPos.x = -CInGameScene::l + m_tRenderPos.x;
	tRenderPos.y = -CInGameScene::t + m_tRenderPos.y;


	POSITION tImagePos;

	if (m_pAnimation)
	{
		PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();

		switch (pClip->eImageOption)
		{
		case IO_ATLAS:

			// ������X ������ * �ε��� + x����.

			tImagePos.x = pClip->iFrameX * pClip->fFrameSizeX + pClip->fOffsetX;

			// y�� ����
			tImagePos.y = pClip->iFrameY * pClip->fFrameSizeY + pClip->fOffsetY;

			// �̷��� ���� ���� left top.

			break;

		case IO_FRAME:

			tImagePos.x = pClip->fOffsetX;

			// y�� ����
			tImagePos.y = pClip->fOffsetY;
			break;
		}

	
		
	}
	else if (m_pTexture)
	{
		tImagePos = m_pTexture->GetOffset(); // Texture ���� offset

		// �ִϸ��̼��� ������ �ִϸ��̼ǿ��� �������ش�� ����.
		// �ִϸ��̼��� ������ �ؽ�ó�� Offset���� ����
	}

	// ���� ���� �߰�.


	if (m_pTexture) // �ؽ�ó�� �ִ� ��츸.
		{
			if (!m_bBlend) // ���� OFF
			{
				TransparentBlt(hDC, tRenderPos.x, tRenderPos.y, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY()
					, m_pTexture->GetDC(),
					tImagePos.x, tImagePos.y, m_pTexture->GetRenderSizeX(),
					m_pTexture->GetRenderSizeY(), m_pTexture->GetColor());
			}
			else // ���� ON
			{
				// ������ �� ���� �߰� �޸� DC�� �ϳ� �� �ʿ��ϴ�.

				HDC m_Alpha = CreateCompatibleDC(hDC);
				HBITMAP m_AlphaBit = CreateCompatibleBitmap(hDC, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY());
				HBITMAP m_OldAlphaBit = (HBITMAP)SelectObject(m_Alpha, m_AlphaBit);


				BitBlt(m_Alpha, 0, 0, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(),
					hDC, tRenderPos.x, tRenderPos.y, SRCCOPY);

				TransparentBlt(m_Alpha, 0, 0, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(),
					m_pTexture->GetDC(), tImagePos.x, tImagePos.y,
					m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(), m_pTexture->GetColor());

				BLENDFUNCTION bf;
				bf.BlendOp = 0;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = m_iBlend;
				bf.AlphaFormat = 0;

				// ��� ������ �A ����, �����ϰ� �Ͽ� backDC�� �ѱ�
				AlphaBlend(hDC, tRenderPos.x, tRenderPos.y, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(),
					m_Alpha, 0, 0, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(), bf);

				// ������ ���� ���� ����
				SelectObject(m_Alpha, m_OldAlphaBit);
				DeleteObject(m_AlphaBit);
				DeleteDC(m_Alpha);
			}
	}

			// Collider Render Key Input

	if (CInGameScene::m_bCollider)
	{
		


				list<CCollider*>::iterator iter;
				list<CCollider*>::iterator iterEnd = m_ColliderList.end();

				for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
				{
					(*iter)->Render(hDC, fDeltaTime);
					(*iter)->RenderText(hDC); // �ȼ��� x shape�� ��ü�� ����������, ��� ��ġ�� ��ġ�µ� �̰� ���߿� �����Ѵ�.

				}
	}
	
}


void CObj::SetObj(OBJECT_TYPE eObject, CTexture * pTexture)
{
	m_pTexture = pTexture;
	m_eObject = eObject;
}

POSITION CObj::GetScreenPos(POSITION tPos)
{
	return POSITION(tPos.x - CInGameScene::l, tPos.y - CInGameScene::t);
}

POINT CObj::GetScreenPos(POINT tPos)
{
	return GetPoint(tPos.x - CInGameScene::l, tPos.y - CInGameScene::t);
}

POSITION CObj::GetCenterScreenPos(POSITION tCenter)
{
	return POSITION(GetCenterX() - CInGameScene::l, GetCenterY() - CInGameScene::t);
}

_SIZE CObj::GetRenderSize() const
{
	return m_pTexture->GetRenderSize();
}

CAnimation * CObj::CreateAnimation(const string & strTag)
{
	SAFE_DELETE(m_pAnimation);

	m_pAnimation = new CAnimation;

	m_pAnimation->SetTag(strTag);

	return m_pAnimation;
}

void CObj::SetAnimation()
{

	if (!m_pAnimation)
		return;

	m_pAnimation->SetObj(this);
	m_pAnimation->ChangeClip(m_pAnimation->GetDefaultClip());
}

void CObj::SetAnimation(const string & strTag)
{

	if (!m_pAnimation)
		return;

	m_pAnimation->SetObj(this);
	m_pAnimation->ChangeClip(strTag);
}