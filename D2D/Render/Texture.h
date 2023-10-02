#pragma once
class Texture
{
public:
	Texture(wstring strImage, wstring strShader, UINT Mode = 0);    // img를 full : bg
	Texture(wstring strImage, wstring strShader, int startX, int startY, int endX, int endY);
	~Texture();


public:
	void Update();
	void Render();
	void UpdateBlock();    // UV값을 변경
public:   // Setter
	void  SetScale(float x, float y) { m_Scale = Vector2(x, y); }
	void  SetScale(Vector2 scale) { m_Scale = scale; }
	void  SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void  SetPosition(Vector2 position) { m_Position = position; }
	void  SetRotation(float x, float y, float z) { m_Rotation = Vector3(x, y, z); }
	void  SetRotation(Vector3 rot) { m_Rotation = rot; }
	void  SetOffset(float x, float y) { m_SpriteOffset = Vector2(x, y); }
	void  SetOffset(Vector2 offset) { m_SpriteOffset = offset; }
	void  SetOffsetSize(float x, float y) { m_SpriteSize = Vector2(x, y); }
	void  SetOffsetSize(Vector2 size) { m_SpriteSize = size; }
	void  SetImageFile(wstring strFile) { m_strImageFile = strFile; }
	void  SetImageSize(float x, float y) { m_ImageSize = Vector2(x, y); }
	void  SetMultiMultiShaderResourceView(wstring strFile);
	void  CreateShaderResourceView(wstring strFile);
	void  CreateMultiShaderResourceView(wstring strFile);
	void  UpdateColorBuffer(int Path, Color color = Color(1, 1, 1, 1), float Time = 0.0f, float dummy1 = 0.0f, float dummy2 = 0.0f);



public:   // Getter
	Vector2  GetPosition() { return m_Position; }
	Vector2  GetScale() { return m_Scale; }
	Vector3  GetRotation() { return m_Rotation; }
	Vector2  GetTextureRealSize();
	Vector2  GetOffset() { return m_SpriteOffset; }
	Vector2  GetOffsetSize() { return m_SpriteSize; }
	wstring  GetImageFile() { return m_strImageFile; }


public:
	Vector2 m_Position = Vector2(0.0f, 0.0f);
	Vector2 m_Scale = Vector2(1.0f, 1.0f);  // 0으로 하면 안됨
	Vector3 m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	Vector2 m_SpriteOffset = Vector2(0.0f, 0.0f);  // Render start
	Vector2 m_SpriteSize = Vector2(0.0f, 0.0f);    // Render end
	Vector2 m_ImageSize = Vector2(0.0f, 0.0f);    // Full Image Size
	class   TextureShader* m_pShader = nullptr;
	wstring m_strImageFile;   // 기능 : 1) 1,2,3장 2) 1장


private:
	void   CreateVertexBuffer(float startX, float startY, float endX, float endY);
private:
	struct Vertex
	{
		Vector3 Position;
		Vector2 Uv;
	};
	ID3D11Buffer* m_pVertexBuffer = nullptr;        // VertexBuffer
	ID3D11ShaderResourceView* m_ptrSRV = nullptr;  // Img파일의 주소
	UINT TextureMode = 0;


};

