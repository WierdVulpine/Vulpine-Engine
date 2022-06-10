#include "GraphicsEngine.h"
#include "Vertex.h"
#include <vector>
#include <d3d11.h>
#include <fstream>
#include "Model.h"
#include "ModelAssetHandler.h"

LRESULT GraphicsEngine::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	else if (uMsg == WM_CREATE)
	{
		const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug)
{
	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = GraphicsEngine::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"TGP";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(
		L"TGP",
		L"Vulpine",
		WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
		someX,
		someY,
		someWidth,
		someHeight,
		nullptr, nullptr, nullptr,
		this
	);

	myDX11FrameWork.Initialize(myWindowHandle, false);

	ModelAssetHandler::Initialize();

	myForwardRenderer.Initialize();

	myCamera = std::make_shared<Camera>(Camera());
	myCamera->Init(90, { 1200, 800 }, 0.1f, 15000.f);
	myCamera->SetPosition({ 0,0,-200 });

	myModel = ModelAssetHandler::GetModel(L"Cube");
	myModel->SetPosition({ 0,0,0 });

    return true;
}

void GraphicsEngine::BeginFrame()
{
	myDX11FrameWork.BeginFrame({ 0,0.94f,0.98,1 });
}

void GraphicsEngine::EndFrame()
{
	myDX11FrameWork.EndFrame();
}

void GraphicsEngine::RenderFrame()
{
	myModel->SetScale({myModel->GetTransform().GetScale().x,myModel->GetTransform().GetScale().y, myModel->GetTransform().GetScale().z });
	myForwardRenderer.Render(myCamera, { myModel });
}
