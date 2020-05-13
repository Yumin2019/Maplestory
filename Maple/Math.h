#pragma once


class Math
{
	//Collision
public:

	// Collision �˻� �Լ�.
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

	// �ű� ! 
	// Rect Vs Pixel
	static bool RectToPixel(class CShape* pSrc, class CPixel* pDest);

	// Pixel Vs Pixel
	static bool PixelToPixel(class CPixel* pSrc, class CPixel* pDest);

	// Point Vs Rect
	static bool PointInRect(const struct _tagPosition& tRectSrc,
		const  struct _tagPosition& tRectDest,
		const  struct _tagPosition& tPoint);

	// Circle Vs Pixel �� �� �� ����.

	// �� �� ������ �Ÿ�
	static float PointToPointDist(struct _tagPosition tPos, struct _tagPosition mPos);

	// �簢���� ��
	static bool PointInScreen(struct _tagPosition tPos);

private:
	bool PixelCollision(class CPixel* pSrc, class CPixel* pDest);
	bool PlayerToMonster(class CShape* pSrc, class CShape* pDest);
};

