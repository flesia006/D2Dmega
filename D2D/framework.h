// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//
#pragma once

#define  _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG          // console을 attach, 조건은 iostream include위에
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
#endif

#pragma warning (disable: 4477) 
#pragma warning (disable: 4467) 
#pragma warning (disable: 4244) 
#pragma warning (disable: 4267) 
#pragma warning (disable: 4018) 




#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <shellapi.h>       // Drag. explore
#include <commdlg.h>        // FileSelect 
#include <assert.h>



// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++
#include  <iostream>
#include  <string>
#include  <vector>
#include  <map>
#include  <bitset>
#include  <functional>    // function을 포인터 변수
#include  <mutex>
using namespace std;

////////////////////////////////////////////////////////////////
// DirectX11
// header, library의 위치를 VisualC++의 속성을 열어서 수정하여야한다
// VC++디렉토리 선택
//    --> 포함디렉토리수정         기존Path;$(DH);
//    --> 라이브디렉토리수정       기존Path;$(DL);
//  * 적용버튼 누름
// DH,DL은 환경변수를 세팅하여야 한다 
// 윈도우의 맨좌하단에서 검색창에   --> 시스템환경변수
//
//  DL
//  C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64
//  DH
//  C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include
//
//  * 시스렘환경변수 Setting이로 컴퓨터를 log on/off
///////////////////////////////////////////////////////


#include  <d3d11.h>          // 11.0
#include  <D3DX11.h>         // 10.0
#include  <D3DX10.h>         // Matrix관련 Util
//#include  <D3DX10math.h>
//#include  <d3dx11effect.h> // 32Bit용  --> 64Bit
#include   <D3Dcompiler.h>   // HLSL,FX 세이더파일 컴파일

#pragma  comment(lib,"d3d11.lib")  // #pragma --> 지시기
#pragma  comment(lib,"d3dx11.lib")
#pragma  comment(lib,"d3dx10.lib")
#pragma  comment(lib,"d3dcompiler.lib")

// Direct 2D font
#include <d2d1_2.h>
#include  <dwrite.h>

#pragma  comment(lib,"d2d1.lib")
#pragma  comment(lib,"dwrite.lib")


// 재정의
typedef  D3DXVECTOR2  Vector2;
typedef  D3DXVECTOR3  Vector3;
typedef  D3DXMATRIX   Matrix;
typedef  D3DXCOLOR    Color;



// MACRO
#define  LEFT  0
#define  RIGHT 1
#define  SAFE_DELETE(p)  {if(p) { delete(p); (p)=NULL;}}
#define  SAFE_RELEASE(p) {if(p) {(p)->Release(); (p)=NULL;}}
#define  SAFE_DELETE_ARRAY(p)  {if(p) { delete [] (p); (p)=NULL;}}
#define  DCST  dynamic_cast

#define DECLARE_SINGLETON(CLASS_NAME)							     \
private:														     \
CLASS_NAME();													     \
~CLASS_NAME();														 \
public:																 \
	CLASS_NAME(const CLASS_NAME& other) = delete;					 \
	CLASS_NAME& operator=(const CLASS_NAME& other) = delete;		 \
	CLASS_NAME(const CLASS_NAME&& other) = delete;					 \
	CLASS_NAME& operator=(const CLASS_NAME&& other) = delete;	     \
public:																 \
static CLASS_NAME* Get()											 \
{																	 \
	static CLASS_NAME instance;										 \
	return &instance;												 \
}

#define SET(POINT)					\
POINT->SetPosition(GetPosition());  \
POINT->SetScale(GetScale());	    \
POINT->SetRotation(GetRotation());  \



// User정의 Class
#include  "System/GameTime.h"
#include  "System/Keyboard.h"			// IMGUI
#include  "System/Mouse.h"				// IMGUI
#include  "Base/Scene.h"
#include  "Base/SceneManager.h"

#include  "Collider/Collider.h"
#include  "Object/GameObject.h"
#include  "Object/ObjectManager.h"
#include  "Object/Component.h"


#include  "MainWidow.h"
#include  "Render/SRVManager.h"
#include  "Render/Texture.h"
#include  "Render/AnimationClip.h"
#include  "Render/Animation.h"
#include  "Render/Camera.h"
#include  "Render/DirectWrite.h"
#include  "Render/LineShader.h"

#include  "Collider/Line.h"
#include  "Collider/Collision.h"

// DiectX의 Global변수 
extern  ID3D11Device*           Device;         // 자원생성(CPU)
extern  ID3D11DeviceContext*    DeviceContext;  // GPU
extern  IDXGISwapChain*         SwapChain;      // B/F Swap
extern  ID3D11RenderTargetView* RTV ;            // RenderTarget View

extern  CMouse* Mouse;
extern  wstring  g_ShaderFile;
