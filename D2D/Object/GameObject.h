#pragma once
class Collider;
class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() = default;

//public:
//	GameObject* GetOwner() const { return owner; }
//	void SetOwner(GameObject* obj) { owner = obj; }
//
//protected:
//	GameObject* owner = nullptr;

public:		// 가상함수
	virtual void			Update() = 0;    // 순수 가상 함수
	virtual void			Render() = 0;                     // 순수 가상 함수
	virtual void			Reset() {};
	virtual void			PostRender() {};



public:		// Setter
	void					SetScale(float x, float y)				{ m_Scale = Vector2(x, y); }
	void					SetScale(Vector2 scale)					{ m_Scale = scale; }
	void					SetPosition(float x, float y)			{ m_Position = Vector2(x, y); }
	void					SetPosition(Vector2 position)			{ m_Position = position; }
	void					SetRotation(float x, float y, float z)	{ m_Rotation = Vector3(x, y, z); }
	void					SetRotation(Vector3 rot)				{ m_Rotation = rot; }
	void					SetName(string name)					{ m_strName = name; }
	void					SetActive(bool value)					{ m_bActive = value;}
	void					SetDebug(bool value)					{ m_bDebug = value; }


public:		// Getter
	Vector2					GetPosition()		{ return	m_Position; }
	Vector2					GetScale()			{ return	m_Scale; }
	Vector3					GetRotation()		{ return	m_Rotation; }
	bool					IsActive()			{ return	m_bActive; }
	string					GetObjectName()		{ return	m_strName; }
	class Collider* GetCollider() { return m_pCollider; }
protected:
	Vector2					m_Position			= Vector2(0.0f, 0.0f);
	Vector2					m_Scale				= Vector2(1.0f, 1.0f);  // 0으로 하면 안됨
	Vector3					m_Rotation			= Vector3(0.0f, 0.0f, 0.0f);
	string					m_strName;                   // Object Name
	bool					m_bActive			= false;           // Active가  true인 경우만 Update,Render
	bool					m_bCollisionCheck	= false;  // 충돌이 되면 True
	class   Collider*       m_pCollider = nullptr;
	bool                    m_bDebug = false;

};

