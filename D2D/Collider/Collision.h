#pragma once
class Collision
{
public:
	Collision();
	~Collision();
public:
	static bool HitTest(Collider* A, Collider* B);
	static bool LineInterscetion(Vector2 AP1, Vector2 AP2, Vector2 BP1, Vector2 BP2, Vector2& result);
private:
	static int  CCW(Vector2 p1, Vector2 p2, Vector2 p3);
	static int  IsLeft(Vector2 p1, Vector2 p2);
	static bool AABB(Collider* A, Collider* B);
	static bool OBB(Collider* A, Collider* B);
};

