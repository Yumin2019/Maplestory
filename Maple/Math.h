#pragma once


class Math
{
	//Collision
public:

	// Collision 검사 함수.
	static bool CollisionCheck(class CCollider* pSrc, class CCollider* pDest);

	// Circle vs Circle
	static bool CircleToCircle(class CShape* pSrc, class CShape* pDest);

	// Circle Vs Rect
	static bool CircleToRect(class CShape* pSrc, class CShape* pDest);

	// Rect Vs Rect
	static bool RectToRect(class CShape* pSrc, class CShape* pDest);

	// Rect Vs Rect
	static bool RectToRect(const struct _tagPosition& tSrcPos, const struct _tagPosition& tSrcSize,
		const struct _tagPosition& tDestPos, const struct _tagPosition& tDestSize);

	// 신규 ! 
	// Rect Vs Pixel
	static bool RectToPixel(class CShape* pSrc, class CPixel* pDest);

	// Pixel Vs Pixel
	static bool PixelToPixel(class CPixel* pSrc, class CPixel* pDest);

	// Point Vs Rect
	static bool PointInRect(const struct _tagPosition& tRectSrc,
		const  struct _tagPosition& tRectDest,
		const  struct _tagPosition& tPoint);

	// Circle Vs Pixel 은 안 쓸 예정.

	// 두 점 사이의 거리
	static float PointToPointDist(struct _tagPosition tPos, struct _tagPosition mPos);

	// 사각형의 점
	static bool PointInScreen(struct _tagPosition tPos);

private:
	bool PixelCollision(class CPixel* pSrc, class CPixel* pDest);
	bool PlayerToMonster(class CShape* pSrc, class CShape* pDest);
};

