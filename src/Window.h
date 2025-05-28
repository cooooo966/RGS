#pragma once
#include"InputCode.h"
#include"FrameBuffer.h"

namespace RGS {
	class Window
	{
	public:
		Window(const char* title,const int width,const int height);
		virtual ~Window();
		virtual void Show() = 0;
		virtual void DrawFrameBuffer(const Framebuffer& framebuffer) = 0;

		static void Init();
		static void Terminate();
		static Window* Create(const char* name, const int width, const int height);
		static void PollInputEvent();

		bool IsClosed() const { return m_IsClosed; }
		bool GetKeyState(const int index) { return m_KeysState[index]; }

	protected:
		const char* m_Title;
		int m_Width, m_Height;
		bool m_IsClosed = true;
		bool m_KeysState[RGS_KEY_MAX_COUNT];
	};
}