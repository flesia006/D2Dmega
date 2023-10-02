#include "framework.h"
#include "Collider.h"
#define  PI 3.141592f

Collider::Collider()
{
	SetActive(true);
	m_pShader = make_shared<LineShader>(L"./Shader/Color.hlsl");
	CreateVertexBuffer();
}

Collider::Collider(Vector2 LT, Vector2 RB, Vector3 rot)
	: m_Rotation(rot)
{
	SetActive(true);
	m_Scale = Vector2(RB.x - LT.x, LT.y - RB.y);
	m_Position = Vector2((RB.x + LT.x) / 2, (LT.y + RB.y) / 2);

	m_pShader = make_shared<LineShader>(L"./Shader/Color.hlsl");
	CreateVertexBuffer();
}

Collider::Collider(Vector2 scale, Vector2 position, int dummy)
	: m_Scale(scale), m_Position(position), m_type(dummy)
{
	SetActive(true);
	m_pShader = make_shared<LineShader>(L"./Shader/Color.hlsl");
	CreateVertexBuffer();
}


Collider::~Collider()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

void Collider::Update()
{
	if (!IsActive())
		return;
	Matrix W, T, S, R;
	Matrix V = CAMERA->GetView();
	Matrix P = CAMERA->GetProjection();

	D3DXMatrixTranslation(&T, GetPosition().x, GetPosition().y, 0.0f);
	D3DXMatrixScaling(&S, GetScale().x, GetScale().y, 0.0f);
	D3DXMatrixRotationYawPitchRoll
		(&R, 
		GetRotation().y * PI / 180.0f,
		GetRotation().x * PI / 180.0f,
		GetRotation().z * PI / 180.0f
		);

	W = S * R * T;
	m_pShader->UpdateColorBuffer(m_Color);
	m_pShader->Update(W, V, P);
}

void Collider::UpdateColor(Color color)
{

	m_Color = color;
	m_pShader->UpdateColorBuffer(m_Color);

}

void Collider::Render()
{
	if (!IsActive())
		return;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// IA단계  : Device
	DeviceContext->IASetVertexBuffers(0,   // slot
		1,   // buffer의 갯수
		&this->m_pVertexBuffer,
		&stride,
		&offset);
	DeviceContext->IASetInputLayout(m_pShader->GetLayout());

	// D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	// D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// VS _OM단계
	m_pShader->Draw(5, 0);
}

void Collider::CreateVertexBuffer()
{
	Vertex         vertices[10];
	// 1. 정점을 만든다  --> 시계방향
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 2
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0


	D3D11_BUFFER_DESC       desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // typedef 및에 계속 typdef로 된경우가 많음
	desc.Usage = D3D11_USAGE_DEFAULT;             // GPU로 데이터가 넘어간후 변경 할 수 없음
	desc.ByteWidth = sizeof(Vertex) * 5;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // 정점Buffer, IndexBuffer, ConstantBuffer

	D3D11_SUBRESOURCE_DATA  data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // memset(&data,0x00,sixeof(...))

	data.pSysMem = vertices; // 정점에 대한 데이터의 주소값

	// 자원은 디바이스로 만든다,(Buffer,Texture,Constant)
	// com interface로 된것은 생성할때 DX11에서는 descrtion이 필요하다
	// com interface로 된것은 생성한것은 Release로 지워야한다
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);

	assert(SUCCEEDED(hr));
}
