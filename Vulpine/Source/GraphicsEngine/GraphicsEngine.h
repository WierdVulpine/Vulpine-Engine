#pragma once
#include "DX11.h"
#include "ForwardRenderer.h"
#include "Model.h"
#include "Camera.h"

class ModelAssetHandler;

class GraphicsEngine
{
	SIZE myWindowSize{ 0,0 };
	HWND myWindowHandle{};

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

public:

	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug);

	void BeginFrame();
	void EndFrame();
	void RenderFrame();

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }

private:
	DX11 myDX11FrameWork;
	ForwardRenderer	myForwardRenderer;

	std::shared_ptr<Model> myModel;
	std::shared_ptr<Camera> myCamera;

};

