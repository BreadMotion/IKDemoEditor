#include "pch.h"
#include <thread>
#include <mutex>
#include <functional>

#include "FrameWork/Main.h"

#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"

#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Graphics/SnapChain/Win/DirectX11/SwapChainDX11.h"
#include "../Source/Graphics/Surface/Win/DirectX11/RenderTargetSurfaceDX11.h"

#include "FrameWork/Input/InputDevice.h"

//#include "RuntimeObjectSystem.h"
//#include "StdioLogSystem.h"
//#include "SystemTable.h"
//#include "RCCppMainLoop.h"
//#include "RCCppMainLoop.cpp"

//////RCC++ Data
//static IRuntimeObjectSystem* pRuntimeObjectSystem;
//static StdioLogSystem             g_Logger;
//static SystemTable                   g_SystemTable;

//main Data
static ID3D11Device*              pd3dDevice            = nullptr;
static ID3D11DeviceContext*       pd3dDeviceContext     = nullptr;
static IDXGISwapChain*            pSwapChain            = nullptr;
static ID3D11RenderTargetView*    pMainRenderTargetView = nullptr;

//bool RCCppInit();
//void RCCppUpdate();
//void RCCppCleanUp();

namespace Bread
{
	namespace FrameWork
	{
		//****************************************************************************
		// メインループ
		//****************************************************************************
		bool Main::Initialize(uintPtr instance)
		{
			using FND::SharedInstance;
#if 0//(defined(DEBUG) | defined(_DEBUG))
			Bread::s32 width  = 1280;
			Bread::s32 height = 720;
#else
			Bread::s32 width  = 1920;
			Bread::s32 height = 1080;
#endif
			display = Bread::OS::IDisplay::Create();
			if (!display->Initialize(L"BREAD", width, height, instance))
			{
				return false;
			}

			SharedInstance<Graphics::IGraphicsDevice>::instance = Graphics::IGraphicsDevice::Create();
			if (!SharedInstance<Graphics::IGraphicsDevice>::instance->Initialize(display.get()))
			{
				return false;
			}

			Bread::Graphics::Viewport v;
			v.x = 0.0f;
			v.y = 0.0f;
			v.width        = static_cast<Bread::f32>(width);
			v.height       = static_cast<Bread::f32>(height);
			v.minDepth = 0.0f;
			v.maxDepth = 1.0f;
			SharedInstance<Graphics::IGraphicsDevice>::instance->GetContext()->SetViewports(1, &v);

			ID3D11Device*        d3dDevice                   = static_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::IGraphicsDevice>::instance->GetDevice())->GetD3DDevice();
			ID3D11DeviceContext* d3dDeviceContext = static_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::IGraphicsDevice>::instance->GetDevice())->GetD3DContext();

			//test
			//RCCppInit();

			ImGui::SetCurrentContext(ImGui::CreateContext());
			ImGui_ImplWin32_Init(display->GetHWND());
			ImGui_ImplDX11_Init(d3dDevice, d3dDeviceContext);

			ImGui::StyleColorsLight();

			ImGui::GetStyle().WindowRounding = 1.0f;
			ImGui::GetStyle().FrameRounding  = 1.0f;
			ImGui::GetStyle().Colors[ImGuiCol_Text]          = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
			ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]     = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg]      = ImVec4(0.25f, 0.25f, 0.25f, 0.85f);
			ImGui::GetStyle().Colors[ImGuiCol_PopupBg]       = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			ImGui::GetStyle().Colors[ImGuiCol_FrameBg]       = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

			ImGui::GetStyle().Colors[ImGuiCol_TitleBg]       = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
			ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);

			ImGui::GetStyle().Colors[ImGuiCol_Tab]           = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
			ImGui::GetStyle().Colors[ImGuiCol_TabActive]     = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);


			ImGuiIO& io = ImGui::GetIO();
//#if	!defined(PHOENIX_TARGET_DEBUG)
//			io.IniFilename = NULL;
//#endif
			io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\BIZ-UDGothicR.ttc", 15.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
			//g_SystemTable.pImContext                  = ImGui::GetCurrentContext();
			//g_SystemTable.pd3dDevice                  = static_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::IGraphicsDevice>::instance->GetDevice())->GetD3DDevice();
			//g_SystemTable.pd3dDeviceContext     = static_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::IGraphicsDevice>::instance->GetDevice())->GetD3DContext();
			//g_SystemTable.pSwapChain                 = static_cast<Graphics::SwapChainDX11*>(SharedInstance<Graphics::IGraphicsDevice>::instance->GetSwapChain())->GetDXGISwapChain();
			//g_SystemTable.pMainRenderTargetView = static_cast<Graphics::RenderTargetSurfaceDX11*>(static_cast<Graphics::SwapChainDX11*>(SharedInstance<Graphics::IGraphicsDevice>::instance->GetSwapChain())->GetRenderTargerSurface())->GetD3DRenderTargetView();
			//
			//auto fn = [&](f32 elapsedTime) { return Update(elapsedTime); };
			//g_SystemTable.pRCCppMainLoopI = new RCCppMainLoop;

			elapsedTime = 0.0f;

			return true;
		}

		void Main::Finalize()
		{
			using FND::SharedInstance;

			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			//test
			//RCCppCleanUp();

			SharedInstance<Graphics::IGraphicsDevice>::instance->Finalize();
			display->Finalize();
		}

		void Main::Run()
		{
			using FND::SharedInstance;
#if 1
			//test
			//RCCppUpdate();

			// 更新
			{
				display->TimerTick();
				display->CalculateFrameStats();
				display->InputUpdate();
				elapsedTime = display->TimerInterval() * 60.0f;

				GetXInputState(&xInput[0], 0, elapsedTime);
				GetDInputState(&dInput[0], 0, elapsedTime);

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();

				{//imgui set up
					ImGuiIO& io = ImGui::GetIO();
					static INT64							time              = 0;
					static INT64							ticksPerSecond    = 0;
					static ImGuiMouseCursor	lastMouseCursor = ImGuiMouseCursor_COUNT;
					// Setup display size (every frame to accommodate for window resizing)
					RECT rect;
					::GetClientRect(display->GetHWND(), &rect);
					io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

					// Setup time step
					INT64 currentTime;
					::QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
					io.DeltaTime = (float)(currentTime - time) / ticksPerSecond;
					time = currentTime;

					// Read keyboard modifiers inputs
					io.KeyCtrl  = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
					io.KeyShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
					io.KeyAlt   = (::GetKeyState(VK_MENU) & 0x8000) != 0;
					io.KeySuper = false;

					if (io.WantSetMousePos)
					{
						POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
						::ClientToScreen(display->GetHWND(), &pos);
						::SetCursorPos(pos.x, pos.y);
					}

					// Set mouse position
					io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
					POINT pos;
					if (HWND active_window = ::GetForegroundWindow())
						if (active_window == display->GetHWND() || ::IsChild(active_window, display->GetHWND()))
							if (::GetCursorPos(&pos) && ::ScreenToClient(display->GetHWND(), &pos))
								io.MousePos = ImVec2((float)pos.x, (float)pos.y);

					// Update OS mouse cursor with the cursor requested by imgui
					ImGuiMouseCursor mouseCursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
					if (lastMouseCursor != mouseCursor)
					{
						lastMouseCursor = mouseCursor;
						if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {}

						ImGuiMouseCursor imguiCursor = ImGui::GetMouseCursor();
						if (imguiCursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
						{
							// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
							::SetCursor(NULL);
						}
						else
						{
							// Show OS mouse cursor
							LPTSTR win32Cursor = IDC_ARROW;
							switch (imguiCursor)
							{
							case ImGuiMouseCursor_Arrow:        win32Cursor = IDC_ARROW; break;
							case ImGuiMouseCursor_TextInput:    win32Cursor = IDC_IBEAM; break;
							case ImGuiMouseCursor_ResizeAll:    win32Cursor = IDC_SIZEALL; break;
							case ImGuiMouseCursor_ResizeEW:     win32Cursor = IDC_SIZEWE; break;
							case ImGuiMouseCursor_ResizeNS:     win32Cursor = IDC_SIZENS; break;
							case ImGuiMouseCursor_ResizeNESW:   win32Cursor = IDC_SIZENESW; break;
							case ImGuiMouseCursor_ResizeNWSE:   win32Cursor = IDC_SIZENWSE; break;
							case ImGuiMouseCursor_Hand:         win32Cursor = IDC_HAND; break;
							//case ImGuiMouseCursor_NotAllowed:   win32Cursor = IDC_NO; break;
							}
							::SetCursor(::LoadCursor(NULL, win32Cursor));
						}
					}
				}

				ImGui::NewFrame();
				//ImGuizmo::BeginFrame();

				//test
				//auto fn = [&](f32 elapsedTime) { return Update(elapsedTime); };
				//g_SystemTable.pRCCppMainLoopI->MainLoop(fn, elapsedTime);

				Update(elapsedTime);
			}

			// 描画
			{
				SharedInstance<Graphics::IGraphicsDevice>::instance->RenderBegin();
				Render(elapsedTime);
				SharedInstance<Graphics::IGraphicsDevice>::instance->RenderEnd();


				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				SharedInstance<Graphics::IGraphicsDevice>::instance->Present(0);
			}
#else
			display->TimerTick();
			display->CalculateFrameStats();
			elapsedTime = display->TimerInterval() * 60.0f;
			const float theFrameElapseTime = elapsedTime;

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			std::mutex mMutex;
			std::thread updateThread([&]() {
				// 更新
				{
					mMutex.lock();
					GetXInputState(&xInput[0], 0, theFrameElapseTime);
					GetDInputState(&dInput[0], 0, theFrameElapseTime);
					//ImGuizmo::BeginFrame();

					SharedInstance<Graphics::IGraphicsDevice>::instance->RenderCommandRun();
					SharedInstance<Graphics::IGraphicsDevice>::instance->Present(0);

					mMutex.unlock();

					Update(theFrameElapseTime);
				}
				});

			std::thread  drawThread([&]() {
				// 描画
				{
					SharedInstance<Graphics::IGraphicsDevice>::instance->RenderBegin();
					Render(theFrameElapseTime);
					SharedInstance<Graphics::IGraphicsDevice>::instance->RenderEnd();
					SharedInstance<Graphics::IGraphicsDevice>::instance->RecordRenderCommand();
				}
				});
			updateThread.join(); drawThread.join();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // 0
		}
	}
}
//
//bool RCCppInit()
//{
//	pRuntimeObjectSystem = new RuntimeObjectSystem;
//	if (!pRuntimeObjectSystem->Initialise(&g_Logger, &g_SystemTable))
//	{
//		delete pRuntimeObjectSystem;
//		pRuntimeObjectSystem = nullptr;
//		return false;
//	}
//
//	FileSystemUtils::Path basePath        = pRuntimeObjectSystem->FindFile(__FILE__).ParentPath();
//	FileSystemUtils::Path imguiIncludeDir = basePath / "imgui";
//	pRuntimeObjectSystem->AddIncludeDir(imguiIncludeDir.c_str());
//	return true;
//}
//
//void RCCppUpdate()
//{
//	if (pRuntimeObjectSystem->GetIsCompiledComplete())
//	{
//		//モジュールを保存する
//
//
//		//コンパイルが完了後モジュールをロード
//		pRuntimeObjectSystem->LoadCompiledModule();
//
//		//読み込み
//
//	}
//
//	if (!pRuntimeObjectSystem->GetIsCompiling())
//	{
//		float deltaTime = 1.0f / ImGui::GetIO().Framerate;
//		pRuntimeObjectSystem->GetFileChangeNotifier()->Update(deltaTime);
//	}
//}
//
//void RCCppCleanUp()
//{
//	delete pRuntimeObjectSystem;
//}