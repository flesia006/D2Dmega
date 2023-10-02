#include "framework.h"
#include "MainWidow.h"
#include  "Resource.h"

// getter
// DiectX의 Global변수 
// I로 시작되는것은 com interface로 만들어진것   --> 이중포인터형식
ID3D11Device* Device = NULL;         // 자원생성(CPU)
ID3D11DeviceContext* DeviceContext = NULL;  // GPU
IDXGISwapChain* SwapChain = NULL;      // B/F Swap
ID3D11RenderTargetView* RTV = NULL;            // RenderTarget View

MainWidow::MainWidow()
{
}

MainWidow::~MainWidow()
{
	Destroy();
}
void MainWidow::Destroy()
{
	SAFE_RELEASE(Device);
	SAFE_RELEASE(DeviceContext);
	SAFE_RELEASE(SwapChain);
	SAFE_RELEASE(RTV);
}
///////////////////////////////////////////////
//
void MainWidow::Update()
{
	SCENEMANAGER->Update();
}

void MainWidow::Render()
{

	D3D11_VIEWPORT  viewport;
	{
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)MAIN->GetWidth();
		viewport.Height = (float)MAIN->GetHeight();
	}
	// 다이렉트X 파이프라인
	// IA(input Assembly ) --> 정점을 생성(C++)
	// VS(Vertex Stage)    --> HLSL
	// RS(Raster Stage)    --> HLSL
	// PS(Pixel Stage)     --> HLSL
	// OM(Output Merger)
	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);
	DeviceContext->RSSetViewports(1, &viewport);

	// 렌더링은  여기서 부터
	Color bgColor = Color(0.2f, 0.2f, 0.2f, 1.0f);  // R,G,B,A
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);

	// ---------  Scene Manger Rendering
	SCENEMANAGER->Render();
	//ImGui::Render();
	SwapChain->Present(0, 0);    // Front  -> Back -> Front

}

/////////////////////////////////////////////////////////////////
// 레즈스트리에 등록
// 정책 --> 1. Window Class등록
//             * Key,Mouse, window가 갱신 --> Message에 처리
//             * Window Class등록시 노티
//          2. Window 생성 
/////////////////////////////////////////////////////////////////
void MainWidow::CreateInstance(HINSTANCE hInstance, int width, int height)
{
    // window class등록
    {
        WNDCLASSEXW wcex;    //Window Class Extention Wide, Win95---> 11
        wcex.cbSize = sizeof(WNDCLASSEX);  // 현재 윈도우의 Class구조체의 Size
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;   //* 함수의 포인터 , OS가 Message를 현 프로그램
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;  //*
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2D));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    //    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_D2D);
		wcex.lpszMenuName = NULL;
        wcex.lpszClassName = L"D2D";
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
        RegisterClassExW(&wcex);  // HINSTANCE, CALLBACK함수
    }

    {
        m_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

        m_hWnd = CreateWindowW(L"D2D",       // 등록된 Class명
            L"D2D",             // 
            WS_OVERLAPPEDWINDOW,
		//	WS_POPUP,
            0,       // x position
            0,                   // y position
            width,
            height,
            nullptr,
            nullptr,
            hInstance,
            nullptr);

		RECT rect;
		GetClientRect(m_hWnd, &rect);

        m_nWidth  = rect.right - rect.left;
        m_nHeight = rect.bottom - rect.top;

//		UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - m_nWidth) / 2;
//		UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - m_nHeight) / 2;
//
//		MoveWindow
//		(
//			m_hWnd
//			, centerX, centerY
//			, m_nWidth, m_nHeight
//			, true
//		);
	
		// Direct X11초기화
		this->Init3D();
	

        ShowWindow(m_hWnd, SW_SHOWDEFAULT);
        UpdateWindow(m_hWnd);
    }


}
/////////////////////////////////////////////////////////
//  1. Device         : CPU --> 자원을 생성하는 역활
//           HDC이전에 수정을 하고 DC Call
//          *HPen MyPen, OldPen;                        ---|
//          *MyPen = CreatePen(PS_SOLID,1,RGB(255,0,0)) ---|
//          MoveTo(hDC,10,10,NULL)
//          LibeTo(hDC,50,50);
//   2. DeviceContext : GPU ( 렌더링 파이프라인)
//   3. SwapChain     : 깜박거리는 현상을 없애주기위해 
//                      BackBuffer를 만들어진다
/////////////////////////////////////////////////////////
void MainWidow::Init3D()
{
	//1. Device, DeviceContext,Swap Chain을 한번에 만들어 준다
		//   호출전에 Graphic카드를 Check 프로그래밍
		//   COM Interface생서할때는 Desc를 물어본다

	DXGI_MODE_DESC  BufferDesc;
	{
		// desc는 반드시 Zeromemory()
		ZeroMemory(&BufferDesc, sizeof(DXGI_MODE_DESC));
		BufferDesc.Width = MAIN->GetWidth();
		BufferDesc.Height = MAIN->GetHeight();
		BufferDesc.RefreshRate.Numerator = 60;   // 분자값 60
		BufferDesc.RefreshRate.Denominator = 1;   // 분모값 1 (1초 60번 재표시)
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 255  --> 1.0f
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	}

	DXGI_SWAP_CHAIN_DESC  swapDesc;
	{
		// descprtion은 반드시 ZeroMememoy를 이용하여 초기화 한다
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapDesc.BufferDesc = BufferDesc;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;  // 2D, 3D: Depth,Stencil
		swapDesc.SampleDesc.Count = 1; // 기본형
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.OutputWindow = MAIN->GetWindowHandler();
		swapDesc.Windowed = TRUE;  // true
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // swap 이후에 데이터를 버린다
	}


	vector<D3D_FEATURE_LEVEL> feature_level =  // 11.3,11,2,10.3......
	{
			D3D_FEATURE_LEVEL_11_1,  //11.0
			D3D_FEATURE_LEVEL_11_0,  // 5.0
			D3D_FEATURE_LEVEL_10_1,  // ps4_0,vs_4
			D3D_FEATURE_LEVEL_10_0
			//	D3D_FEATURE_LEVEL_9_3,
	};

	D3D_FEATURE_LEVEL  seleted_feature_level;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,    // 그래픽카드,
		D3D_DRIVER_TYPE_HARDWARE,  // H/W,S/W
		NULL,    // S/W Module ( SGL,Intel같은 회사가 좀더 최적화 ..)
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, // Buffer Support, 2D+3D(글자)
		feature_level.data(),  // 지원가능한 우선순위
		feature_level.size(),  // 위에 선언된 데이터 크기
		D3D11_SDK_VERSION,     // 9,10,11,12
		&swapDesc,             // swap chain에 대한 desc
		&SwapChain,            // (**) 
		&Device,
		&seleted_feature_level, // 지원가능한 우선순위에서 선택된 feature
		&DeviceContext);

	// 만약 error가 어느 라인에서 Error가 니는지 check

	assert(SUCCEEDED(hr));

	// 2. Rendering Target View
	//    swapChain으로 부터 backbuffer를 RTV를 생서
	CreateBackBuffer();
}

void MainWidow::CreateBackBuffer()
{
	// Get Backbuffer
	ID3D11Texture2D* BackBuffer;
	// com interface --> typelib ( Automation기법  --> Excel 
	HRESULT hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(hr));

	// Create RenderTargetView
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(BackBuffer);

	// RenderTargetView -> OMSet  PipeLine Stage : IA->VS->PS->RS->OM 
	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);

	// Create ViewPort
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)m_nWidth;
		viewport.Height = (float)m_nHeight;

		DeviceContext->RSSetViewports(1, &viewport);
	}
}

void MainWidow::DeleteBackBuffer()
{
	// cominterface는  release로 지워야 한다
	SAFE_RELEASE(RTV);
}
void MainWidow::SetWindowSize(int width, int height)
{
	RECT rect = { 0,0,(LONG)width,(LONG)height };
	UINT centerX = GetSystemMetrics(SM_CXSCREEN) - width / 2;
	UINT centerY = GetSystemMetrics(SM_CYSCREEN) - height / 2;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow(m_hWnd, 10, 10, rect.right - rect.left,
		rect.bottom - rect.top, TRUE);

}

LRESULT MainWidow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (Mouse)
		Mouse->WndProc(message, wParam, lParam);
    switch (message)
    {
    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
