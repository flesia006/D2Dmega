#pragma once
class TextureShader
{
public:
	void  Update(Matrix W, Matrix V, Matrix P);
	void  Draw(UINT VertexCount, UINT StartVertexLocation);
	void  DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation);

public: // Setter
	void  SetShaderResourceView(int id, ID3D11ShaderResourceView* pSRV) { m_pSRV[id] = pSRV; }
	void  UpdateColorBuffer(Color color, int path, float Time, float Time2, float Time3);
public: // Getter
	auto  GetLayout() { return m_pInputLayout; }

private:
	void   CreateShader(wstring shaderFile);
	void   CreateInputLayout();
	void   CreateMatrixBuffer();
	void   CreateColorBuffer();
	void   OutputError(ID3D10Blob* error);

	void   CreateSamplerState();
	void   CreateBlendState();
	void   CreateRasterState();


private:
	struct TRANSFORM_DATA    // W,V,P 
	{
		Matrix World;
		Matrix View;
		Matrix Projection;
	};
	struct COLOR_DATA       // Shader에 보내줄 상수
	{
		Color  color;
		int    Path;
		float  Time;
		float  dummy1;
		float  dummy2;
	};
	// Shader & Blob & Layout
	ID3D11VertexShader* m_pVertexShader = nullptr;   // Shader로 작성된 파일을 I/F
	ID3D11PixelShader*	m_pPixelShader = nullptr;     // Shader Pixel
	ID3D10Blob*			m_pVsBlob = nullptr;                 // HLSL->Binary변환
	ID3D10Blob*			m_pPsBlob = nullptr;                 // HLSL->Binary변환
	ID3D11InputLayout*	m_pInputLayout = nullptr;     // 정점 --> Line,Rectangle,Triangle
	// 입력데이터를 어떤식으로 처리?
// CB --> Matrix
	ID3D11Buffer*				m_pMatrixBuffer = nullptr;
	ID3D11Buffer*				m_pColorBuffer = nullptr;

	// Texture
	ID3D11SamplerState*			m_pSampleState = nullptr;    // 확대/축소시 Sampling
	ID3D11RasterizerState*		m_pRasterState = nullptr;
	ID3D11BlendState*			m_pBlendState = nullptr;    // OM (S+D)
	ID3D11BlendState*			m_pBlendStateOff = nullptr;
	ID3D11ShaderResourceView*   m_pSRV[2];              // Multi로 Texture혼합하여 색깔 만듬
public:
	TextureShader(wstring strFileName);
	~TextureShader();

};

