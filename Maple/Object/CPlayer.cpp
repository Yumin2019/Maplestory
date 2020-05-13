#include "CPlayer.h"
#include "../Animation/CAnimation.h"
#include "../Core/CInput.h"
#include "../Core/CPencil.h"
#include "../Texture/CTexture.h"
#include "../Scene/CInGameScene.h"
#include "../Collision/CShape.h"
#include "../Core/CSceneManager.h"

#include "../Collision/CPixel.h"
#include "CEffect.h"
#include "../UI/CUIPanel.h"

// Item

#include "../UI/CInventoryPanel.h"
#include "../UI/CShopUI.h"


/*
��ų 1.2��(�߰��� ���� �ʴµ� �ɸ��� �ð�)�� �⺻ ��� 1.5�� + 0.2 ��� �ð� �����ؼ� 1.5�ʷ� ��Ұ�.
�⺻������ 0.75�� ����ε� ��� 0.9�ʰ� �ɷ��� 0.9�ʸ� �����ð����� ��Ҵ�. ��� �ð� x ���� ���ð��� ���߿� �� �Ÿ� ����.

�⺻�� ��� ������ ���� �� ������ �ʴ� ������ ��ǲó���� �ߺ����� �ϰ� �ִ� ���� Ǯ� �ذ��ߴ�.
*/

CPlayer::CPlayer()
{
	m_bRopeCycle = false;
	m_bRopeCollision = false;
	m_iExpMax = 0;
	m_iPoint = 0;

	m_fSpeedPotion = 0.f;
	m_fWorriorPotion = 0.f;
	m_iCriticalPercent = 5;
	m_iStancePercent = 0;

	m_bSpeedPotion = false;
	m_bWorriorPotion = false;

	m_pShop 		= NULL;
	m_pSkill		= NULL;
	m_pMinimap		= NULL;
	m_pInventory	= NULL;
	m_pAbility		= NULL;

	m_iStrength = 0;
	m_iIntellect = 0;
	m_iDexterity = 0;
	m_iLucky = 0;
}

CPlayer::CPlayer(const CPlayer & Player) :
	CMoveObj(Player)
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::DoubleJumpEffectLeft()
{
	// �ٶ�
	CEffect* pEffect = FIND(CEffect)->FindObj("Leaf1Left")->Clone();

	pEffect->SetObj(this);

	CAnimation* pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	list<CObj*>& ObjList = GET_SINGLE(CSceneManager)->GetObjList("DefaultLayer");

	ObjList.push_back(pEffect);

	// ������ : �÷��̾ ���� �ʰ� �� �ڸ��� �ֵ���.
	pEffect = FIND(CEffect)->FindObj("Leaf2Left")->Clone();

	pEffect->SetRenderPos(GetCenterX() - 20.f, GetCenterY() - 59.f);

	pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	ObjList.push_back(pEffect);

	// ������

	pEffect = FIND(CEffect)->FindObj("Leaf3Left")->Clone();

	pEffect->SetObj(this);

	pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	ObjList.push_front(pEffect);

}

void CPlayer::DoubleJumpEffectRight()
{
	// 1
	CEffect* pEffect = FIND(CEffect)->FindObj("Leaf1Right")->Clone();

	pEffect->SetObj(this);

	CAnimation* pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	list<CObj*>& ObjList = GET_SINGLE(CSceneManager)->GetObjList("DefaultLayer");

	ObjList.push_back(pEffect);

	// 2
	pEffect = FIND(CEffect)->FindObj("Leaf2Right")->Clone();

	pEffect->SetRenderPos(GetCenterX() - 164.f, GetCenterY() - 59.f);

	pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	ObjList.push_back(pEffect);

	// 3

	pEffect = FIND(CEffect)->FindObj("Leaf3Right")->Clone();

	pEffect->SetObj(this);

	pAni = pEffect->GetAnimation();

	pAni->SetObj(pEffect);
	pAni->ChangeClip();

	ObjList.push_front(pEffect);

}

void CPlayer::ChangeMotion()
{

	/*
		���� Default�� Stand�ε�, �� �ð��� Stop�� ���� swing, stab���� ��쿡��
		Stand����� �ƴ�, �ٸ� ������� �ٲٴ� ���̴�.
		�׷��� �������� ����.
		
		���� ���ϴ� ó���� �� �� �ִ�.
	*/


	string strAni;

	switch (m_ePlayerMotion) // �̸� ����
	{
	case PM_ALERT:
		strAni = "Alert";

		break;

	case PM_DIE:
		strAni = "Dead";

		break;

	case PM_HIT:
		strAni = "Hit";


		break;

	case PM_JUMP:
		strAni = "Jump";

		break;

	case PM_PRONE:
		strAni = "Prone";

		break;

	case PM_PRONE_ATTACK:
		strAni = "ProneAttack";

		m_fStopLimitTime = 0.9f;
		m_bStop = true;


		break;

	case PM_ROPE:
		strAni = "Rope";

		break;

	case PM_SIT:
		strAni = "Sit";


		break;

	case PM_STAND:
		strAni = "Stand";


		break;

	case PM_WALK:
		strAni = "Walk";


		break;

	case PM_STAB:
		strAni = "Stab";
		m_fStopLimitTime = 0.9f;

		m_bStop = true;

		break;

	case PM_SWING1:
		strAni = "Swing1";

		m_fStopLimitTime = 0.9f;
		m_bStop = true;

		break;

	case PM_SWING2:
		strAni = "Swing2";
		m_fStopLimitTime = 0.9f;
		m_bStop = true;

		break;

	case PM_SWING3:
		strAni = "Swing3";
		m_fStopLimitTime = 0.9f;
		m_bStop = true;

		break;

	case PM_SLASH:
		strAni = "Slash";
		m_fStopLimitTime = 1.5f; // 
		m_bStop = true;

		break;

	}

	
	// ���� �ִ� ���⿡ ���� ó���Ѵ�.
	switch (m_eDir)
	{
	case MD_LEFT:
		strAni += "Left";

		break;

	case MD_RIGHT:
		strAni += "Right";

		break;
	}

	m_pAnimation->ChangeClip(strAni);

	// ���ݽÿ��� ����.
	AttackEffect();
	
}

void CPlayer::ChangeNextMotion()
{
	// ������ �ϰ� �ִ� ��� -> Default
	// ������ �ϰ� �ִ� ��� -> Default�� �ƴ� ��� (���� ������� �����̱� ������ �ش� Ŭ������ ��ȯ !)

	// ��� �̰Ϳ� ���� ó���� Stop�� ���谡 �Ǵ� ģ���鸸 �����Ǳ� ������ . . . ó���� ���� �ʴ� �͵� �ִ�.


	string strAni;

	switch (m_ePlayerMotion)
	{
	case PM_WALK:

		if (m_bMove)
		{
			strAni = "Walk";
			m_ePlayerMotion = PM_WALK;
		}
		else
		{
			strAni = "Stand";
			m_ePlayerMotion = PM_STAND;
		}
		break;
	case PM_HIT: // to default


		break;

	case PM_JUMP:

		if (KEYKEY("Jump"))
		{
			strAni = "Jump";
			m_ePlayerMotion = PM_JUMP;
		}
		else
		{
			strAni = "Stand";
			m_ePlayerMotion = PM_STAND;
		}
	

		break;
		
	case PM_PRONE:

		// ���� ���¸� ����, ��� PRONE�� ���� ���� ����.


		if (KEYKEY("Prone"))
		{
			strAni = "Prone";
			m_ePlayerMotion = PM_PRONE;
		}
		else
		{
			strAni = "Stand";
			m_ePlayerMotion = PM_STAND;
		}

		break;




	

	/*case PM_PRONE_ATTACK:

		if ( (KEYKEY("Prone")) && (KEYKEY("Attack")) )
		{
			strAni = "ProneAttack";
			m_ePlayerMotion = PM_PRONE_ATTACK;

			m_bStop = true;
			m_fStopLimitTime = 0.9f;

		}
		else if (KEYKEY("Prone"))
		{
			strAni = "Prone";
			m_ePlayerMotion = PM_PRONE;
		}
		else
		{
			strAni = "Alert";
			m_bAlert = true;
			m_bProne = false;

			m_ePlayerMotion = PM_ALERT;
		}

		break;*/

	case PM_SLASH:
	case PM_SWING1:
	case PM_SWING2:
	case PM_SWING3:
	case PM_STAB: // alert�� ! 

		strAni = "Alert";
		m_bAlert = true;
		m_bProne = false;

		m_ePlayerMotion = PM_ALERT;

		// ��ų�� ��쿡�� �� ����ȭ �Ǿ� ���߿� ���� �Ѵ�.
	}


	// ���� �ִ� ���⿡ ���� ó���Ѵ�.
	switch (m_eDir)
	{
	case MD_LEFT:
		strAni += "Left";

		break;

	case MD_RIGHT:
		strAni += "Right";

		break;
	}

	if (strAni.size() == 4 || strAni.size() == 5)
		return;

	m_pAnimation->ChangeClip(strAni);

	// ���ݽÿ��� ����.
	AttackEffect();
}

void CPlayer::ChangeHitMotion()
{
	// �̹� x �� �˹��� true�� ���´�.

	if(!m_bJump) // ������ ��� ����...
	m_bKnockBackY = true;

	m_ePlayerMotion = PM_HIT;
	ChangeMotion();
}

void CPlayer::RenderPosUpdate()
{

	// �ִϸ��̼ǿ� ���� RenderPos�� ��ġ�� �޶�����.
	// �̰����� �װ��� ó���� ���̴�.
	// ��ü�����δ� tPos�� �������� ��� ������ �������� ���� ���Ѵ�.

	switch (m_ePlayerMotion)
	{
		case PM_ALERT:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 12.f;
				m_tRenderPos.y = m_tPos.y;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y;

				break;
			}

			break;

		case PM_DIE:

			break;

		case PM_HIT:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x -12.f;
				m_tRenderPos.y = m_tPos.y + 2.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y + 2.f;

				break;
			}

			break;

		case PM_JUMP:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y - 9.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x - 7.f;
				m_tRenderPos.y = m_tPos.y - 9.f;

				break;
			}

			break;

		case PM_PRONE:


			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 28.f;
				m_tRenderPos.y = m_tPos.y + 27.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y + 27.f;

				break;
			}

			break;

		case PM_PRONE_ATTACK:
	
			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 48.f; //
				m_tRenderPos.y = m_tPos.y + 27.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y + 27.f;

				break;
			}

			break;

		case PM_ROPE:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y;

				break;
			}
		
			break;

		case PM_SIT:
			switch (m_eDir)
			{
			case MD_LEFT:
				break;

			case MD_RIGHT:
				break;
			}

			break;

		case PM_STAND:


			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 9.f;
				m_tRenderPos.y = m_tPos.y;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y;

				break;
			}

		
			break;

		case PM_WALK:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 15.f;
				m_tRenderPos.y = m_tPos.y;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y;

				break;
			}

			break;

		case PM_STAB:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 60.f;
				m_tRenderPos.y = m_tPos.y + 2.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y + 2.f;

				break;
			}

			break;

		case PM_SWING1:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 13.f;
				m_tRenderPos.y = m_tPos.y -4.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x - 15.f;
				m_tRenderPos.y = m_tPos.y - 4.f;
				break;
			}
			break;

		case PM_SWING2:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 53.f;
				m_tRenderPos.y = m_tPos.y - 3.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x - 16.f;
				m_tRenderPos.y = m_tPos.y - 3.f;

				break;
			}

			break;

		case PM_SWING3:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 28.f;
				m_tRenderPos.y = m_tPos.y - 12.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x - 18.f;
				m_tRenderPos.y = m_tPos.y - 12.f;

				break;
			}

			break;

		case PM_SLASH:

			switch (m_eDir)
			{
			case MD_LEFT:
				m_tRenderPos.x = m_tPos.x - 52.f;
				m_tRenderPos.y = m_tPos.y - 13.f;

				break;

			case MD_RIGHT:
				m_tRenderPos.x = m_tPos.x;
				m_tRenderPos.y = m_tPos.y - 13.f;

				break;
			}

			break;

	}
}

void CPlayer::RenderAnimation(HDC hDC, int x, int y)
{
	wstring strAni;

	switch (m_ePlayerMotion) 
	{
	case PM_ALERT:
		strAni = L"Alert";

		break;

	case PM_DIE:
		strAni = L"Dead";

		break;

	case PM_HIT:
		strAni = L"Hit";


		break;

	case PM_JUMP:
		strAni = L"Jump";

		break;

	case PM_PRONE:
		strAni = L"Prone";

		break;

	case PM_PRONE_ATTACK:
		strAni = L"ProneAttack";



		break;

	case PM_ROPE:
		strAni = L"Rope";

		break;

	case PM_SIT:
		strAni = L"Sit";


		break;

	case PM_STAND:
		strAni = L"Stand";


		break;

	case PM_WALK:
		strAni = L"Walk";


		break;

	case PM_STAB:
		strAni = L"Stab";


		break;

	case PM_SWING1:
		strAni = L"Swing1";



		break;

	case PM_SWING2:
		strAni = L"Swing2";

		break;
	case PM_SWING3:
		strAni = L"Swing3";

		break;

	case PM_SLASH:
		strAni = L"Slash";

		break;

	}


	switch (m_eDir)
	{
	case MD_LEFT:
		strAni += L"Left";

		break;

	case MD_RIGHT:
		strAni += L"Right";

		break;
	}

	TextOut(hDC, x, y, strAni.c_str(), strAni.size());
}

void CPlayer::AttackEffect()
{
	string strEffect;

	switch (m_ePlayerMotion)
	{
	case PM_SWING1:


		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "Swing1SlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;
			break;

		case MD_RIGHT:
			strEffect = "Swing1SlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;


			break;
		}

		break;

	case PM_SWING2:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "Swing2SlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "Swing2SlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_SWING3:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "Swing3SlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "Swing3SlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_STAB:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "StabSlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "StabSlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_PRONE_ATTACK:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "ProneSlashLeft";
			m_eAttack = AT_NORMAL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "ProneSlashRight";
			m_eAttack = AT_NORMAL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_SLASH:

		switch (m_eDir)
		{
		case MD_LEFT:
			strEffect = "SlashBladeLeft";
			m_eAttack = AT_SKILL_ATTACK_LEFT;

			break;

		case MD_RIGHT:
			strEffect = "SlashBladeRight";
			m_eAttack = AT_SKILL_ATTACK_RIGHT;

			break;
		}

		break;

	case PM_JUMP:
		if (!m_bDoubleJump || m_bDoubleEffect) // ���������� off �ų�, �̹� ������ ���
			return;


		switch (m_eDir)
		{
		case MD_LEFT:
			DoubleJumpEffectLeft();
			break;

		case MD_RIGHT:
			DoubleJumpEffectRight();
			break;
		}

		m_bDoubleEffect = true;

		return;

	default:
		return;
	}


	CEffect* pEffect = FIND(CEffect)->FindObj(strEffect)->Clone();

	pEffect->SetObj(this);

	CAnimation* pAni = pEffect->GetAnimation();

	if (pAni) // �ִϸ��̼��� �ִٸ�...
	{
		pAni->SetObj(pEffect);
		pAni->ChangeClip();
	}

	GET_SINGLE(CSceneManager)->GetObjList("DefaultLayer").push_back(pEffect);
}

void CPlayer::CollisionRopeStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	switch (pDest->GetShape())
	{
	case ST_RECT_PORTAL:
		m_bPortal = true;

		break;

	case ST_PIXEL:
		m_bRopeCollision = true;
		m_tRopePos = ((CPixel*)pDest)->GetRopePos();

		break;

	case ST_RECT_MONSTER_ATTACK:
		break;

	}


}

void CPlayer::CollisionRopeStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	switch (pDest->GetShape())
	{
	case ST_RECT_PORTAL:
	case ST_PIXEL:
		break;
	case ST_RECT_MONSTER_ATTACK:

		
		m_bBlend = true;

		m_fBlendTime = 0.f;
		m_fHitTime = 0.f;
		m_bHit = true;

		m_fKnockBackYPx = 0.f;
		m_fKnockBackYTime = 0.f;

		m_fBlendLimitTime = 0.f;

		m_fHitLimitTime = 0.f;

		m_iGold = 50000;


		// �÷��̾ ���󰡴� �ӵ��� ������ �ӵ��� �ٸ�... ���󰡴� �ӵ�.. �Ÿ��� �ٸ�...
		// ���� ȿ�� �����. �˹��Ŀ� ��� �� ����.

		m_iBlend = 250;



		// ������ ���Ͱ� �ٶ󺸴� �������� ���󰣴�.
		// �ٵ� ���� ó����. ���� �������� �ؼ�, �ݴ�� �־�� �Ѵ�.
		// ������ ������ �ݴ� ����

		// ���� ���̺�
		// �浹�� ��ġ�� ��������� �˾ƾ� �Ѵ�.

		// �� �浹ü P �� M �� ���� ��
		// P.x >= M.x �� ���� �����ʿ��� �浹�̶�� �� �� �ִ�.
		// else �� ���ʿ��� �浹�� ���̴�.

		// �Ѵ� Shape Ÿ������ �������� �̾ƿ� �� �ִ�.

		float SrcX = ((CShape*)pSrc)->GetPos().x;
		float DestX = ((CShape*)pDest)->GetPos().x;

		// �ٵ� ���ÿ� �ݴ�� ���󰣴ٴ� ���� ����ؾ� �Ѵ�.
		
		if (SrcX >= DestX) // ������
		{
			m_eHitDir = MD_LEFT;
		}
		else // ����
		{
			m_eHitDir = MD_RIGHT;
		}

		break;

	}
}

void CPlayer::CollisionRopeEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	switch (pDest->GetShape())
	{
	case ST_RECT_PORTAL:
		m_bPortal = false;
		break;

	case ST_PIXEL:
		m_bRopeCollision = false;
		m_bRopeCycle = false;
		m_bFalling = true;

		break;

	case ST_RECT_MONSTER_ATTACK:
		break;
	}

}




void CPlayer::CollisionTerrainStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{

	
	// �پ��µ�.. Jump ���̸� ������ ������ �ʴ´�.


	// �¾Ƶ� ��� �ö󰡴ٰ�,  Ƚ���� ������, End

}


void CPlayer::CollisionTerrainStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{

	// �������� �ƴѵ� �پ����� ó���ϸ� �ȴ�.
	if (!m_bJump)
	{
		// Change��ǿ��� ���� ���ϴ� ��Ȳ�� �ƴѵ� ������ �Ǿ� �ִϸ��̼� ��ŵ�� �Ͼ�� ��찡 �ִ�.

		
		if(!m_bStop && !m_bProne)
			ChangeNextMotion();

		// ��Ȯ�� ó��
		m_bJumpCycle = false;

		m_bDoubleJumpCycle = false;

		m_bFalling = false;

		m_bDoubleJump = false;

		m_bDoubleEffect = false;

	}

	if (m_bRopeCycle)
	{
		m_bRopeCycle = false;
		m_eDir = MD_RIGHT;
		m_ePlayerMotion = PM_STAND;
		ChangeMotion();
	}
}

void CPlayer::CollisionTerrainEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	// �ȼ��� �浹�� �����ٴ� ���� ������ falling on�� �ǹ��Ѵ�.
	m_bFalling = true;

	// �׷��ٴ� ���� ������ fallingCount�� �̿��Ѵٴ� �ǹ�
	m_iFallingCount = 0;
	
}

void CPlayer::CollisionLeftStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{

}

void CPlayer::CollisionLeftStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bLeftEnable = false;
}

void CPlayer::CollisionLeftEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bLeftEnable = true;
}

void CPlayer::CollisionRightStart(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	
}

void CPlayer::CollisionRightStay(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bRightEnable = false;
}

void CPlayer::CollisionRightEnd(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	m_bRightEnable = true;

}

bool CPlayer::Init()
{

	m_eDir = MD_RIGHT;

	SetInfo(1, 100, 50, 0, 10000, 10, 15, 200.f); // 130 ���谡�� ������. 

	m_ePlayerMotion = PM_STAND;

	m_tPos = POSITION(600.f, 340.f);

	m_eAttack = AT_NONE;

	// ���� �÷��̾� �ִϸ��̼��� ������, ������ ��� ���� ũ��� �ƴϱ� ������, 
	// �ִϸ��̼��� ũ��� ������ �صΰ� ������ �´� �� ����.

	// ��ü�����δ� ChangeClip���� �ؽ�ó�� �̹��� ũ��� RenderSize�� �����ϸ� �ȴ�.


	// �ִϸ��̼� �� : ��ų ���

	// �� �浹 ������ �� �����ؾ� �Ѵ�.


	// �÷��̾� �������� �� �� ��.
	// ��ų, Icon

	
	SetObj(OT_PLAYER, NULL);

	m_pAnimation = CreateAnimation("Player");

	// Alert Left
	m_pAnimation->AddFrameClip(L"Player/Alert/Left/Alert",
		"AlertLeft", AO_TIME_RETURN, 2.4f, 1.2f, 3,
		_SIZE(59.f, 65.f), 0.f, 0.f,
		59.f, 65.f, 0, 128, 0);


	// Alert Right
	m_pAnimation->AddFrameClip(L"Player/Alert/Right/Alert",
		"AlertRight", AO_TIME_RETURN, 2.4f, 1.2f, 3,
		_SIZE(59.f, 65.f), 0.f, 0.f,
		59.f, 65.f, 0, 128, 0);

	// Dead
	m_pAnimation->AddFrameClip(L"Player/Dead/Dead",
		"Dead", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(47.f, 63.f), 0.f, 0.f,
		47.f, 63.f, 0, 128, 0);

	// HIT ���� ��� ���߰� �ǰ�. 


	// Hit Left
	m_pAnimation->AddFrameClip(L"Player/Hit/Left/Hit",
		"HitLeft", AO_LOOP, 2.f, 1.f, 3,
		_SIZE(59.f, 65.f), 0.f, 0.f,
		59.f, 65.f, 0, 128, 0);

	// Hit Right
	m_pAnimation->AddFrameClip(L"Player/Hit/Right/Hit",
		"HitRight", AO_LOOP, 2.f, 1.f, 3,
		_SIZE(59.f, 65.f), 0.f, 0.f,
		59.f, 65.f, 0, 128, 0);

	
	// Jump Left ������ Ŭ���� �ٲ�� �������� ���
	m_pAnimation->AddFrameClip(L"Player/Jump/Left/Jump",
		"JumpLeft", AO_LOOP, 0.f, 0.5f, 1,
		_SIZE(54.f, 73.f), 0.f, 0.f,
		54.f, 73.f, 0, 128, 0);

	// Jump Right 
	m_pAnimation->AddFrameClip(L"Player/Jump/Right/Jump",
		"JumpRight", AO_LOOP, 0.f, 0.5f, 1,
		_SIZE(54.f, 73.f), 0.f, 0.f,
		54.f, 73.f, 0, 128, 0);

	// Prone Left
	m_pAnimation->AddFrameClip(L"Player/Prone/Left/Prone",
		"ProneLeft", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(75.f, 40.f), 0.f, 0.f,
		75.f, 40.f, 0, 128, 0);

	// Prone Right
	m_pAnimation->AddFrameClip(L"Player/Prone/Right/Prone",
		"ProneRight", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(75.f, 40.f), 0.f, 0.f,
		75.f, 40.f, 0, 128, 0);

	

	// Rope �� ������ ���� ������ �غ��� �Ѵ�.
	// �Ƹ� ����, �Լ�ó���� ���ؼ� �ð��� 0���� �� ��.
	// �Լ� ó���� ���߿�
	// Ÿ�� �ִµ� �������� �ʴ� ��쿡 �Լ� ó����
	// ������ ���´�.
	m_pAnimation->AddFrameClip(L"Player/Rope/Rope",
		"Rope", AO_LOOP, 0.f, 1.f, 2,
		_SIZE(47.f, 67.f), 0.f, 0.f,
		47.f, 67.f, 0, 128, 0);

	// Sit Left
	m_pAnimation->AddFrameClip(L"Player/Sit/Left/Sit",
		"SitLeft", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(90.f, 103.f), 0.f, 0.f,
		90.f, 103.f, 0, 128, 0);

	// Sit Right
	m_pAnimation->AddFrameClip(L"Player/Sit/Right/Sit",
		"SitRight", AO_LOOP, 0.f, 5.f, 1,
		_SIZE(90.f, 103.f), 0.f, 0.f,
		90.f, 103.f, 0, 128, 0);

	// SKILL�� ������.

	

	// Attack ���� ======================

	// ProneAttack Left
	m_pAnimation->AddFrameClip(L"Player/ProneAttack/Left/ProneAttack",
		"ProneAttackLeft", AO_LOOP, 0.f, 0.75f, 2,
		_SIZE(95.f, 46.f), 0.f, 0.f,
		95.f, 46.f, 0, 128, 0);

	// ProneAttack Right
	m_pAnimation->AddFrameClip(L"Player/ProneAttack/Right/ProneAttack",
		"ProneAttackRight", AO_LOOP, 0.f, 0.75f, 2,
		_SIZE(95.f, 46.f), 0.f, 0.f,
		95.f, 46.f, 0, 128, 0);

	// Swing1 Left
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing1/Left/Swing",
		"Swing1Left", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(75.f, 102.f), 0.f, 0.f,
		75.f, 102.f, 0, 128, 0);

	// Swing1 Right
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing1/Right/Swing",
		"Swing1Right", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(75.f, 102.f), 0.f, 0.f,
		75.f, 102.f, 0, 128, 0);

	// Swing2 Left
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing2/Left/Swing",
		"Swing2Left", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(115.f, 68.f), 0.f, 0.f,
		115.f, 68.f, 0, 128, 0);

	// Swing2 Right
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing2/Right/Swing",
		"Swing2Right", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(115.f, 68.f), 0.f, 0.f,
		115.f, 68.f, 0, 128, 0);

	// Swing3 Left
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing3/Left/Swing",
		"Swing3Left", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(93.f, 78.f), 0.f, 0.f,
		93.f, 78.f, 0, 128, 0);

	// Swing3 Right
	m_pAnimation->AddFrameClip(L"Player/Swing/Swing3/Right/Swing",
		"Swing3Right", AO_LOOP, 0.f, 0.75f, 3,
		_SIZE(93.f, 78.f), 0.f, 0.f,
		93.f, 78.f, 0, 128, 0);

	// Stab Left
	m_pAnimation->AddFrameClip(L"Player/Stab/Left/Stab",
		"StabLeft", AO_LOOP, 0.f, 0.75f, 2,
		_SIZE(108.f, 63.f), 0.f, 0.f,
		108.f, 63.f, 0, 128, 0);

	// Stab Right
	m_pAnimation->AddFrameClip(L"Player/Stab/Right/Stab",
		"StabRight", AO_LOOP, 0.f, 0.75f, 2,
		_SIZE(108.f, 63.f), 0.f, 0.f,
		108.f, 63.f, 0, 128, 0);

	// Skill SlashBlast

	// Slash Left
	m_pAnimation->AddFrameClip(L"Player/Slash/Left/Slash",
		"SlashLeft", AO_LOOP, 0.f, 1.5f, 8,
		_SIZE(100.f, 78.f), 0.f, 0.f,
		100.f, 78.f, 0, 128, 0);

	// Slash Right
	m_pAnimation->AddFrameClip(L"Player/Slash/Right/Slash",
		"SlashRight", AO_LOOP, 0.f, 1.5f, 8,
		_SIZE(100.f, 78.f), 0.f, 0.f,
		100.f, 78.f, 0, 128, 0);

	// Attack ���� =================== ��
	// Stand Left
	m_pAnimation->AddFrameClip(L"Player/Stand/Left/Stand",
		"StandLeft", AO_LOOP, 0.f, 1.2f, 3,
		_SIZE(56.f, 69.f), 0.f, 0.f,
		56.f, 69.f, 0, 128, 0);

	// Stand Right
	m_pAnimation->AddFrameClip(L"Player/Stand/Right/Stand",
		"StandRight", AO_LOOP, 0.f, 1.2f, 3,
		_SIZE(56.f, 69.f), 0.f, 0.f,
		56.f, 69.f, 0, 128, 0);

	// Walk Left
	m_pAnimation->AddFrameClip(L"Player/Walk/Left/Walk",
		"WalkLeft", AO_LOOP, 0.f, 0.75f, 4,
		_SIZE(62.f, 66.f), 0.f, 0.f,
		62.f, 66.f, 0, 128, 0);

	// Walk Right
	m_pAnimation->AddFrameClip(L"Player/Walk/Right/Walk",
		"WalkRight", AO_LOOP, 0.f, 0.75f, 4,
		_SIZE(62.f, 66.f), 0.f, 0.f,
		62.f, 66.f, 0, 128, 0);

	m_pAnimation->SetObj(this);

	m_pAnimation->SetDefaultClip("StandRight");

	// ������ ���� Obj ��� ChangeClip
	m_pAnimation->ChangeClip("StandRight");

	m_tRopePos = GetPoint(0, 0);

	m_fKnockBackYPx = 0.f;
	m_fKnockBackYTime = 0.f;

	m_fKnockBackYLimitPx = 65.f;
	m_fKnockBackXDist = 2.5f;
	m_fKnockBackLimitPx = 100.f;

	m_bFalling = true;
	m_bJumpCycle = true; 

	m_bMinimap = true;

	m_bInventory = false;
	m_bAbility = false;
	m_bSkill = false;

	m_bDoubleEffect = false;

	m_bProne = false;

	m_bPortal = false;

	m_bRightEnable = true;

	m_bLeftEnable = true;

	m_bAlert = false;

	m_bSlash = false;

	m_bKnockBackY = false;

	m_bDoubleJump = false;

	m_bDoubleJumpCycle = false;

	m_bShop = false;

	m_fBlendTime = 0.f;

	m_fBlendLimitTime = 0.f;

	m_iExpMax = 100;

	m_fJumpLimitPx = 70.f;

	m_fHitLimitTime = 1.f; // Ȯ�ο�.


	m_fDoubleJumpPx = 0.f;
	m_fDoubleJumpTime = 0.f;
	m_fDoubleJumpLimitPx = 380; // 350 + 30 * Lv

	m_fDoubleJumpDist = 3.8f; // 0.01s�� �����ϴ� ����. limitpx / 100.f
	// ĳ���� �⺻ ũ��
	m_tSize.x = 47.f;
	m_tSize.y = 66.f;



	// COLLIDER 

	// ==  ���� ��ֹ�
	CShape* pShape = new CShape;

	pShape->InitColliderShape(_SIZE(3.f, 3.f), GetPoint(0, 61), ST_RECT_OBSTACLE);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CPlayer::CollisionLeftStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionLeftStay);

	pShape->AddCollisionFunc(CS_END, this, &CPlayer::CollisionLeftEnd);

	AddCollider(pShape);



	// ������ ��ֹ�

	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(3.f, 3.f), GetPoint(44, 61), ST_RECT_OBSTACLE);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CPlayer::CollisionRightStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionRightStay);

	pShape->AddCollisionFunc(CS_END, this, &CPlayer::CollisionRightEnd);

	AddCollider(pShape);


	// �ٴڿ� 
	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(24.f, 1.f), GetPoint(14, 66), ST_RECT_TERRAIN);

	pShape->SetObj(this);

	pShape->AddCollisionFunc(CS_START, this, &CPlayer::CollisionTerrainStart);

	pShape->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionTerrainStay);

	pShape->AddCollisionFunc(CS_END, this, &CPlayer::CollisionTerrainEnd);

	AddCollider(pShape);

	// �ϴ� NONE
	// ���� �⺻���� ��Ÿ� �¿� 55

	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(55.f, 96.f), GetPoint(-55, -15), ST_RECT_NORMAL_LEFT);

	pShape->SetObj(this);

	AddCollider(pShape);

	// ������ �⺻���� ��Ÿ�

	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(55.f, 96.f), GetPoint(47, -15), ST_RECT_NORMAL_RIGHT);

	pShape->SetObj(this);

	AddCollider(pShape);

	// ��ų ��Ÿ�  �÷��̾�κ��� 70px ��ŭ �������� size 150 x 210

	// ��ų �ߵ� �� ��� �Ŀ� Ȱ��ȭ. ��ų effect ������ ��Ȱ��ȭ.

	// ������
	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(130.f, 170.f), GetPoint(117, -40), ST_RECT_SKILL_RIGHT);

	pShape->SetObj(this);

	AddCollider(pShape);


	// ����
	pShape = new CShape;

	pShape->InitColliderShape(_SIZE(130.f, 170.f), GetPoint(-200, -40), ST_RECT_SKILL_LEFT);

	pShape->SetObj(this);

	AddCollider(pShape);


	
	CShape* pHead = new CShape;

	pHead->InitColliderShape(_SIZE(24.f, 24.f), GetPoint(12, 26), ST_RECT_ROPE);

	pHead->SetObj(this);

	pHead->AddCollisionFunc(CS_START, this, &CPlayer::CollisionRopeStart);

	pHead->AddCollisionFunc(CS_STAY, this, &CPlayer::CollisionRopeStay);

	pHead->AddCollisionFunc(CS_END, this, &CPlayer::CollisionRopeEnd);
	
	AddCollider(pHead);

	// ==

	m_bPhysics = true; // �ϴ� �÷��̾.



	return true;
}

void CPlayer::Input(float fDeltaTime)
{



	if (KEYDOWN("Minimap")) // M
	{
		m_bMinimap = !m_bMinimap;

		if (m_bMinimap)
		{
			// ���� ����
			GET_SINGLE(CSceneManager)->SetChange(((CUIPanel*)m_pMinimap)->GetZOrder());
			
		}
	}

	if (KEYDOWN("Skill")) // K
	{
		m_bSkill = !m_bSkill;

		if (m_bSkill)
		{
			GET_SINGLE(CSceneManager)->SetChange(((CUIPanel*)m_pSkill)->GetZOrder());
		}
	}

	if (KEYDOWN("Ability")) // J
	{
		m_bAbility = !m_bAbility;

		if (m_bAbility)
		{
			GET_SINGLE(CSceneManager)->SetChange(((CUIPanel*)m_pAbility)->GetZOrder());
		}
	}

	if (KEYDOWN("Inventory")) // I
	{
		m_bInventory = !m_bInventory;

		if (m_bInventory)
		{
			GET_SINGLE(CSceneManager)->SetChange(((CUIPanel*)m_pInventory)->GetZOrder());
		}

	}


	if (KEYDOWN("ESC"))
	{
		// ZOrder �켱 ������ ���� �ݾƹ���.
		GET_SINGLE(CSceneManager)->ESCKey();

	}


	// �˹� ���¶�� Input�� ���� �ʴ´�.
	if (m_bKnockBack)
		return;
		
	if (m_bStop)
		return;


	if (m_bShop)
		return;
	

	if (!m_bDoubleJumpCycle && m_bFalling && KEYDOWN("Jump")) // �������� ���� �� �� ���� && �����߿� ������ ���� ���.
	{
		m_bDoubleJump = true;

		m_bDoubleJumpCycle = true;

		m_fJumpPx = 35.f; 

		m_eDoubleDir = m_eDir;

		m_bJump = true;

		m_bDoubleJumpCycle = true;

		m_bDoubleEffect = false;

		m_fDoubleJumpTime = 0.f;

		m_fDoubleJumpPx = 0.f;

		m_ePlayerMotion = PM_JUMP;

		ChangeMotion();
		
	}

	/*
		���� : ����� ���� ������ �ʴ´�. �׳� ���� ������.
		���� Angle�� �̿��ϴ� ���� ������� �ʴ´�.
	*/


	// KEYKEY ���� && ���� ��� X

	
	 
	if (KEYDOWN("MoveUp") && m_bRopeCollision || KEYPRESS("MoveUp") && m_bRopeCollision)
	{
		// ��ٸ� Ÿ�� ����

		m_bAlert = false;

		m_bRopeCycle = true;

		m_bJumpCycle = false;

		m_bDoubleJump = false;

		m_bDoubleJumpCycle = false;

		JumpEnd();

		m_bMove = false;

		// Rope�� ��ġ�� �̵��Ѵ�.

		m_tPos.x = m_tRopePos.x - 23.f;

		m_ePlayerMotion = PM_ROPE;

		m_pAnimation->ChangeClip("Rope");

	}
	else if (KEYPRESS("Prone") && !m_bJumpCycle && !m_bRopeCycle)
	{
 		
		if (KEYKEY("Jump")) // �Ʒ� + ������ �Ʒ��� ������ �ʰ� ���� ������ �����ϰ� ����. �ٵ� �̰� rope�� ���� ���ƾ� ��.
		{

			m_bProne = false;

			m_bAlert = false;

			m_fJumpPx = 0.f;

			Jump();

			m_ePlayerMotion = PM_JUMP;

			m_bJumpCycle = true;

			ChangeMotion();

		}
		else if (KEYKEY("Attack")) // ��ũ���� + ����
		{
			m_ePlayerMotion = PM_PRONE_ATTACK;

			m_bProne = true;

			m_bAlert = false;

			ChangeMotion();

		
		}
		else  // �׳� ��ũ����
		{
			m_ePlayerMotion = PM_PRONE;

			m_bProne = true;

			m_bAlert = false;

			ChangeMotion();
		}
			

	}
	else if (KEYPRESS("Jump") && !m_bJumpCycle || KEYDOWN("Jump") && !m_bJumpCycle) // ������ �ϰ� �ִµ� ���� ��ư�� ������ ����Ű�� �þ�����ٸ� ���... �׷��ϱ� ���ƾ� ����.
	{
		if (m_bRopeCycle)
		{
			// ������ �ϴµ� ������ Ÿ�� �־��ٸ� ..

			if (KEYKEY("MoveRight") || KEYKEY("MoveLeft")) // ������ Ÿ�� �׳� ������ �Ұ��� �¿� �����鼭�� ����.
			{

				m_fJumpPx = 35.f;

				Jump();

				m_bRopeCycle = false;

				m_bAlert = false;

				m_ePlayerMotion = PM_JUMP;

				m_bJumpCycle = true;

				ChangeMotion();
			}
		
		}
		else if (!m_bFalling) // ������ �� Ÿ�� �ִµ� falling = false (�ٴڿ� ����)
		{
			m_fJumpPx = 0.f;

			Jump();

			m_bRopeCycle = false;

			m_bAlert = false;

			m_ePlayerMotion = PM_JUMP;

			m_bJumpCycle = true;

			ChangeMotion();
		}
	}
	else if (KEYDOWN("Attack") && !m_bFalling && !m_bRopeCycle && !m_bProne
		|| KEYPRESS("Attack") && !m_bFalling && !m_bRopeCycle && !m_bProne) // a
	{
		m_bAlert = false;

		// �⺻ ���� 3�� �߿� ��������.
		int idx = rand() % 4;

		switch (idx)
		{
		case 0:
			m_ePlayerMotion = PM_SWING1;
			break;

		case 1:
			m_ePlayerMotion = PM_SWING2;

			break;

		case 2:
			m_ePlayerMotion = PM_SWING3;
			break;


		case 3:
			m_ePlayerMotion = PM_STAB;
			break;

		
		}

		ChangeMotion();

		return;
		
		// ���⼭ �����ָ�, KEY UP���� ���ؼ� stand����� ����� �� �ִ�.
	
	}
	else if (KEYDOWN("Slash") && !m_bFalling && !m_bRopeCycle && !m_bProne || KEYPRESS("Slash") && !m_bFalling && !m_bRopeCycle && !m_bProne)
	{
		m_bAlert = false;
		m_ePlayerMotion = PM_SLASH;

		m_bSlash = true;

		ChangeMotion();

	}
	

	else if (KEYPRESS("MoveLeft") && !m_bRopeCycle && m_bLeftEnable && !m_bDoubleJump
		|| KEYDOWN("MoveLeft") && !m_bRopeCycle && m_bLeftEnable && !m_bDoubleJump)
	{

		// �������ε� �ڽ��� ����� �ݴ�� ������ �Ѵٸ� ���ƾ� �Ѵ�. ������ �Է��� ���� ������ Ȱ���ϴ� ���̴�.

				switch (m_eDir)
				{
				case MD_RIGHT:

					m_eDir = MD_LEFT;

					m_pAnimation->SetDefaultClip("StandLeft");

					break;

				case MD_LEFT:
					break;
				}


				m_ePlayerMotion = PM_WALK;

				ChangeMotion();

				MoveX(fDeltaTime);

				m_bAlert = false;

	

	}

	else if (KEYPRESS("MoveRight") && !m_bRopeCycle && m_bRightEnable && !m_bDoubleJump
		|| KEYDOWN("MoveRight") && !m_bRopeCycle && m_bRightEnable && !m_bDoubleJump)
	{

			switch (m_eDir)
			{
			case MD_RIGHT:
				break;

			case MD_LEFT:
				m_eDir = MD_RIGHT;
				m_pAnimation->SetDefaultClip("StandRight");

				break;
			}


			m_ePlayerMotion = PM_WALK;

			if (!m_bJumpCycle)
				ChangeMotion();


			MoveX(fDeltaTime);

			m_bAlert = false;

	}


	



	if (KEYPRESS("MoveUp") && m_bRopeCycle) 
	{
		m_tPos.y -= fDeltaTime * m_fSpeed;
		m_bMove = true;
		m_bAlert = false;

	}

	if (KEYPRESS("MoveDown") && m_bRopeCycle) 
	{
		m_tPos.y += fDeltaTime * m_fSpeed;
		m_bMove = true;
		m_bAlert = false;

	}




	if (KEYUP("Prone"))
	{
		if (!m_bJumpCycle && !m_bRopeCycle)
		{
			m_ePlayerMotion = PM_STAND;
			ChangeMotion();
		}

		m_bProne = false;

	}

	if (KEYUP("MoveLeft") && !m_bProne && !m_bJumpCycle && !m_bRopeCycle)
	{
		m_ePlayerMotion = PM_STAND;
		ChangeMotion();
	}

	if (KEYUP("MoveRight") && !m_bProne && !m_bJumpCycle && !m_bRopeCycle)
	{
		m_ePlayerMotion = PM_STAND;
		ChangeMotion();
	}

	

	
}

void CPlayer::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);

}

void CPlayer::Update(float fDeltaTime)
{
	if (m_bWorriorPotion)
	{
		m_fWorriorPotion += fDeltaTime;

		if (m_fWorriorPotion >= 10.f) // 180
		{
			m_bWorriorPotion = false;
			m_fWorriorPotion = 0.f;

			// Return
			m_iAttackMin -= 50; // 5
			m_iAttackMax -= 50;
		}
	}

	if (m_bSpeedPotion)
	{
		m_fSpeedPotion += fDeltaTime;

		if (m_fSpeedPotion >= 10.f)
		{
			m_bSpeedPotion = false;
			m_fSpeedPotion = 0.f;

			// Return
			m_fSpeed -= 100.f; // 20
		}
	}

	if (m_bProne && !KEYPRESS("Prone")) // �ٵ� ���⼭ ��� ��ŵ�� ���ɼ��� �ִ�.
	{

		switch(m_ePlayerMotion)
		{
		case PM_PRONE_ATTACK:
		case PM_SWING1:
		case PM_SWING2:
		case PM_SWING3:
		case PM_STAB:

			m_bProne = false;

			break;

		default:
			m_bProne = false;
			m_ePlayerMotion = PM_STAND;
			ChangeMotion();
			break;
		}
	
	}

	if (m_bBlend)
	{
		m_fBlendTime += fDeltaTime;
		m_fBlendLimitTime += fDeltaTime;

		if (m_fBlendTime >= 0.1f)
		{
			m_fBlendTime -= 0.1f;
			
			switch (m_iBlend)
			{
			case 250:
				m_iBlend -= 100;
				break;

			case 150:
				m_iBlend += 100;
				break;
			}
		}

		// 0.1 �� �������� ������� �� ������� �� �ݺ��ϴٰ� false ! �ϸ� ��.
		if (m_fBlendLimitTime >= 1.5f)
		{
			m_bBlend = false;

			// ���ĵ�� (�⺻)
			m_ePlayerMotion = PM_STAND;

			ChangeMotion();

			// �̰��� �⺻�̰�, ���߿� �Է¹����� ���� ����.
		}
	}

	if (m_bKnockBackY) // ���󰡴°� ����
	{

		// �÷��̾��� �˹��� y������ ���󰡴� ó������ �ϴ� ���̴�.
		// ��Ǳ����� �ٲ��� ���� �׳� hit���� �ϵ�, �˻� ! 0.2 ���� x������ �˹� + �˾�
		
		m_fKnockBackYTime += fDeltaTime;

		if (m_fKnockBackYTime >= 0.01f)
		{
			m_fKnockBackYTime -= 0.01f; // 0.2 s

			// ���⿡ ������� y������ �̵�
			m_tPos.y -= 5.f;
			m_fKnockBackYPx += 5.f;

			if (m_fKnockBackYPx >= m_fKnockBackYLimitPx)
			{
				m_fKnockBackYPx -= m_fKnockBackYLimitPx;

				m_tPos.y += m_fKnockBackYPx;

				m_bKnockBackY = false;
			}
			
		}
	}


	if (m_bFalling)
	{
		m_ePlayerMotion = PM_JUMP;
		ChangeMotion();

		if (m_bDoubleJump)
		{
			m_fDoubleJumpTime += fDeltaTime;

			if (m_fDoubleJumpTime >= 0.01f)
			{
 				m_fDoubleJumpTime -= 0.01f; // 0.01s �� 350+30*x �⺻ 380���� ���.
				

				switch (m_eDoubleDir)
				{
				case MD_LEFT:
					if (!m_bLeftEnable)
						break;
					
					// ó���� ����, ���� �������� ó��.
					
					m_tPos.x -= m_fDoubleJumpDist;
					m_fDoubleJumpPx += m_fDoubleJumpDist;

					break;

				case MD_RIGHT:
					if (!m_bRightEnable)
						break;

					m_tPos.x += m_fDoubleJumpDist;
					m_fDoubleJumpPx += m_fDoubleJumpDist;


					break;
				}
				
			}

			if (m_fDoubleJumpPx >= m_fDoubleJumpLimitPx)
			{
				m_fDoubleJumpPx -= m_fDoubleJumpLimitPx;

				// ���⼭ ���� ���� �̵��Ÿ����� �����Ѵ�.

				switch (m_eDoubleDir)
				{
				case MD_LEFT:
					m_tPos.x += m_fDoubleJumpPx;
					break;

				case MD_RIGHT:
					m_tPos.x -= m_fDoubleJumpPx;

					break;
				}

				m_bDoubleJump = false;

			}


		}


	}

	switch (m_ePlayerMotion)
	{
	case PM_ROPE:
		
		if (!m_bRopeCycle)  // rope�ε� cycle�� �ƴϸ� �߸��� ó��.
		{
			m_ePlayerMotion = PM_STAND;
			ChangeMotion();
			break;
		}

		if (!m_bMove)
			break;

	


		// �����̴� ��쿡�� Update
	default:
		CMoveObj::Update(fDeltaTime);
		break;
	}



	
	if (m_bStop)
	{
		m_fStopTime += fDeltaTime;

		if (m_fStopTime >= m_fStopLimitTime)
		{
			m_fStopTime = 0.f;
			m_bStop = false;

			// ���� ��ǿ� ���� Default�� �ƴ� �ٸ� ������� ���ư��� �ϴ� ģ������ ����Ѵ�.
			ChangeNextMotion();
		}
	}

	// �ϴ��� �÷��̾ ��������� ���ش�. RenderPos�� ���߿��� ������ ���缭
	// ��Ȳ�� ���� ����ְ� �ٽ� Stand�� ��Ȳ��� ó��

	
	

	RenderPosUpdate();

}

void CPlayer::Render(HDC hDC, float fDeltaTime)
{
	wchar_t strPos[100];


	if (CInGameScene::m_bCollider)
	{

		// ���ʹ� ���� �簢���� ���� �����̰�, �÷��̾�� �׳� �ڱ� ���� ���.
		// �⺻ ũ�� �簢��

		POSITION tPos;
		tPos.x = m_tPos.x - CInGameScene::l;
		tPos.y = m_tPos.y - CInGameScene::t;

		POSITION tPos2 = tPos + m_tSize;

		PEN->DrawRect(PC_RED, tPos, tPos2);


		////// ��ũ����
		////m_tSize.x = m_tPos.x + 73.f;
		////m_tSize.y = m_tPos.y + 66.f;

		////PEN->DrawRect(PC_RED, GetScreenPos(POSITION(m_tPos.x - 26.f, m_tPos.y + 27.f))
		////	, GetScreenPos(m_tSize));

		swprintf_s(strPos, L"Player (%.1f, %.1f)", m_tPos.x, m_tPos.y);
		TextOut(hDC, 200, 20, strPos, lstrlen(strPos));

		swprintf_s(strPos, L"RopeCollision : (%d)", m_bRopeCollision ? 1 : 0);
		TextOut(hDC, 200, 40, strPos, lstrlen(strPos));

		swprintf_s(strPos, L"PortalCollision : (%d)", m_bPortal ? 1 : 0);
		TextOut(hDC, 200, 60, strPos, lstrlen(strPos));

		// Prone
		swprintf_s(strPos, L"Prone : %d", m_bProne ? 1 : 0);
		TextOut(hDC, 50, 20, strPos, lstrlen(strPos));

		// Jump
		swprintf_s(strPos, L"JumpCycle : %d", m_bJumpCycle ? 1 : 0);
		TextOut(hDC, 50, 40, strPos, lstrlen(strPos));

		// Falling
		swprintf_s(strPos, L"Falling : %d", m_bFalling ? 1 : 0);
		TextOut(hDC, 50, 60, strPos, lstrlen(strPos));

		// Rope
		swprintf_s(strPos, L"RopeCycle : %d", m_bRopeCycle ? 1 : 0);
		TextOut(hDC, 50, 80, strPos, lstrlen(strPos));

		// Left
		swprintf_s(strPos, L"Left Enable : %d", m_bLeftEnable ? 1 : 0);
		TextOut(hDC, 50, 100, strPos, lstrlen(strPos));

		//Right
		swprintf_s(strPos, L"Right Enable : %d", m_bRightEnable ? 1 : 0);
		TextOut(hDC, 50, 120, strPos, lstrlen(strPos));

		//Right
		swprintf_s(strPos, L"Alert : %d", m_bAlert ? 1 : 0);
		TextOut(hDC, 50, 140, strPos, lstrlen(strPos));

		// Slash
		swprintf_s(strPos, L"Slash : %d", m_bSlash ? 1 : 0);
		TextOut(hDC, 50, 160, strPos, lstrlen(strPos));

		// Double Jump
		swprintf_s(strPos, L"Double JumpCycle : %d", m_bDoubleJumpCycle ? 1 : 0);
		TextOut(hDC, 50, 180, strPos, lstrlen(strPos));

		// Hit
		swprintf_s(strPos, L"Hit : %d", m_bKnockBack ? 1 : 0);
		TextOut(hDC, 50, 200, strPos, lstrlen(strPos));

	}

	CMoveObj::Render(hDC, fDeltaTime);



	// �ϴ�. enum�� ��� �ű⼭ �浹 ������ �ɷ� �س���.
	// �ѹ� ó�� & �浹���϶� collider�� list�� ������ �ٽ� ó���� �� �ֵ��� �س���...
	// Collider ������ �̿� ��Ȱ��ȭ�� �ص־� �Ѵ�. �÷��̾��� enum�� none����. �׷��� �� �̻� ó�� ����
	// ó�� �ѹ� ó��, ���Ŀ� ��Ȱ��ȭ ó�� ���� �״��. && �ٽ� �����ϸ� �ݺ�.

	swprintf_s(strPos, L"Blend : %d", m_bBlend ? 1 : 0);
	TextOut(hDC, 50, 220, strPos, lstrlen(strPos));


	m_eAttack = AT_NONE;

	
}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}
