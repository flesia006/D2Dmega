#include "framework.h"
#include "Collision.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}
bool Collision::HitTest(Collider* A, Collider* B)
{
	bool  result = AABB(A, B);
	return result;
}
//////////////////////////////////////////////////
//  1. CCW를 이용하여 두개의 Line이 교차되어 있는지 판단
//  2. 교차점을 구하는것
//////////////////////////////////////////////////
bool Collision::LineInterscetion(Vector2 AP1, Vector2 AP2, Vector2 BP1, Vector2 BP2, Vector2& result)
{
	bool ret;

    // https://gaussian37.github.io/math-algorithm-line_intersection/

	// A Line 기준으로 B Line이 교차하는 지 확인한다.
	int AB = CCW(AP1, AP2, BP1) * CCW(AP1, AP2, BP2);
	// B Line 기준으로 A Line이 교차하는 지 확인한다.
	int BA = CCW(BP1, BP2, AP1) * CCW(BP1, BP2, AP2);


	// A 과 B가 일직선 상에 있는 경우
	if (AB == 0 && BA == 0)
	{
		Vector2 ap1 = AP1;
		Vector2 ap2 = AP2;
		Vector2 bp1 = BP1;
		Vector2 bp2 = BP2;

		// A Line의 점의 크기 순서를 p1 < p2 순서로 맞춘다.
		if (IsLeft(AP2, AP1))
		{
			ap1 = AP2;
			ap2 = AP1;
		}
		// B Line의 점의 크기 순서를 p1 < p2 순서로 맞춘다.
		if (IsLeft(BP2, BP1))
		{
			bp1 = BP2;
			bp2 = BP1;
		}
		// ap1 -----------l ap2
		//         bp1 ----------- bp2
		// 위 조건을 만족하는 지 살펴본다.
		ret = (IsLeft(bp1, ap2)) && (IsLeft(ap1, bp2));
		if (ret)
		{
			result = bp1;
			return true;
		}
		else
			return false;
	}
	// A와 B가 일직선 상에 있지 않는 경우
	else
		ret = (AB <= 0) && (BA <= 0);

	if (!ret)		return false;

  // http://www.gisdeveloper.co.kr/?p=89

	double under = (BP2.y - BP1.y) * (AP2.x - AP1.x) - (BP2.x - BP1.x) * (AP2.y - AP1.y);
	double _t = (double)(BP2.x - BP1.x) * (AP1.y - BP1.y) - (BP2.y - BP1.y) * (AP1.x - BP1.x);
	double _s = (double)(AP2.x - AP1.x) * (AP1.y - BP1.y) - (AP2.y - AP1.y) * (AP1.x - BP1.x);

	double t = _t / under;
	double s = _s / under;

	result.x = (float)(AP1.x + t * (double)(AP2.x - AP1.x));
	result.y = (float)(AP1.y + t * (double)(AP2.y - AP1.y));

	return true;
}
// 면적이 양수면 시계 반대 방향
int Collision::CCW(Vector2 p1, Vector2 p2, Vector2 p3)
{
	double area = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

	if (area > 0)
		return 1;       // 시계반대방향
	else if (area < 0)
		return -1;      // 시계방향
	else
		return 0;       // 일직선
}
// 두점을 주어졌을때 왼쪽에 있는가 판단
int Collision::IsLeft(Vector2 p1, Vector2 p2)
{
	int ret;
	if (p1.x == p2.x)
		ret = (p1.y <= p2.y);
	else
		ret = (p1.x <= p2.y);
	return ret;
}
/////////////////////////////////////////////////
// AABB(Axis Aligned Bounding Box) 충돌 감지
//////////////////////////////////////////////////
bool Collision::AABB(Collider* A, Collider* B)
{
	float ax1 = A->GetPosition().x - A->GetScale().x * 0.5f;
	float ax2 = A->GetPosition().x + A->GetScale().x * 0.5f;
	float ay1 = A->GetPosition().y - A->GetScale().y * 0.5f;
	float ay2 = A->GetPosition().y + A->GetScale().y * 0.5f;

	float bx1 = B->GetPosition().x - B->GetScale().x * 0.5f;
	float bx2 = B->GetPosition().x + B->GetScale().x * 0.5f;
	float by1 = B->GetPosition().y - B->GetScale().y * 0.5f;
	float by2 = B->GetPosition().y + B->GetScale().y * 0.5f;

	if( (max(ax1,ax2) < min(bx1,bx2)) ||
		 (max(bx1,bx2)  < min(ax1, ax2)) )
		return false;
	
	if ((max(ay1, ay2) < min(by1, by2)) ||
		(max(by1, by2) < min(ay1, ay2)))
		return false;

	return true;
}
//////////////////////////////////////////////////
// OBB(Oriented Bounding Box) 충돌 감지
// 회전되어 있는 경우
//////////////////////////////////////////////////
bool Collision::OBB(Collider* A, Collider* B)
{
	return false;
}
