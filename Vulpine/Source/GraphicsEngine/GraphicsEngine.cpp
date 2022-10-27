#include "GraphicsEngine.h"
#include "Vertex.h"
#include <vector>
#include <d3d11.h>
#include <fstream>
#include "Model.h"
#include "ModelAssetHandler.h"
#include "Renderer.h"
#include "FBXImporter.h"
#include "RenderStateManager.h"
#include <filesystem>

LRESULT GraphicsEngine::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	/*static bool dropped;
	if (!dropped)
	{
		DragAcceptFiles(hWnd, true);
		dropped = true;
	}

	if (uMsg == WM_DROPFILES)
	{
		HDROP hDrop = (HDROP)wParam;
		TCHAR filePath[MAX_PATH];
		DragQueryFile(hDrop, 0, filePath, MAX_PATH);
		std::wstring wPath = filePath;
		std::filesystem::path path = std::filesystem::path(wPath.begin(), wPath.end());
		BOOL f = TRUE;
		SwitchToThisWindow(hWnd, f);

		if (path.string().substr(path.string().size() - 4, path.string().size() - 1) == ".dds")
		{
			const int idx = path.string().find_last_of("\\");
			if (std::string::npos != idx)
			{
				std::string fileName(path.string().substr(idx + 1));
				fileName;
			}
		}

	}*/

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
	myDefferedRenderer.Initialize();
	myShadowRenderer.Initialize();

	RenderStateManager::Initialize();

	myClearColor = { 1,1,1,1 };

    return true;
}

void GraphicsEngine::BeginFrame()
{
	myDX11FrameWork.BeginFrame({ myClearColor.x,myClearColor.y,myClearColor.z,1 });
	RenderStateManager::ResetStates();
}

void GraphicsEngine::EndFrame()
{
	myDX11FrameWork.EndFrame();
}

void GraphicsEngine::RenderFrame()
{
	//DX11::Context->ClearRenderTargetView(GBuffer::myVRTVs.Get(), &myClearColor.x);
	Renderer::Render(myForwardRenderer, myDefferedRenderer, myShadowRenderer);
	
	DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), DX11::DepthBuffer.Get());
}
