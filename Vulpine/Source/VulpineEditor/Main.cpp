// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "GraphicsEngine.h"
#include "Input.h"
#include "Time.h"
#include "Scene.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#ifdef _DEBUG
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // _DEBUG

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

    std::shared_ptr<GraphicsEngine> graphicsEngine = std::make_shared<GraphicsEngine>();


    const SIZE windowSize = { 1600, 1000 };

    bool shouldRun = graphicsEngine->Initialize(
        (GetSystemMetrics(SM_CXSCREEN) - windowSize.cx) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - windowSize.cy) / 2,
        windowSize.cx,
        windowSize.cy,
        false
    );

    Scene myScene(graphicsEngine);
    myScene.Init();

    Input::SetHandle(graphicsEngine->GetWindowHandle());

#ifdef _DEBUG
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(graphicsEngine->GetWindowHandle());
    ImGui_ImplDX11_Init(DX11::Device.Get(), DX11::Context.Get());
    ImGui::StyleColorsDark();
#endif



    while (shouldRun)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {

#ifdef _DEBUG
            ImGui_ImplWin32_WndProcHandler(graphicsEngine->GetWindowHandle(), msg.message, msg.wParam, msg.lParam);
#endif // _DEBUG

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

        

        // REMEMBER!
        // The frame update for the game does NOT happen inside the PeekMessage loop.
        // This would cause the game to only update if there are messages and also run
        // the update several times per frame (once for each message).

        graphicsEngine->BeginFrame();

#ifdef _DEBUG
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

#endif // _DEBUG

        graphicsEngine->RenderFrame();

        myScene.Update();
        myScene.Render();

#ifdef _DEBUG
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // _DEBUG


        graphicsEngine->EndFrame();

        Input::UpdateInput();
    }

    return 0;
}