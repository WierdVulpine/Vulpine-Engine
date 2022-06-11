// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "GraphicsEngine.h"
#include "Input.h"
#include "Time.h"
#include "Scene.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    MSG msg = { 0 };

    GraphicsEngine graphicsEngine;


    const SIZE windowSize = { 1200, 800 };

    bool shouldRun = graphicsEngine.Initialize(
        (GetSystemMetrics(SM_CXSCREEN) - windowSize.cx) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - windowSize.cy) / 2,
        windowSize.cx,
        windowSize.cy,
        false
    );

    Scene myScene;
    myScene.Init();

    Input::SetHandle(graphicsEngine.GetWindowHandle());

    while (shouldRun)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            Input::UpdateEvents(msg.message, msg.wParam, msg.lParam);

            // Should update input here.

            if (msg.message == WM_QUIT)
            {
                return 0;
            }
        }

        Time::Update();

        myScene.Update();
        myScene.Render();

        // REMEMBER!
        // The frame update for the game does NOT happen inside the PeekMessage loop.
        // This would cause the game to only update if there are messages and also run
        // the update several times per frame (once for each message).

        graphicsEngine.BeginFrame();

        graphicsEngine.RenderFrame();

        graphicsEngine.EndFrame();

        Input::UpdateInput();
    }

    return 0;
}