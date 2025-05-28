#include"Window.h"
#include"WindowsWindow.h"
#include"Base.h"

namespace RGS {
	Window::Window(const char* title,const int width,const int height):m_Title(title), m_Width(width), m_Height(height)
	{
		m_IsClosed = true;
		memset(m_KeysState, RGS_RELEASE, RGS_KEY_MAX_COUNT);
	}

	Window::~Window()
	{
		m_IsClosed = true;
	}

	void Window::Init(){
		WindowsWindow::Init();
	}

	void Window::Terminate(){
		WindowsWindow::Terminate();
	}

	Window* Window::Create(const char* name, const int width, const int height) {
		ASSERT((width > 0) && (height > 0))
		return new WindowsWindow(name, width, height);
	}

	void Window::PollInputEvent() {
		WindowsWindow::PollInputEvent();
	}
}