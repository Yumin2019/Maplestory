#include "CUI.h"
#include "../Animation/CAnimation.h"
#include "../Texture/CTexture.h"
#include "../Collision/CCollider.h"
#include "../Scene/CInGameScene.h"

CUI::CUI()
{
	m_eObject = OT_UI;
}

CUI::CUI(const CUI & ui) :
	CStaticObj(ui)
{
	m_eObject = OT_UI;

}


CUI::~CUI()
{
}

bool CUI::Init()
{
	return true;
}

void CUI::Input(float fDeltaTime)
{
}

void CUI::Collision(float fDeltaTime)
{
}

void CUI::Update(float fDeltaTime)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}

}

void CUI::Render(HDC hDC, float fDeltaTime)
{

	POSITION tImagePos;

	if (m_pAnimation)
	{
		PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();

		switch (pClip->eImageOption)
		{
		case IO_ATLAS:

			tImagePos.x = pClip->iFrameX * pClip->fFrameSizeX + pClip->fOffsetX;

			tImagePos.y = pClip->iFrameY * pClip->fFrameSizeY + pClip->fOffsetY;

			break;

		case IO_FRAME:

			tImagePos.x = pClip->fOffsetX;

			tImagePos.y = pClip->fOffsetY;
			break;
		}

	}
	else if (m_pTexture)
	{
		tImagePos = m_pTexture->GetOffset();

	}

	// UI Offset 추가 사항에 대해서 생각해보자.

	if (m_pTexture)
	{

		if (!m_bBlend) // 블렌딩 OFF
		{
			TransparentBlt(hDC, m_tRenderPos.x, m_tRenderPos.y, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY()
				, m_pTexture->GetDC(),
				tImagePos.x, tImagePos.y, m_pTexture->GetRenderSizeX(),
				m_pTexture->GetRenderSizeY(), m_pTexture->GetColor());
		}
		else // 블렌딩 ON
		{
			// 블렌딩을 할 때는 추가 메모리 DC가 하나 더 필요하다.

			HDC m_Alpha = CreateCompatibleDC(hDC);
			HBITMAP m_AlphaBit = CreateCompatibleBitmap(hDC, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY());
			HBITMAP m_OldAlphaBit = (HBITMAP)SelectObject(m_Alpha, m_AlphaBit);


			BitBlt(m_Alpha, 0, 0, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(),
				hDC, m_tRenderPos.x, m_tRenderPos.y, SRCCOPY);

			TransparentBlt(m_Alpha, 0, 0, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(),
				m_pTexture->GetDC(), tImagePos.x, tImagePos.y,
				m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(), m_pTexture->GetColor());

			BLENDFUNCTION bf;
			bf.BlendOp = 0;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = 130;
			bf.AlphaFormat = 0;

			// 배경 색깔을 뺸 것을, 투명하게 하여 backDC에 넘김
			AlphaBlend(hDC, m_tRenderPos.x, m_tRenderPos.y, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(),
				m_Alpha, 0, 0, m_pTexture->GetRenderSizeX(), m_pTexture->GetRenderSizeY(), bf);

			// 이전에 쓰던 것을 장착
			SelectObject(m_Alpha, m_OldAlphaBit);
			DeleteObject(m_AlphaBit);
			DeleteDC(m_Alpha);
		}
	}
	
	if (CInGameScene::m_bCollider)
	{


		list<CCollider*>::iterator iter;
		list<CCollider*>::iterator iterEnd = m_ColliderList.end();

		for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
		{
			(*iter)->Render(hDC, fDeltaTime);
		}
	}

}
