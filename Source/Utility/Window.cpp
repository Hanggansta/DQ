#include <Utility/Window.h>

namespace DQ
{
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (message == WM_DESTROY)
        {
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }

    int WindowsMain(HINSTANCE hInstance, IApp* pApp)
    {
        WNDCLASSEXA wcex{};
        wcex.cbSize = sizeof(WNDCLASSEXA);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIconA(wcex.hInstance, "IDI_ICON");
        wcex.hCursor = LoadCursorA(nullptr, IDC_ARROW);
        wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcex.lpszClassName = "DQ";
        wcex.hIconSm = LoadIconA(wcex.hInstance, "IDI_ICON");

        RegisterClassExA(&wcex);

        auto stype = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
        RECT rc = { 0, 0, static_cast<LONG>(pApp->mSettings.mWidth), static_cast<LONG>(pApp->mSettings.mHeight) };
        AdjustWindowRect(&rc, stype, FALSE);
        auto hwnd = CreateWindowExA(0, "DQ", pApp->GetName(), stype, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, wcex.hInstance, nullptr);
        ShowWindow(hwnd, SW_SHOWDEFAULT);

        pApp->mHwnd = hwnd;
        pApp->Init();

        LARGE_INTEGER frequency;
        LARGE_INTEGER nowTime, lastTime;

        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&lastTime);

        MSG msg = {};
        while (WM_QUIT != msg.message)
        {
            if (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            else
            {
                QueryPerformanceCounter(&nowTime);
                float deltaTimeFloat = (nowTime.QuadPart - lastTime.QuadPart) / (float)frequency.QuadPart;
                lastTime = nowTime;
                pApp->Update(deltaTimeFloat);
                pApp->Draw();
            }
        }

        pApp->Exit();

        return 0;
    }
}