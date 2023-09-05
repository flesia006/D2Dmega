#include "framework.h"
#include "SRVManager.h"
#include <io.h>

SRVManager::SRVManager()
{
}

SRVManager::~SRVManager()
{
	for (auto v : m_cmSpriteResouces)
	{
		SAFE_RELEASE(v.second->m_pSRV);
		delete v.second;
	}
	m_cmSpriteResouces.clear();
}

ID3D11ShaderResourceView* SRVManager::CreateShaderResourceView(wstring strFile)
{
	string str;

	// wstring --> string변환 : V22 Bug?
	str.assign(strFile.begin(), strFile.end());

	if (_access(str.c_str(), 00) != 0)  // File의 read only일때 check안된다(Bug?)
	{
		str = "SRVManager::CreateShaderResourceView: File not found  " + str;
		MessageBoxA(MAIN->GetWindowHandler(),
			str.c_str(), "이미지 파일 에러", MB_OK);
		return nullptr;
	}

	// 1. map에 존재 하는가 Check
	if (m_cmSpriteResouces.find(strFile) != m_cmSpriteResouces.end())
		return m_cmSpriteResouces.find(strFile)->second->m_pSRV;
	// 2. map에 없는 경우
	SpriteResource* pSpriteResource = new SpriteResource();

	auto hr = D3DX11CreateShaderResourceViewFromFile(Device,
		strFile.c_str(), NULL, NULL,
		&pSpriteResource->m_pSRV, NULL);
	assert(SUCCEEDED(hr));

	// 3. ImageFile width,height를 얻음
	D3DX11_IMAGE_INFO ImageInfo;
	hr = D3DX11GetImageInfoFromFile(strFile.c_str(), NULL, &ImageInfo, NULL);
	assert(SUCCEEDED(hr));

	pSpriteResource->m_SpriteSize = Vector2((float)ImageInfo.Width,
		(float)ImageInfo.Height);

	m_cmSpriteResouces.insert(make_pair(strFile, pSpriteResource));

	return pSpriteResource->m_pSRV;

}

ID3D11ShaderResourceView* SRVManager::FindShaderResourceView(wstring strFile)
{
	if (m_cmSpriteResouces.find(strFile) != m_cmSpriteResouces.end())
		return m_cmSpriteResouces.find(strFile)->second->m_pSRV;
	return nullptr;
}

Vector2 SRVManager::FindSpriteSize(wstring strFileName)
{
	if (m_cmSpriteResouces.find(strFileName) != m_cmSpriteResouces.end())
		return m_cmSpriteResouces.find(strFileName)->second->m_SpriteSize;
	return Vector2(0.0f,0.0f);
}


