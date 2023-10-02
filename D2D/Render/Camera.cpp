#include "framework.h"
#include "Camera.h"
#include "Object/MegaMan.h"

Camera::Camera()
{
	// P,V를 만들기 
	Matrix  V, P;

	Vector3 eye = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 at = Vector3(0.0f, 0.0f, 0.1f);
	Vector3 temp = eye + at;

	D3DXMatrixLookAtLH(&V, &eye, &temp, &up);

	D3DXMatrixOrthoOffCenterLH(&P, -(float)(MAIN->GetWidth() * 0.5f),
		(float)(MAIN->GetWidth()  * 0.5f),
		-(float)(MAIN->GetHeight() * 0.5f+48.0f),
		(float)(MAIN->GetHeight()  * 0.5f+48.0f), -1.0f, 1.0f);
	m_Projection = P;
	m_View = V;

	{
		Matrix R = P;
		printf("%2.4f %2.4f %2.4f %2.4f\n", R._11, R._12, R._13, R._14);
		printf("%2.4f %2.4f %2.4f %2.4f\n", R._21, R._22, R._23, R._24);
		printf("%2.4f %2.4f %2.4f %2.4f\n", R._31, R._32, R._33, R._34);
		printf("%2.4f %2.4f %2.4f %2.4f\n", R._41, R._42, R._43, R._44);
	}
	{
		printf("\n");
		Matrix R = V;
		printf("%2.4f %2.4f %2.4f %2.4f\n", R._11, R._12, R._13, R._14);
		printf("%2.4f %2.4f %2.4f %2.4f\n", R._21, R._22, R._23, R._24);
		printf("%2.4f %2.4f %2.4f %2.4f\n", R._31, R._32, R._33, R._34);
		printf("%2.4f %2.4f %2.4f %2.4f\n", R._41, R._42, R._43, R._44);
	}

}
void Camera::FollowingCamera(Vector2 & position)
{
	position = m_ptrObject->GetPosition();

}
void Camera::ShakingCamera(Vector2 & position)
{
}
void Camera::MoveCamera(Vector2& position)
{

}
Camera::~Camera()
{
}

void Camera::Update()
{
	Vector2   position = GetPosition();
	MegaMan* pMM = nullptr;

	if (m_ptrObject)
		pMM = dynamic_cast<class MegaMan*>(m_ptrObject);

	if (pMM->IsTouchBossRoom() && !m_MovingCamera)
	{
		SOUND->Stop("bgm");
		m_DoorOpen += DELTA;
		if (m_DoorOpen >= 0.65f)
		{
			m_MovingCamera = true;
			SetCornerRight(Vector2(6680 - MAIN->GetWidth() * 0.5f, 0.0f));
		}
	}

	if (m_ptrObject)
	{		
		if (!m_MovingCamera)
		{
			FollowingCamera(position);
		}
		else
		{
			position.x += 700.0f * DELTA;
		}
	}


	// CAMERA 제한

	if (!m_bCameraShake)
	{
		if (position.x < m_CornerLeft.x)
			position.x = m_CornerLeft.x;
		if (position.x > m_CornerRight.x)
		{
			position.x = m_CornerRight.x;

		}

		if (position.y < m_CornerLeft.y)
			position.y = m_CornerLeft.y;
		if (position.y > m_CornerRight.y)
			position.y = m_CornerRight.y;
	}



	SetPosition(position);


	// P,V를 만들기 
	Matrix  V, P;

	Vector3 eye  = Vector3(m_Position.x, m_Position.y, 0.0f);
	Vector3 up   = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 at   = Vector3(0.0f, 0.0f, 0.1f);
	Vector3 temp = eye + at;

	D3DXMatrixLookAtLH(&V, &eye, &temp, &up);
	m_View = V;
}

void Camera::Update(Matrix V, Matrix P)
{
}

void Camera::Render()
{
}

void Camera::WCtoVC(Vector2& position)
{
	Matrix vp = m_View * m_Projection;

	D3DXMatrixInverse(&vp, NULL, &vp);
	position.x = (position.x / MAIN->GetWidth() * 2.0f - 1.0f);
	position.y = -(position.y / MAIN->GetHeight() * 2.0f - 1.0f);

	D3DXVec2TransformCoord(&position, &position, &vp);
}

void Camera::VCtoWC(Vector2& position)
{
	Matrix vp = m_View * m_Projection;
	Matrix S, T, W;

	D3DXMatrixScaling(&S, MAIN->GetWidth() * 0.5f, 
		         -MAIN->GetHeight() * 0.5f, 0.0f);
	D3DXMatrixTranslation(&T, MAIN->GetWidth() * 0.5f,
		          MAIN->GetHeight() * 0.5f, 0.0f);
	W = vp * S * T;
	D3DXVec2TransformCoord(&position, &position, &W);
}

bool Camera::IsCornerLeft(Vector2 pos)
{

	if (pos.x >= m_CornerLeft.x && 
		pos.x < m_CornerLeft.x + (m_CornerRight.x - m_CornerLeft.x) / 3)
		return true;
	else
		return false;
}

bool Camera::IsCornerRight(Vector2 pos)
{
	if (pos.x < m_CornerRight.x && 
		pos.x < m_CornerLeft.x + ((m_CornerRight.x - m_CornerLeft.x) / 3) * 2)
		return true;
	else
		return false;
}


