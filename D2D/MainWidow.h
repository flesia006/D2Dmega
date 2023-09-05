#pragma once
#define   MAIN MainWidow::GetInstance()
class MainWidow
{
public:
	static MainWidow* GetInstance()
	{
		static MainWidow singleTone;
		return &singleTone;
	}
public:
	void   Update();
	void   Render();
public:
	void   CreateInstance(HINSTANCE hInstance, int width, int height);
	void   Init3D();             // DirectX Create
	void   CreateBackBuffer();   // Window Size변경시
	void   DeleteBackBuffer();   // Window Size변경시
	void   SetWindowSize(int width, int hight);
	static LRESULT CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:   // getter
	// inline함수 
	int			 GetWidth()         {return m_nWidth;}   
	int			 GetHeight()        { return m_nHeight; }
	HWND		 GetWindowHandler() { return m_hWnd; }
private:
	int			 m_nWidth = 0;
	int			 m_nHeight = 0;
	HWND		 m_hWnd=nullptr;
	HINSTANCE    m_hInstance = nullptr;





public:
	void   Destroy();
private:
	MainWidow();
	~MainWidow();


};

