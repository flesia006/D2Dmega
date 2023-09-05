#pragma once
#define  SRVMANAGER SRVManager::GetInstance()
class SRVManager
{
public:
	static SRVManager* GetInstance()
	{
		static SRVManager singleTone;
		return &singleTone;
	}

public:  // Setter
	ID3D11ShaderResourceView* CreateShaderResourceView(wstring strFile);
	ID3D11ShaderResourceView* FindShaderResourceView(wstring strFile);
	Vector2                   FindSpriteSize(wstring strFileName);

private:
	struct  SpriteResource
	{
		ID3D11ShaderResourceView* m_pSRV;
		Vector2  m_SpriteSize;
	};
	map<wstring, SpriteResource*> m_cmSpriteResouces;


private:
	SRVManager();
public:
	~SRVManager();

};

