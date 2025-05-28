#include"WindowsWindow.h"
#include"Base.h"
#include<iostream>
#include"Maths.h"

#define RGS_WINDOW_CLASS_NAME "RGS"
#define RGS_WINDOW_ENTRY_NAME "Window"

namespace RGS {
	bool WindowsWindow::s_IsInited = false;

	WindowsWindow::WindowsWindow(const char* title,const int width,const int height):Window(title, width, height)
	{
		ASSERT(s_IsInited)
		DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;  //创建一个有标题框且带边框、具有最小化按钮、在标题条上带有窗口菜单的窗口
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.bottom = (long)height;
		rect.right = (long)width;
		AdjustWindowRect(&rect, style, false);
		m_Handle = CreateWindow(
			RGS_WINDOW_CLASS_NAME,                     // 窗口类名称
			m_Title,    // 窗口标题
			style,            // Window style
			// Size and position
			CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top,

			NULL,       // Parent window    
			NULL,       // Menu
			GetModuleHandle(NULL),  // Instance handle
			NULL        // Additional application data
		);
		ASSERT(m_Handle!=nullptr);

		m_IsClosed = false;
		SetProp(m_Handle, RGS_WINDOW_ENTRY_NAME, this);

		HDC windowDC = GetDC(m_Handle);
		m_MemoryDC = CreateCompatibleDC(windowDC);

		BITMAPINFOHEADER bitHeader = {};
		HBITMAP newBitmap;
		HBITMAP oldBitmap;
		bitHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitHeader.biWidth = (long)width;
		bitHeader.biHeight = -(long)height;
		bitHeader.biBitCount = 24;
		bitHeader.biPlanes = 1;
		bitHeader.biCompression = BI_RGB;

		newBitmap = CreateDIBSection(m_MemoryDC, (BITMAPINFO*)&bitHeader, DIB_RGB_COLORS, (void**)&m_Buffer, nullptr, 0);
		ASSERT(newBitmap != nullptr);
		constexpr uint32_t channelCount = 3;
		size_t size = m_Width * m_Height * channelCount * sizeof(unsigned char);
		memset(m_Buffer, 0, size);
		oldBitmap = (HBITMAP)SelectObject(m_MemoryDC, newBitmap);

		DeleteObject(oldBitmap);
		ReleaseDC(m_Handle, windowDC);

		//使窗口可见
		Show();
	}

	WindowsWindow::~WindowsWindow()
	{
		//对应构造函数
		ShowWindow(m_Handle, SW_HIDE);
		RemoveProp(m_Handle, RGS_WINDOW_ENTRY_NAME);
		DeleteDC(m_MemoryDC);
		DestroyWindow(m_Handle);
	}

	void WindowsWindow::Init(){
		ASSERT(!s_IsInited)
		Register();
		s_IsInited = true;
	}

	void WindowsWindow::Terminate(){
		//对应Init()
		ASSERT(s_IsInited)
		UnRegister();
		s_IsInited = false;
	}

	void WindowsWindow::Register() {
		//填充WNDCLASS结构
		WNDCLASS wc = {0 };
		wc.style = CS_HREDRAW | CS_VREDRAW; //窗口类样式
		wc.lpfnWndProc = WindowsWindow::WindowProc;  //窗口过程
		wc.cbClsExtra = 0;  //按照窗口类结构分配的额外字节数
		wc.cbWndExtra = 0;  //在窗口实例之后分配的额外字节数
		wc.hInstance = GetModuleHandle(NULL);  //返回当前可执行文件（.exe）的模块句柄
		wc.hIcon = NULL;  // 默认光标 
		wc.hCursor = NULL;  // 默认图标
		wc.lpszMenuName = NULL;  // 不要菜单
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //初始化为纯白色背景
		wc.lpszClassName = RGS_WINDOW_CLASS_NAME;

		//注册窗口
		RegisterClass(&wc);
	}

	void WindowsWindow::UnRegister() {
		UnregisterClass(RGS_WINDOW_CLASS_NAME, GetModuleHandle(NULL));
	}

	void WindowsWindow::Show() {
		ASSERT(m_Handle!=nullptr)
		HDC windowDC = GetDC(m_Handle);
		BitBlt(windowDC, 0, 0, m_Width, m_Height, m_MemoryDC, 0, 0, SRCCOPY);
		ShowWindow(m_Handle, SW_SHOW);
		ReleaseDC(m_Handle, windowDC);
	}

	void WindowsWindow::PollInputEvent() {
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		WindowsWindow* window = (WindowsWindow*)GetProp(hwnd, RGS_WINDOW_ENTRY_NAME);
		if(window==nullptr)
			return DefWindowProc(hwnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_DESTROY:
			window->m_IsClosed = true;
			return 0;
		case WM_KEYDOWN:
			KeyPressImpl(window, wParam, RGS_PRESS);
			return 0;
		case WM_KEYUP:
			KeyPressImpl(window, wParam, RGS_RELEASE);
			return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void WindowsWindow::KeyPressImpl(WindowsWindow* window,const WPARAM wParam,const bool KeyState) {
		if (wParam >= '0' && wParam <= '9') {
			window->m_KeysState[wParam] = KeyState;
			return;
		}
		if (wParam >= 'A' && wParam <= 'Z') {
			window->m_KeysState[wParam] = KeyState;
			return;
		}

		switch (wParam) {
		case VK_SPACE:
			window->m_KeysState[RGS_KEY_SPACE] = KeyState;
			break;
		case VK_SHIFT:
			window->m_KeysState[RGS_KEY_LEFT_SHIFT] = KeyState;
			break;
		default:
			break;
		}
	}

	void WindowsWindow::DrawFrameBuffer(const Framebuffer& framebuffer){
		const int fWidth = framebuffer.GetWidth();
		const int fHeight = framebuffer.GetHeight();
		const int width = m_Width < fWidth ? m_Width : fWidth;
		const int height = m_Height < fHeight ? m_Height : fHeight;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				constexpr int channelCount = 3;
				constexpr int rChannel = 2;
				constexpr int gChannel = 1;
				constexpr int bChannel = 0;

				Vec3 color = framebuffer.GetColor(j, fHeight - 1 - i);

				const int PixelStart = (i * m_Width + j) * channelCount;
				const int rIndex = PixelStart + rChannel;
				const int gIndex = PixelStart + gChannel;
				const int bIndex = PixelStart + bChannel;

				m_Buffer[rIndex] = Float2UChar(color.X);
				m_Buffer[gIndex] = Float2UChar(color.Y);
				m_Buffer[bIndex] = Float2UChar(color.Z);
			}
		}
		Show();
	}
	
}