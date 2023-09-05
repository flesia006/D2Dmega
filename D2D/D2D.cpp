// D2D.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "D2D.h"

CMouse*  Mouse = nullptr;
wstring  g_ShaderFile = L"./Shader/Texture.hlsl";

//////////////////////////////////////////////////
// Window의 Main함수
// int main(int argc,char **argv)
//////////////////////////////////////////////////
int APIENTRY wWinMain(HINSTANCE hInstance,      // OS에서 생성된 Instance Handle
                      HINSTANCE hPrevInstance,  // Kill되었을때  예전 핸들
                      LPWSTR    lpCmdLine,      // 명령어줄
                      int       nCmdShow)       // Window를 어떻게 보이게 할 것인가?
{
  
    // Game에 대한 root class
   MAIN->CreateInstance(hInstance, 1280, 960);
 //   MAIN->CreateInstance(hInstance, 500, 500);
    // Scene Manager 생성

   SCENEMANAGER->Init();

    // Mouse Create
    Mouse = new CMouse(MAIN->GetWindowHandler());
    // DirecrX 2D Create
    DirectWrite::Create();



    MSG msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //Timer
            TIME->Update(120.0f);       // 120Frame;
            Mouse->Update();
            MAIN->Update(); //  충돌, 키보드/마우스에 대한 위치변경
            MAIN->Render(); //  Rendering
        }

    }
    DirectWrite::Delete();
    MAIN->Destroy();
    return (int) msg.wParam;
}