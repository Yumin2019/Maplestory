
#include "Math.h"
#include "Collision/CShape.h"
#include "Collision\CPixel.h"
#include "Object\CPlayer.h"
#include "Core/CSceneManager.h"
#include "UI/CMouse.h"

// 순서에 상관없이 넣어도 되도록 만든 함수.

bool Math::CollisionCheck(CCollider * pSrc, CCollider * pDest)
{
	// OT_EFFECT 는 아무 처리 안 함. (FALSE)

	switch (pSrc->GetShape())
	{
		// 몬스터 공격범위 vs 플레이어 충돌 중심 rect
	case ST_RECT_MONSTER_ATTACK: // 플레이어 중심 렉트의 경우에만 싸움.

		if (pDest->GetShape() != ST_RECT_ROPE)
			return false;

		if (pDest->GetObj()->GetBlend())
			return false;

		return RectToRect((CShape*)pSrc, (CShape*)pDest);

		////////////////////////////////////////////////////////

		// 아이템 블럭 vs 마우스


	case ST_ITEM_BLOCK:
	{
		// 아이템 블럭이 계속 처리가 된다... 위에 UI가 존재하는데...
		switch (pDest->GetShape())
		{
		case ST_MOUSE:
			return RectToRect((CShape*)pSrc, (CShape*)pDest);

			break;
		}
	}

	break;


	

		// 마우스와 버튼의 충돌

	case ST_MOUSE:

	{
		CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();


			switch (pDest->GetShape())
			{
			case ST_ITEM_BLOCK:
				return RectToRect((CShape*)pSrc, (CShape*)pDest);

			case ST_MINIMAP:

				if (!pPlayer->GetMinimap())
					return false;

				return RectToRect((CShape*)pSrc, (CShape*)pDest);

			case ST_SKILL_UI_BUTTON:

				if (!pPlayer->GetSkill())
					return false;

				return RectToRect((CShape*)pSrc, (CShape*)pDest);

			case ST_INVENTORY_UI_BUTTON:

				if (!pPlayer->GetInventory())
					return false;

				return RectToRect((CShape*)pSrc, (CShape*)pDest);


			case ST_ABILITY_UI_BUTTON:

				if (!pPlayer->GetAbility())
					return false;

				return RectToRect((CShape*)pSrc, (CShape*)pDest);

			case ST_RECT_NPC:
			{
				if (pPlayer->GetShop())
					return false;


				CShape* src = (CShape*)pSrc;
				CShape* dest = (CShape*)pDest;

				return RectToRect(src->GetWorldPos(src->GetPos()), src->GetSize(), dest->GetPos(), dest->GetSize());
			}

			case ST_SHOP_UI_BUTTON:
			case ST_SHOP_SCROLL:
				if (!pPlayer->GetShop())
					return false;

				return RectToRect((CShape*)pSrc, (CShape*)pDest);

			}
	}
		break;

		case ST_RECT_NPC:
		{
			CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

			if (pPlayer->GetShop())
				return false;

			switch (pDest->GetShape())
			{

				case ST_MOUSE:
				{
					// NPC는 월드 좌표에 존재한다.
					// 따라서 스크린 좌표에서만 노는 마우스를 월드좌표로써 바꾸어 계산한다.

					CShape* src = (CShape*)pSrc;
					CShape* dest = (CShape*)pDest;

					return RectToRect(src->GetPos(), src->GetSize(), dest->GetWorldPos(dest->GetPos()), dest->GetSize());
				}
			}
		}
		break;


	case ST_INVENTORY_UI_BUTTON:
	{
		CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

		if (!pPlayer->GetInventory())
			return false;

		switch (pDest->GetShape())
		{
		case ST_MOUSE:
			return RectToRect((CShape*)pSrc, (CShape*)pDest);

			break;
		}
	}
		break;

	case ST_ABILITY_UI_BUTTON:
	{
		CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

		if (!pPlayer->GetAbility())
			return false;

		switch (pDest->GetShape())
		{
		case ST_MOUSE:
			return RectToRect((CShape*)pSrc, (CShape*)pDest);

			break;
		}
	}
	break;

	case ST_SKILL_UI_BUTTON:
	{
		CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

		if (!pPlayer->GetSkill())
			return false;

		switch (pDest->GetShape())
		{
		case ST_MOUSE:
			return RectToRect((CShape*)pSrc, (CShape*)pDest);

			break;
		}
	}
	break;

	case ST_SHOP_UI_BUTTON:
	case ST_SHOP_SCROLL:
	{
		CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

		if (!pPlayer->GetShop())
			return false;

		switch (pDest->GetShape())
		{
		case ST_MOUSE:
			return RectToRect((CShape*)pSrc, (CShape*)pDest);

			break;
		}
	}
	break;

	case ST_MINIMAP:
	{
		CPlayer* pPlayer = GET_SINGLE(CSceneManager)->GetPlayer();

		if (!pPlayer->GetMinimap())
			return false;

		switch (pDest->GetShape())
		{
		case ST_MOUSE:
			return RectToRect((CShape*)pSrc, (CShape*)pDest);

			break;
		}
	}
	break;



		// 1 : 1 체크 이전에 충돌 가능성 여부를 따져야 한다. 플레이어 충돌체 vs 픽셀 -> 이런 경우 제외.

		// 왼기 오기 왼스 오스  중에서
		// 1 : 1 대응인 경우만 처리하면 된다.

		// 플레이어 충돌체 기준으로 .

	case ST_RECT_SKILL_LEFT:

	{
		if (pDest->GetShape() != ST_RECT_MONSTER)
			return false;

		CPlayer* pPlayer = (CPlayer*)pSrc->GetObj();

		if (pPlayer->GetAttack() != AT_SKILL_ATTACK_LEFT)
			return false;

		// Check
		return RectToRect((CShape*)pSrc, (CShape*)pDest);
	}

		break;

	case ST_RECT_SKILL_RIGHT:

	{
		if (pDest->GetShape() != ST_RECT_MONSTER)
			return false;

		CPlayer* pPlayer = (CPlayer*)pSrc->GetObj();

		if (pPlayer->GetAttack() != AT_SKILL_ATTACK_RIGHT)
			return false;

		// Check
		return RectToRect((CShape*)pSrc, (CShape*)pDest);

	}
		break;

	case ST_RECT_NORMAL_LEFT:

	{

		if (pDest->GetShape() != ST_RECT_MONSTER)
			return false;


		CPlayer* pPlayer = (CPlayer*)pSrc->GetObj();

		if (pPlayer->GetAttack() != AT_NORMAL_ATTACK_LEFT)
			return false;

		// Check
		return RectToRect((CShape*)pSrc, (CShape*)pDest);

	}
		break;

	case ST_RECT_NORMAL_RIGHT:

	{
		if (pDest->GetShape() != ST_RECT_MONSTER)
			return false;

		CPlayer* pPlayer = (CPlayer*)pSrc->GetObj();

		if (pPlayer->GetAttack() != AT_NORMAL_ATTACK_RIGHT)
			return false;

		// Check
		return RectToRect((CShape*)pSrc, (CShape*)pDest);

	}
		break;


		/*
			이거 충돌체 미니맵 관련 왜 동일한 코드인데 따로 썼지 ? -> 나중에 수정.
		*/

		
		// 몬스터 충돌체를 기준으로.

	case ST_RECT_MONSTER:

		switch (pDest->GetShape())
		{
		case ST_RECT_ROPE:
		case ST_RECT_OBSTACLE:
		case ST_RECT_PORTAL:
		case ST_RECT_TERRAIN:
		case ST_CIRCLE:
			break;

	

		case ST_RECT_NORMAL_LEFT:
	

			if (((CPlayer*)pDest->GetObj())->GetAttack() != AT_NORMAL_ATTACK_LEFT)
				return false;

			return RectToRect((CShape*)pSrc, (CShape*)pDest);



		case ST_RECT_NORMAL_RIGHT:

			if (((CPlayer*)pDest->GetObj())->GetAttack() != AT_NORMAL_ATTACK_RIGHT)
				return false;

			return RectToRect((CShape*)pSrc, (CShape*)pDest);



		case ST_RECT_SKILL_LEFT:

			if (((CPlayer*)pDest->GetObj())->GetAttack() != AT_SKILL_ATTACK_LEFT)
				return false;

			return RectToRect((CShape*)pSrc, (CShape*)pDest);



		case ST_RECT_SKILL_RIGHT:

			if (((CPlayer*)pDest->GetObj())->GetAttack() != AT_SKILL_ATTACK_RIGHT)
				return false;

			return RectToRect((CShape*)pSrc, (CShape*)pDest);


		}

		break;

	case ST_RECT_TURN: // 몬스터 turn은 pixel과만 충돌

		if (pDest->GetShape() != ST_PIXEL)
			return false;


			return RectToPixel((CShape*)pSrc, (CPixel*)pDest);



	case ST_PIXEL:


		switch (pDest->GetShape())
		{
		case ST_RECT_TURN:
		case ST_RECT_OBSTACLE:
		case ST_RECT_ROPE:
		case ST_RECT_TERRAIN:
			return RectToPixel((CShape*)pDest, (CPixel*)pSrc);

		}

		break;

	case ST_RECT_TERRAIN:

			switch (pDest->GetShape())
			{

			case ST_PIXEL:
				return RectToPixel((CShape*)pSrc, (CPixel*)pDest);

			case ST_RECT_OBSTACLE:
			case ST_RECT_ROPE:
			case ST_RECT_PORTAL:
			case ST_RECT_TERRAIN:
			case ST_CIRCLE:
				break;

			}


		break;

	case ST_RECT_ROPE: 

		// 방어하는 입장.
			switch (pDest->GetShape())
			{

			case ST_RECT_MONSTER_ATTACK:
				// 몬스터 공격 범위와 만난 경우 + 무적.
				if (pSrc->GetObj()->GetBlend())
					return false;

				
			case ST_RECT_PORTAL:
				return RectToRect((CShape*)pSrc, (CShape*)pDest);

			case ST_RECT_OBSTACLE:
			case ST_RECT_TERRAIN:
			case ST_CIRCLE:
			case ST_RECT_ROPE:
				break;

			case ST_PIXEL: 
				return RectToPixel((CShape*)pSrc, (CPixel*)pDest);

			}

		break;



	case ST_RECT_PORTAL:

		switch (pDest->GetShape())
		{
		case ST_RECT_OBSTACLE:
		case ST_RECT_PORTAL:
		case ST_PIXEL:
		case ST_RECT_TERRAIN:
		case ST_CIRCLE: 
			break;

		case ST_RECT_ROPE:
			return RectToRect((CShape*)pSrc, (CShape*)pDest);
		}

		break;


	case ST_RECT_OBSTACLE:

		switch (pDest->GetShape())
		{
		case ST_RECT_ROPE:
		case ST_RECT_OBSTACLE:
		case ST_RECT_PORTAL:
		case ST_RECT_TERRAIN:
		case ST_CIRCLE:
			break;

		case ST_PIXEL:
			return RectToPixel((CShape*)pSrc, (CPixel*)pDest);

		}

		break;



	}


	return false;
}

bool Math::CircleToCircle(CShape * pSrc, CShape * pDest)
{

	POSITION src = pSrc->GetCenter();

	float srcR = pSrc->GetSize().x / 2.f;

	POSITION dest = pDest->GetCenter();

	float destR = pDest->GetSize().x / 2.f;

	if (PointToPointDist(src, dest) <= (destR + srcR))
		return true;

	return false;
}

bool Math::CircleToRect(CShape * pSrc, CShape * pDest)
{

	// src 원 dest 사각형 

	// 원의 중심
	POSITION Circle = pSrc->GetCenter();
	float l, t, r, b;

	// 사각형
	l = pDest->GetLeft();
	t = pDest->GetTop();
	r = pDest->GetRight();
	b = pDest->GetBottom();

	// 반지름
	float    CircleR = pSrc->GetSize().x / 2.f;


	// 원의 중심이 사각형의 왼쪽과 오른쪽 사이에 있거나
	// 위쪽과 아래쪽 사이에 있는 경우와 그러지 않은 경우로 나눈다

	if (Circle.y > pDest->GetTop() && Circle.y < pDest->GetBottom() ||
		Circle.x > pDest->GetLeft() && Circle.x < pDest->GetRight())
	{

		// 확장된 사각형을 구한다.
		l -= CircleR;
		t -= CircleR;
		r += CircleR;
		b += CircleR;

		// 확장된 사각형에 원의 중심이 있다면,
		// 충돌이다.

		if (Circle.x < l)
			return false;
		else if (Circle.x > r)
			return false;

		if (Circle.y < t)
			return false;
		else if (Circle.y > b)
			return false;

		return true;
	}

	POSITION RectPos[4];
	RectPos[0] = POSITION(l, t);
	RectPos[1] = POSITION(l, b);
	RectPos[2] = POSITION(r, t);
	RectPos[3] = POSITION(r, b);

	// 사각형의 4 꼭짓점이 
	// 원에 속하면 충돌

	for (size_t i = 0; i < 4; ++i)
	{
		if (PointToPointDist(RectPos[i], Circle) <= CircleR)
			return true;
	}


	return false;
}

bool Math::RectToRect(CShape * pSrc, CShape * pDest)
{
	float srcl = pSrc->GetLeft();
	float srct = pSrc->GetTop();
	float srcr = pSrc->GetRight();
	float srcb = pSrc->GetBottom();


	float destl = pDest->GetLeft();
	float destt = pDest->GetTop();
	float destr = pDest->GetRight();
	float destb = pDest->GetBottom();


	if (srcl <= destr &&
		srcr >= destl &&
		srct <= destb &&
		srcb >= destt)
		return true;
	else
		return false;


	
}

bool Math::RectToRect(const POSITION & tSrcPos, const POSITION & tSrcSize,
	const POSITION & tDestPos, const POSITION & tDestSize)
{
	float srcl = tSrcPos.x;
	float srct = tSrcPos.y;
	float srcr = tSrcPos.x + tSrcSize.x;
	float srcb = tSrcPos.y + tSrcSize.y;


	float destl = tDestPos.x;
	float destt = tDestPos.y;
	float destr = tDestPos.x + tDestSize.x;
	float destb = tDestPos.y + tDestSize.y;


	if (srcl <= destr &&
		srcr >= destl &&
		srct <= destb &&
		srcb >= destt)
		return true;
	else
		return false;

}

bool Math::RectToPixel(CShape * pSrc, CPixel * pDest)
{
	// 렉트의 좌표를 모두 돌리면서 해당 위치가 '특정 색상'과 겹치는지 검사한다.

	COLORREF Color;
	POSITION tSrc, tDest;
	
	tSrc = pSrc->GetPos();
	tDest = tSrc + pSrc->GetSize();

	int iWidth = pDest->GetWidth();
	int iHeight = pDest->GetHeight();

	// 좌표 음수값 들어가서 터지는 건 나중에 막는 걸로.
	int iStartX, iEndX;
	int iStartY, iEndY;

	iStartX = tSrc.x < 0 ? 0 : tSrc.x;
	iEndX = tDest.x >= iWidth ? iWidth - 1 : tDest.x;

	iStartY = tSrc.y < 0 ? 0 : tSrc.y;
	iEndY = tDest.y >= iHeight ? iHeight - 1 : tDest.y;


	const vector<PIXEL>& vecPixel = pDest->GetPixelVec();


	switch (pSrc->GetShape())
	{

	case ST_RECT_TURN:

		Color = MONSTER_OBSTACLE;

		for (size_t i = iStartY; i <= iEndY; ++i)
		{
			for (size_t j = iStartX; j <= iEndX; ++j)
			{
				// Rect의 한 점에서의 위치와 '특정' 픽셀이 겹치면
				// 충돌이다.
				int idx = i * iWidth + j;
				const PIXEL& pixel = vecPixel[idx];

				if (pixel.r == GetRValue(Color) && pixel.g == GetGValue(Color) && pixel.b == GetBValue(Color))
					return true;

			}
		}



		break;

	case ST_RECT_OBSTACLE:
		Color = OBSTACLE;

		for (size_t i = iStartY; i <= iEndY; ++i)
		{
			for (size_t j = iStartX; j <= iEndX; ++j)
			{
				
				int idx = i * iWidth + j;
				const PIXEL& pixel = vecPixel[idx];

				if (pixel.r == GetRValue(Color) && pixel.g == GetGValue(Color) && pixel.b == GetBValue(Color))
					return true;

			}
		}

		break;

	case ST_RECT_TERRAIN:
		Color = MAGENTA;

		for (size_t i = iStartY; i <= iEndY; ++i)
		{
			for (size_t j = iStartX; j <= iEndX; ++j)
			{
			
				int idx = i * iWidth + j;
				const PIXEL& pixel = vecPixel[idx];

				if (pixel.r == GetRValue(Color) && pixel.g == GetGValue(Color) && pixel.b == GetBValue(Color))
					return true;

			}
		}


		break;

	case ST_RECT_ROPE:
		Color = ROPE;

		for (size_t i = iStartY; i <= iEndY; ++i)
		{
			for (size_t j = iStartX; j <= iEndX; ++j)
			{
				
				int idx = i * iWidth + j;
				const PIXEL& pixel = vecPixel[idx];

				if (pixel.r == GetRValue(Color) && pixel.g == GetGValue(Color) && pixel.b == GetBValue(Color))
				{
					pDest->SetRopePos(j, i);
					return true;
				}
			}
		}


		break;

	}

	

	
	return false;
}


bool Math::PointInRect(const _tagPosition & tRectSrc, const _tagPosition & tRectDest, const _tagPosition & tPoint)
{
	if (tRectSrc.x > tPoint.x)
		return false;

	else if (tRectSrc.y > tPoint.y)
		return false;

	else if (tRectDest.x < tPoint.x)
		return false;

	else if (tRectDest.y < tPoint.y)
		return false;


	return true;
}


float Math::PointToPointDist(_tagPosition tPos, _tagPosition mPos)
{
	return sqrt((tPos.x - mPos.x) * (tPos.x - mPos.x) + (tPos.y - mPos.y) * (tPos.y - mPos.y));

}

bool Math::PointInScreen(_tagPosition tPos)
{
	if (tPos.x >= 0.f && tPos.x <= 1280.f &&
		tPos.y >= 0.f && tPos.y <= 720.f)
		return true;


	return false;
}




