#pragma once


class LineShader;
class Collider
{
public:
	Collider();
	Collider(Vector2 LT, Vector2 RB, Vector3 rot = {0, 0, 0});
	Collider(Vector2 scale, Vector2 position, int dummy);
	~Collider();



public:
	void  Update();
	void  UpdateColor(Color color);
	void  Render();

public: // Setter
	void	 SetScale(float x, float y)				  { m_Scale		= Vector2(x, y); }
	void	 SetScale(Vector2 scale)				  { m_Scale		= scale; }
	void	 SetPosition(float x, float y)			  { m_Position  = Vector2(x, y); }
	void	 SetPosition(Vector2 position)			  { m_Position  = position; }
	void	 SetRotation(float x, float y, float z)   { m_Rotation  = Vector3(x, y, z); }
	void	 SetRotation(Vector3 rot)				  { m_Rotation  = rot; }
	void	 SetActive(bool value) { m_bActive = value; }

public:   // Getter
	Vector2  GetPosition()	{ return m_Position; }
	Vector2  GetScale()		{ return m_Scale; }
	Vector3  GetRotation()	{ return m_Rotation; }
	bool     IsActive()		{ return m_bActive; }
	int		 GetType()		{ return m_type; }

protected:
	void     CreateVertexBuffer();
	Vector2  m_Position				= Vector2(0.0f, 0.0f);
	Vector2  m_Scale				= Vector2(1.0f, 1.0f);			// 0으로 하면 안됨
	Vector3  m_Rotation				= Vector3(0.0f, 0.0f, 0.0f);
	bool     m_bActive				= false;						// Active가  true인 경우만 Update,Render
	bool     m_bCollisionCheck		= false;						// 충돌이 되면 True
	Color    m_Color				= Color(0.0f, 1.0f, 0.0f, 1.0f);
	int      m_type = 0;


	struct Vertex
	{
		Vector3 Position;
		Color   Color;
	};
	vector<Vector2*>		m_cvLines;  // (0,1) Line, (2,3)
	shared_ptr<class LineShader>	m_pShader;
	ID3D11Buffer*			m_pVertexBuffer = nullptr;
	vector<Collider*>		cv_Grounds;

	





};

