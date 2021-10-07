#include "pch.h"

#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <sstream>

#include "DisplayWin.h"
#include "FrameWork/Input/InputDevice.h"
//#include "OS/InputSystem.h"


LRESULT CALLBACK FnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Bread::OS::DisplayWin* d = reinterpret_cast<Bread::OS::DisplayWin*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	return d ? d->HandleMessage(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}

namespace Bread
{
	namespace OS
	{
		// TODO : move to file
		// mbstowcs
		int MbsToWcs(wchar_t* wcstr, int sizeInWords, const char* mbstr, int count)
		{
			size_t ret;
			mbstowcs_s(&ret, wcstr, sizeInWords, mbstr, count);
			return static_cast<int>(ret);
		}

		std::unique_ptr< DisplayWin > IDisplay::Create()
		{
			return std::make_unique<DisplayWin>();
		}

		bool DisplayWin::Initialize(const wchar_t* name, Bread::s32 width, Bread::s32 height, Bread::uintPtr instance)
		{
			//MbsToWcs(this->name, sizeof(this->name), name, static_cast<int>(strlen(name)));
			wcscpy_s(this->name, name);
			hInstaice = reinterpret_cast<HINSTANCE>(instance);

			// サイズ調整
			RECT rect;
			{
				int x = 0;
				int y = 0;
				this->width = width;
				this->height = height;

				SetRect(&rect, x, y, width, height);
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			}

			// ウィンドウ作成
			{
				WNDCLASSEX wcex;
				wcex.cbSize                = sizeof(WNDCLASSEX);
				wcex.style                   = CS_HREDRAW | CS_VREDRAW;
				wcex.lpfnWndProc    = FnWndProc;
				wcex.cbClsExtra          = 0;
				wcex.cbWndExtra       = 0;
				wcex.hInstance           = hInstaice;
				wcex.hIcon                  = LoadIcon(hInstaice, IDI_APPLICATION);
				wcex.hCursor              = LoadCursor(NULL, IDC_ARROW);
				wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
				wcex.lpszMenuName  = NULL;
				wcex.lpszClassName   = this->name;
				wcex.hIconSm              = LoadIcon(hInstaice, IDI_APPLICATION);
				RegisterClassEx(&wcex);

				hwnd = CreateWindow(this->name, this->name,
					WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE | WS_EX_TRANSPARENT | WS_BORDER,
					CW_USEDEFAULT, CW_USEDEFAULT,
					rect.right - rect.left, rect.bottom - rect.top,
					NULL, NULL, hInstaice, NULL);

				DEVMODE    devMode;
				devMode.dmSize            = sizeof(DEVMODE);
				devMode.dmFields         = DM_PELSWIDTH | DM_PELSHEIGHT;
				devMode.dmPelsWidth  = this->width;
				devMode.dmPelsHeight = this->height;

#if (defined(DEBUG) | defined(_DEBUG))
				SetMenu(hwnd, NULL);	//メニューを隠す
				SetWindowLong(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);//ウィンドウのスタイルを変更
				MoveWindow(hwnd,
					GetSystemMetrics(SM_XVIRTUALSCREEN),
					GetSystemMetrics(SM_YVIRTUALSCREEN),
					(int)this->width,//GetSystemMetrics(SM_CXVIRTUALSCREEN),
					(int)this->height,//GetSystemMetrics(SM_CYVIRTUALSCREEN),
					TRUE);

				//ウィンドウを画面に合わせる
				ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
#endif // DEBUG

				// 表示
				ShowWindow(hwnd, SW_SHOW);
				UpdateWindow(hwnd);
			}

			// Icon
			//SetClassLongW(hwnd, GCL_HICON, static_cast<LONG>((LONG_PTR)LoadIconW(hInstaice, (101 == 0) ? (LPWSTR)IDI_APPLICATION : MAKEINTRESOURCEW(101))));

			// Drag&Dropに対応
			DragAcceptFiles(hwnd, TRUE);

			// 自分を関連付け
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

			//InputSystem 初期化
			//InputSystem::Instance()->Initialize(hwnd);

			// TODO : 作り直し パッド初期化
			PAD.hwnd     = hwnd;
			PAD.instance = hInstaice;
			PAD.InitInputDevice();

#if !(defined(DEBUG) | defined(_DEBUG))
			HMONITOR hMonitor;
			MONITORINFOEX  MonitorInfoEx;
			POINT pt = { 1920, 1080 };

			//ptで指定した部分のモニターのハンドルを取得する
			hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);

			//モニター情報を取得する
			MonitorInfoEx.cbSize = sizeof(MonitorInfoEx);
			GetMonitorInfo(hMonitor, &MonitorInfoEx);

			int x = MonitorInfoEx.rcMonitor.left;
			int y = MonitorInfoEx.rcMonitor.top;
			int w = MonitorInfoEx.rcMonitor.right;
			int h = MonitorInfoEx.rcMonitor.bottom;

			SetMenu(hwnd, NULL);	//メニューを隠す
			SetWindowLong(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);//ウィンドウのスタイルを変更

			MoveWindow(hwnd, x, y, w, h, TRUE);
#endif

			return true;
		}

		void DisplayWin::Finalize()
		{
			if (hwnd != nullptr)
			{
				DestroyWindow(hwnd);
			}
			hwnd = nullptr;

			if (name[0])
			{
				UnregisterClass(name, hInstaice);
				name[0] = 0;
			}
		}

		Handle DisplayWin::GetHWND()
		{
			return hwnd;
		}

		int DisplayWin::GetWidth()
		{
			return width;
		}

		int DisplayWin::GetHeight()
		{
			return height;
		}

		WindowHandle DisplayWin::GetHandle()
		{
			return reinterpret_cast<WindowHandle>(hwnd);
		}

		// タイマーチック
		void DisplayWin::TimerTick()
		{
			timer.Tick();
		}

		// タイマーインターバル
		f32 DisplayWin::TimerInterval()
		{
			return timer.TimeInterval();
		}

		// フレームレートの表示
		void DisplayWin::CalculateFrameStats()
		{
			static int    frames     = 0;
			static float timeTlapsed = 0.0f;

			frames++;

			if ((timer.timeStamp() - timeTlapsed) >= 1.0f)
			{
				float fps  = static_cast<float>(frames);
				float mspf = 1000.0f / fps;
				std::ostringstream outs;
				outs.precision(6);
				outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
				SetWindowTextA(hwnd, outs.str().c_str()); // TODO : Draw fps
				fpsVal = fps;
				rate   = mspf;

				frames = 0;
				timeTlapsed += 1.0f;
			}
		}

		void DisplayWin::InputUpdate()
		{
			//InputSystem::Instance()->UpdateStates();
		}

#define DBT_DEVNODES_CHANGED 0x0007

		LRESULT CALLBACK DisplayWin::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			{
				return 1;
			}

			switch (msg)
			{
			case WM_PAINT:
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			case WM_CREATE:
				break;
			case WM_INPUT:
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			case WM_MOUSEHOVER:
				//DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
				break;
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
			case WM_SYSKEYDOWN:
			//	DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
				if (wparam == VK_ESCAPE) {
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				}
				break;
			case WM_ENTERSIZEMOVE:
				timer.Stop();
				break;
			case WM_EXITSIZEMOVE:
				timer.Start();
				break;
			case WM_DROPFILES:
				HDROP hdrop;
				WCHAR filename[MAX_PATH];
				int num, i;
				bool multipleLoad;
				multipleLoad = false;


				hdrop = (HDROP)wparam;

				// ドロップされたファイルの個数を得る
				num   = DragQueryFile(hdrop, -1, NULL, 0);

				if (1 < num) multipleLoad = true;

				// ファイルを処理
				for (i = 0; i < num; i++)
				{
					DragQueryFile(hdrop, i, filename, sizeof(filename));
					/*
					// filename にファイル名が入っているので、ここで処理を行う。
					//変換文字列格納バッファ
					char	filenameChar[256];

					size_t len = 0;
					errno_t err = 0;

					//ロケール指定
					//setlocale(LC_ALL, "japanese");

					//変換
					err = wcstombs_s(&len, filenameChar, sizeof(filenameChar), filename, _TRUNCATE);

					/*char fileName[256];
					bool isInput = false;
					int cnt = 0;
					for (int i = 0; i < 256; i++)
					{
						if ( filenameChar[i] == 'D' && filenameChar[i+1] == 'a' && filenameChar[i+2] == 't' && filenameChar[i+3] == 'a' )
						{
							isInput = true;
						}
						if ( isInput )
						{
							fileName[cnt++] = filenameChar[i];
						}
					}
					//SceneManager::GetInstance()->PassThePath(filenameChar, multipleLoad);*/
				}
				DragFinish(hdrop);
				break;
			default:
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}

			return 0;
		}
	} // namespace OS
} // namespace Bread