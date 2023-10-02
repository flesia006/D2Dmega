#pragma once
#define   CAMERA Camera::GetInstance()
class Camera
{
public:
	static Camera* GetInstance()
	{
		static Camera singleTone;
		return &singleTone;
	}
public:
	void  Update();                  // Camera 위치 Update
	void  Update(Matrix V,Matrix P); // 일반적인 
	void  Render();
	void  WCtoVC(Vector2& position);
	void  VCtoWC(Vector2& position);

public:
	void  SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void  SetPosition(Vector2 pos)      { m_Position = pos; }
	void  SetGameObject(GameObject* pObject) { m_ptrObject = pObject; }
	void  SetCornerLeft(Vector2 value) { m_CornerLeft = value; }
	void  SetCornerRight(Vector2 value) { m_CornerRight = value; }

	float GetCenter() { return (m_CornerLeft.x + m_CornerRight.x) / 2; }

	bool  IsCornerLeft(Vector2 pos);
	bool  IsCornerRight(Vector2 pos);

	Vector2   GetPosition()  { return m_Position; }

public:
	Matrix  GetProjection() { return m_Projection; }
	Matrix  GetView()       { return m_View; }

private:
	void     FollowingCamera(Vector2& position);
	void     ShakingCamera(Vector2& position);
	void     MoveCamera(Vector2& position);


private:
	Texture*		m_pTexture  = nullptr;
	GameObject*		m_ptrObject = nullptr;    // following Camear
	Matrix			m_Projection;
	Matrix			m_View;
	Vector2			m_Position     = Vector2(0.0f, 0.0f);
	Vector2			m_CornerLeft   = Vector2(0.0f, 0.0f);
	Vector2			m_CornerRight  = Vector2(0.0f, 0.0f);
	bool			m_bCameraShake = false;      // Player 공격을 당할때 true
	bool            m_MovingCamera = false;
	float           m_DoorOpen = 0.0f;

public:
	~Camera();
private:
	Camera();
};

