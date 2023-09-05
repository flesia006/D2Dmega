#include "framework.h"
#include "Texture.h"
#include "Render/TextureShader.h"
#define  PI  3.141592f

Texture::Texture(wstring strImage, wstring strShader)
{
	m_pShader      = new TextureShader(strShader);
	m_strImageFile = strImage;

	m_ptrSRV = SRVMANAGER->CreateShaderResourceView(strImage);
	Vector2  size = SRVMANAGER->FindSpriteSize(strImage);
	m_ImageSize = size;
	CreateVertexBuffer(0.0f, 0.0f, size.x, size.y);
}

Texture::Texture(wstring strImage, wstring strShader, int startX, int startY, int endX, int endY)
{


	m_pShader = new TextureShader(strShader);
	m_strImageFile = strImage;

	m_ptrSRV = SRVMANAGER->CreateShaderResourceView(strImage);
	Vector2  size = SRVMANAGER->FindSpriteSize(strImage);
	m_ImageSize = size;
	CreateVertexBuffer((float)startX, (float)startY, (float)endX, (float) endY);

}

Texture::~Texture()
{
	SAFE_RELEASE(this->m_pVertexBuffer);
	SAFE_DELETE(m_pShader);
}
///////////////////////////////////////////////////////
//  1. W Matrix 계산
//  2. Shader연결 ShaderResource view update
//  3. UV 바꾸는 작업
//  4. Shader W,V,P Update
//////////////////////////////////////////////////////
void Texture::Update(Matrix V, Matrix P)
{
	Matrix W, T, S, R;

	// x,y,z위 배경
	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, 0.0f);
	// scale
	D3DXMatrixScaling(&S, m_Scale.x * m_SpriteSize.x,
		m_Scale.y * m_SpriteSize.y,
		0.0f);
	// rotate
	D3DXMatrixRotationYawPitchRoll(&R,
		m_Rotation.y * PI / 180.0f, m_Rotation.x * PI / 180.0f,
		m_Rotation.z * PI / 180.0f);

	W = S * R * T;  

	// Shader를 가져오기
	m_ptrSRV = SRVMANAGER->FindShaderResourceView(m_strImageFile);
	Vector2 size = SRVMANAGER->FindSpriteSize(m_strImageFile);

	this->m_ImageSize = size;  // NCD용 
	UpdateBlock();             // UV좌표 

	// Shader에 setting

	m_pShader->SetShaderResourceView(0, m_ptrSRV);
	m_pShader->Update(W, V, P);

}

void Texture::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// IA단계 : Device
	DeviceContext->IASetVertexBuffers(0, // slot
		1, // buffer갯수)
		&this->m_pVertexBuffer, &stride, &offset);
	DeviceContext->IASetInputLayout(m_pShader->GetLayout());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Vertex Stage --> OM단계
	m_pShader->Draw(6, 0);

}

void Texture::UpdateBlock()
{
	Vertex  vertices[6];

	// TODO : index로 할수 있게끔

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f); // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f); // 1
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f); // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f); // 3
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f); // 1
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f); // 2

	// 비율계산 -> NCD
	float  startX = m_SpriteOffset.x / m_ImageSize.x;
	float  startY = m_SpriteOffset.y / m_ImageSize.y;
	float  endX   = (m_SpriteOffset.x +m_SpriteSize.x ) / m_ImageSize.x;
	float  endY   = (m_SpriteOffset.y + m_SpriteSize.y) / m_ImageSize.y;

	// UV값   :  (uv)x,y,z
	// u  --> bitmap x방향 ( 우측방향이 증가)
	// v  --> bitmap y방향 ( 하단방향이 증가)

	vertices[0].Uv = Vector2(startX, endY);     // 0
	vertices[1].Uv = Vector2(startX, startY);   // 1
	vertices[2].Uv = Vector2(endX, endY);       // 3
	vertices[3].Uv = Vector2(endX, endY);       // 3
	vertices[4].Uv = Vector2(startX, startY);   // 1
	vertices[5].Uv = Vector2(endX, startY);     // 2

	// Map -> Uumap 사이에서 수정
	{
		D3D11_MAPPED_SUBRESOURCE subResource;

		DeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, vertices, sizeof(Vertex)*6);
		}
		DeviceContext->Unmap(m_pVertexBuffer, 0);
	}
}

void Texture::SetMultiMultiShaderResourceView(wstring strFile)
{
	ID3D11ShaderResourceView* ptrSRV = nullptr;

	SRVMANAGER->CreateShaderResourceView(strFile);
	ptrSRV = SRVMANAGER->FindShaderResourceView(strFile);
	m_pShader->SetShaderResourceView(1, ptrSRV);
}

void Texture::CreateShaderResourceView(wstring strFile)
{
	SRVMANAGER->CreateShaderResourceView(strFile);
}

void Texture::CreateMultiShaderResourceView(wstring strFile)
{
	ID3D11ShaderResourceView* ptrSRV = nullptr;

	SRVMANAGER->CreateShaderResourceView(strFile);
	ptrSRV = SRVMANAGER->FindShaderResourceView(strFile);
	m_pShader->SetShaderResourceView(1, ptrSRV);

}
void Texture::UpdateColorBuffer(int Path, Color color, float Time, float dummy1, float dummy2)
{
	m_pShader->UpdateColorBuffer(color, Path, Time, dummy1, dummy2);
}
Vector2 Texture::GetTextureRealSize()
{
	Vector2 size;

	size.x = m_Scale.x * m_SpriteSize.x;
	size.y = m_Scale.y * m_SpriteSize.y;

	return size;
}
///////////////////////////////////////////////
//      1  |  2
//     -------    
//      0  |  3
///////////////////////////////////////////////
void Texture::CreateVertexBuffer(float startX, float startY, float endX, float endY)
{
	Vertex  vertices[6];

	// TODO : index로 할수 있게끔

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f); // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f); // 1
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f); // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f); // 3
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f); // 1
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f); // 2

	Vector2 size = SRVMANAGER->FindSpriteSize(m_strImageFile);
	m_SpriteOffset  = Vector2(startX, startY);
	m_SpriteSize = Vector2(endX-startX, endY-startY);

	// UV값  NCD좌표로 변환 : 800 -> 1.0f, 20/800->0.16f

	startX = startX / size.x;
	startY = startY / size.y;
	endX   = endX   / size.x;
	endY   = endY   / size.y;

	// UV값   :  (uv)x,y,z
	// u  --> bitmap x방향 ( 우측방향이 증가)
	// v  --> bitmap y방향 ( 하단방향이 증가)

	vertices[0].Uv = Vector2(startX, endY);     // 0
	vertices[1].Uv = Vector2(startX, startY);   // 1
	vertices[2].Uv = Vector2(endX, endY);       // 3
	vertices[3].Uv = Vector2(endX, endY);       // 3
	vertices[4].Uv = Vector2(startX, startY);   // 1
	vertices[5].Uv = Vector2(endX, startY);     // 2

	// 위에 선언된 데이터를 VertextBuffer에 Setting
	// DX에서 Craete,Get()를 사용할때 변수명칭 앞에 "I"자가 있으면
	// 무조건 Desctrpion이 붙는다
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // 변수가 0로 초기화
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *생성한후에 변경할수  한다
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *생성한후에 변경할수  한다
		desc.ByteWidth = sizeof(Vertex) * 6;          // Vector3,Color ( 3*4 + 4*4)
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // 12 +16 = 28

		//* D3D11_BIND_VERTEX_BUFFER = 0x1L,     정점
		//* D3D11_BIND_INDEX_BUFFER = 0x2L,      중복되는 정점을 Index로 구분하여 사용
		//* D3D11_BIND_CONSTANT_BUFFER = 0x4L,   상수Buffer, User가 HLSL에 데이터 보낼때
		// D3D11_BIND_SHADER_RESOURCE = 0x8L,    ImageFile

	}
	D3D11_SUBRESOURCE_DATA    data;    // input
	{
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;
	}

	HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pVertexBuffer);
	assert(SUCCEEDED(hr));

}

