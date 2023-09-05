#include  "framework.h"
#include "TextureShader.h"
#include  <io.h>

TextureShader::TextureShader(wstring strFileName)
{
	wstring  File = strFileName;
	string   str;

	m_pSRV[0] = nullptr;
	m_pSRV[1] = nullptr;

	// File이 있는가 Check
	str.assign(File.begin(), File.end()); // 2022Version Check
	if (_access(str.c_str(), 00) == 0)
	{
		CreateShader(File);
		CreateInputLayout();
		CreateMatrixBuffer();
		CreateColorBuffer();
		CreateSamplerState();
		CreateBlendState();
		CreateRasterState();
	}
	else
	{
		str = "File nou found " + str;
		MessageBoxA(NULL, str.c_str(), "FILE ERROR", MB_OK);
	}
}

TextureShader::~TextureShader()
{
	SAFE_RELEASE(m_pVertexShader);   // Shader로 작성된 파일을 I/F
	SAFE_RELEASE(m_pPixelShader);    // Shader Pixel
	SAFE_RELEASE(m_pVsBlob);         // HLSL->Binary변환
	SAFE_RELEASE(m_pPsBlob);         // HLSL->Binary변환
	SAFE_RELEASE(m_pInputLayout);    // 정점 --> Line,Rectangle,Triangle
	SAFE_RELEASE(m_pMatrixBuffer);
	SAFE_RELEASE(m_pColorBuffer);
	SAFE_RELEASE(m_pSampleState);    // 확대/축소시 Sampling
	SAFE_RELEASE(m_pRasterState);
	SAFE_RELEASE(m_pBlendState);    // OM (S+D)
	SAFE_RELEASE(m_pBlendStateOff);
}

void TextureShader::Update(Matrix W, Matrix V, Matrix P)
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

void TextureShader::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	// VS단계
	DeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

	// RS단계
	DeviceContext->RSSetState(this->m_pRasterState);    // 추가

	// PS단계
	DeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	DeviceContext->PSSetSamplers(0, 1, &m_pSampleState); // 추가

	if (m_pSRV[1])
		DeviceContext->PSSetShaderResources(0, 2, m_pSRV); // 추가
	else
		DeviceContext->PSSetShaderResources(0, 1, m_pSRV); // 추가

	DeviceContext->PSSetConstantBuffers(1, 1, &m_pColorBuffer); // 추가

	// OM단계
	DeviceContext->OMSetBlendState(m_pBlendState, nullptr, 0xffffffff);

	// PIPE LINE 작동
	DeviceContext->Draw(VertexCount, StartVertexLocation);

}

void TextureShader::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation)
{
}

void TextureShader::UpdateColorBuffer(Color color, int path, float Time, float Time2, float Time3)
{
	COLOR_DATA  temp;
	temp.color = color;
	temp.Path = path;
	temp.Time = Time;
	temp.dummy1 = Time2;
	temp.dummy2 = Time3;

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
////////////////////////////////////////////////////
// VS,PS에 관련 쉐이더 생성
// BLOB
////////////////////////////////////////////////////
void TextureShader::CreateShader(wstring shaderFile)
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

void TextureShader::CreateInputLayout()
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

	layout[1].SemanticName = "TEXCOORD";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;  // Vector2 float x, y
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = 12;   // x,y,z,(w : gpu에 붙인다)
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	// 2개이상일 경우는 변경?

	hr = Device->CreateInputLayout(layout, 2, m_pVsBlob->GetBufferPointer(),
		m_pVsBlob->GetBufferSize(),
		&this->m_pInputLayout);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateMatrixBuffer()
{
	// 위에 선언된 데이터를 VertextBuffer에 Setting
// DX에서 Craete,Get()를 사용할때 변수명칭 앞에 "I"자가 있으면
// 무조건 Desctrpion이 붙는다
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

void TextureShader::CreateColorBuffer()
{
	// 위에 선언된 데이터를 VertextBuffer에 Setting
// DX에서 Craete,Get()를 사용할때 변수명칭 앞에 "I"자가 있으면
// 무조건 Desctrpion이 붙는다
	D3D11_BUFFER_DESC         desc;    // input
	{
		ZeroMemory(&desc, sizeof(desc));              // 변수가 0로 초기화
		desc.Usage = D3D11_USAGE_DYNAMIC;             // *생성한후에 변경할수  한다
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE; // *생성한후에 변경할수  한다
		desc.ByteWidth = sizeof(COLOR_DATA);              // Vector3,Color ( 3*4 + 4*4)
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

void TextureShader::OutputError(ID3D10Blob* error)
{
	string str = (char*)error->GetBufferPointer();
	MessageBoxA(NULL, str.c_str(), "Output  Error Message", MB_OK);
}

void TextureShader::CreateSamplerState()
{
	D3D11_SAMPLER_DESC  desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

		// 정규화된 좌표(NCD)를 넘었을때 어떻게 할지
		// WRAP   : 원본픽세로 나머지 부분을 채운다
		// CLAMP  : 맨마지막 픽셀로 채운다
		// MIRROR : 인접한 픽셀로 반전시켜 채운다
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		desc.BorderColor[0] = 1; //외곽선
		desc.BorderColor[1] = 0; //외곽선
		desc.BorderColor[2] = 0; //외곽선
		desc.BorderColor[3] = 1; //외곽선

		// 이전 / 현재 데이터를 비교하는 플래그
		// 기존 SAMPLE과 4개의 값
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		// MIN --> 이미지 축소
		// MAG --> 이미지 확대
		// 이미지를 확대 하거나 축소할때 미리 1/4크기로 이미지를 구성
		// POINT  : 2개의 PIXEL이 있다면 한나를 폐기
		// LINEAR : 2개의 PIXEL의 평균값

		desc.MaxAnisotropy = 16;  // 비등방성 필터링
		desc.MaxLOD = FLT_MAX;
		desc.MinLOD = FLT_MIN;
		desc.MipLODBias = 0.0f;
	}

	auto hr = Device->CreateSamplerState(&desc, &this->m_pSampleState);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateBlendState()
{
	D3D11_BLEND_DESC  desc;
	{
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;  // 뒤에 테두리 제거
		desc.IndependentBlendEnable = true;  // 알파혼합
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;     // 0.9
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 0.9 --> 0.1
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	}


	auto hr = Device->CreateBlendState(&desc, &this->m_pBlendState);
	assert(SUCCEEDED(hr));
}

void TextureShader::CreateRasterState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	desc.FillMode = D3D11_FILL_SOLID;  // 내부를 칠하는 것
	desc.CullMode = D3D11_CULL_NONE;   // 후면 표시도 될수 있게
	desc.FrontCounterClockwise = false;

	auto hr = Device->CreateRasterizerState(&desc, &this->m_pRasterState);
	assert(SUCCEEDED(hr));
}
