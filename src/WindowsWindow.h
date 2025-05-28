#pragma once
#include"Window.h"
#include<windows.h>

namespace RGS{
	class WindowsWindow:public Window
	{
	private:
		HWND m_Handle;
		HDC m_MemoryDC;
		unsigned char* m_Buffer;
		static bool s_IsInited;

	public:
		WindowsWindow(const char* title,const int width,const int height);
		~WindowsWindow();
		static void Init();
		static void Terminate();
		static void PollInputEvent();
		virtual void Show() override;
		virtual void DrawFrameBuffer(const Framebuffer& framebuffer) override;

	private:
		static void Register();
		static void UnRegister();
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static void KeyPressImpl(WindowsWindow* window, const WPARAM wParam, const bool KeyState);
	};

}