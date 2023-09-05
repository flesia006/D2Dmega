#include  "framework.h"
#include "LineShader.h"

LineShader::LineShader(wstring   shaderFile)
{
	CreateShader(shaderFile);
	CreateInputLayout();
	CreateMatrixBuffer();
	CreateColorBuffer();
}

LineShader::~LineShader()
{
	
	SAFE_RELEASE(m_pVertexShader);   
	SAFE_RELEASE(m_pPixelShader);   
	SAFE_RELEASE(m_pVsBlob);        
	SAFE_RELEASE(m_pPsBlob);       
	SAFE_RELEASE(m_pInputLayout);   
	SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pColorBuffer);
}

void LineShader::Update(Matrix W, Matrix V, Matrix P)
{
	TRANSFORM_DATA  temp;
	// 전치행렬로 변환
	D3DXMatrixTranspose(&temp.Projection, &P);
	D3DXMatrixTranspose(&temp.View, &V);
	D3DXMatrixTranspose(&temp.World, &W);

	// Map -> Uumap 사이에서 수정
	{
		D3D11_MAPPED_SUBRESOURCE subResource;

		DeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, &temp, sizeof(TRANSFORM_DATA));
		}
		DeviceContext->Unmap(m_pMatrixBuffer, 0);
	}

}

void LineShader::UpdateColorBuffer(Color color)
{
	COLOR_DATA  temp;
	temp.color = color;
	temp.Path = 1;
	temp.Time = 0.0f;
	temp.dummy1 = 0.0f;
	temp.dummy2 = 0.0f;

	// Map -> Uumap 사이에서 수정
	{
		D3D11_MAPPED_SUBRESOURCE subResource;

		DeviceContext->Map(m_pColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, &temp, sizeof(COLOR_DATA));
		}
		DeviceContext->Unmap(m_pColorBuffer, 0);
	}
}

void LineShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	// VS단계
	DeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

	// RS단계

	// PS단계
	DeviceContext->PSSetConstantBuffers(1, 1, &m_pColorBuffer);
	DeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	// OM단계
	DeviceContext->Draw(VertexCount, StartVertexLocation);

}

void LineShader::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation)
{
	// VS단계
	DeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

	// RS단계

	// PS단계
	DeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	// OM단계
	DeviceContext->DrawIndexed(IndexCount,
		StartIndexLocation,
		BaseVertexLocation);
}

void LineShader::CreateShader(wstring shaderFile)
{
	HRESULT  hr;
	HRESULT  result;
	ID3D10Blob* pErrorMsgs = NULL;

	// HLSL( VS )을 컴파일후 Blob에 저장
	// Binary Large Object
	// Shader을 생성하기 위해 먼저 Blob생성해야 한다
	{
		hr = D3DX11CompileFromFile(
			shaderFile.c_str(),        // Source File
			NULL,                      // MACRO
			NULL,                      // include
			"VS",                      // Effect.hlsk에 정의된 function명
			"vs_5_0",                  // hlsl에 사용되는 version명
			0,                         // Flag1
			0,                         // Flag2
			NULL,                      // ThredPump 사용안함
			&this->m_pVsBlob,          // 컴파일후 --> Blob에 저장
			&pErrorMsgs,
			&result
		);

		if (pErrorMsgs)
			OutputError(pErrorMsgs);


		assert(SUCCEEDED(hr));
	}

	pErrorMsgs = NULL;

	// HLSL( PS )을 컴파일후 Blob에 저장
	// Binary Large Object
	// Shader을 생성하기 위해 먼저 Blob생성해야 한다
	{
		hr = D3DX11CompileFromFile(
			shaderFile.c_str(),   // Source File
			NULL,                      // MACRO
			NULL,                      // include
			"PS",                      // Effect.hlsk에 정의된 function명
			"ps_5_0",                  // hlsl에 사용되는 version명
			0,                         // Flag1
			0,                         // Flag2
			NULL,                      // ThredPump 사용안함
			&this->m_pPsBlob,          // 컴파일후 --> Blob에 저장
			&pErrorMsgs,
			&result
		);

		if (pErrorMsgs)
			OutputError(pErrorMsgs);


		assert(SUCCEEDED(hr));
	}

	// 만들어진 Blob을 통해 Shader생성
	// VertexShader생성
	{
		hr = Device->CreateVertexShader(m_pVsBlob->GetBufferPointer(),
			m_pVsBlob->GetBufferSize(),
			NULL,
			&this->m_pVertexShader);
		assert(SUCCEEDED(hr));
	}

	// 만들어진 Blob을 통해 Shader생성
	// PixelShader생성
	{
		hr = Device->CreatePixelShader(m_pPsBlob->GetBufferPointer(),
			m_pPsBlob->GetBufferSize(),
			NULL,
			&this->m_pPixelShader);
		assert(SUCCEEDED(hr));
	}
}

void LineShader::CreateInputLayout()
{
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC layout[2];

	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Vector3 float x, y, z
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "COLOR";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Vector3 float x, y, z
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = 12;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	hr = Device->CreateInputLayout(layout, 2, m_pVsBlob->GetBufferPointer(),
		m_pVsBlob->GetBufferSize(),
		&this->m_pInputLayout);
	assert(SUCCEEDED(hr));
}

void LineShader::CreateMatrixBuffer()
{
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // 변수가 0로 초기화
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *생성한후에 변경할수  한다
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *생성한후에 변경할수  한다
		desc.ByteWidth = sizeof(TRANSFORM_DATA);              // Vector3,Color ( 3*4 + 4*4)
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    // 12 +16 = 28

		//* D3D11_BIND_VERTEX_BUFFER = 0x1L,     정점
		//* D3D11_BIND_INDEX_BUFFER = 0x2L,      중복되는 정점을 Index로 구분하여 사용
		//* D3D11_BIND_CONSTANT_BUFFER = 0x4L,   상수Buffer, User가 HLSL에 데이터 보낼때
		// D3D11_BIND_SHADER_RESOURCE = 0x8L,    ImageFile

	}
	HRESULT hr = Device->CreateBuffer(&desc, nullptr, &m_pMatrixBuffer);
	//	printf("%d\n", sizeof(Vertex));
	assert(SUCCEEDED(hr));
}

void LineShader::CreateColorBuffer()
{
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // 변수가 0로 초기화
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *생성한후에 변경할수  한다
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *생성한후에 변경할수  한다
		desc.ByteWidth = sizeof(TRANSFORM_DATA);              // Vector3,Color ( 3*4 + 4*4)
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;    // 12 +16 = 28

		//* D3D11_BIND_VERTEX_BUFFER = 0x1L,     정점
		//* D3D11_BIND_INDEX_BUFFER = 0x2L,      중복되는 정점을 Index로 구분하여 사용
		//* D3D11_BIND_CONSTANT_BUFFER = 0x4L,   상수Buffer, User가 HLSL에 데이터 보낼때
		// D3D11_BIND_SHADER_RESOURCE = 0x8L,    ImageFile

	}
	HRESULT hr = Device->CreateBuffer(&desc, nullptr, &m_pColorBuffer);
	//	printf("%d\n", sizeof(Vertex));
	assert(SUCCEEDED(hr));
}

void LineShader::OutputError(ID3D10Blob* error)
{
	string str = (char*)error->GetBufferPointer();
	MessageBoxA(NULL, str.c_str(), "Output Error Message", MB_OK);
}
