#pragma once

#include <Windows.h>
#include <glm.hpp>

#include <gl/glew.h>
#include <gl/wglew.h>
#include "log.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:

	Window(HINSTANCE hInstance, LPCWSTR winName, glm::vec2 size)
	{
		if (!m_isInitialized)
		{
			Init(hInstance);
			m_isInitialized = true;
		}

		HWND m_hwnd = CreateWindowEx(
			0,                              // Optional window styles.
			m_className,                     // Window class
			winName,						// Window text
			WS_OVERLAPPEDWINDOW,            // Window style

			// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y,

			NULL,       // Parent window    
			NULL,       // Menu
			hInstance,  // Instance handle
			NULL        // Additional application data
		);

		CHECK_FOR_ERROR(m_hwnd, nullptr, GetLastErrorAsString().c_str());
		
		m_hdc = GetDC(m_hwnd);
		CHECK_FOR_ERROR(m_hdc, NULL, "Couldn`t get HDC");

		LOG_INFO("Window`s created.");
		ShowWindow(m_hwnd, SW_SHOWDEFAULT);

		m_isRunning = true;
	}

	~Window() 
	{
		PostMessage(m_hwnd, WM_DESTROY, NULL, NULL);
		OnUpdate();

		ReleaseDC(m_hwnd, m_hdc);
	}

	void OnUpdate()
	{
		MSG msg;
		while (PeekMessage(&msg, m_hwnd, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_isRunning = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool IsRunning() const { return m_isRunning; }

	void SwapBuffers()
	{
		CHECK_FOR_ERROR(::SwapBuffers(m_hdc), FALSE, GetLastErrorAsString().c_str());
	}

	void ClearScreen()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

private:

	void Init(HINSTANCE hInstance)
	{
		WNDCLASS wc = { };

		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = m_className;

		auto res = RegisterClass(&wc);


		CHECK_FOR_ERROR(res, 0, GetLastErrorAsString().c_str())
	}

private:

	HWND m_hwnd;
	HDC m_hdc;
	const wchar_t* m_className = L"Main Window Class";
	bool m_isInitialized = false;
	bool m_isRunning = false;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			HDC hdc = GetDC(hwnd);
			HGLRC  tempHGLRC, mainHGLRC;

			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
				PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
				32,                   // Colordepth of the framebuffer.
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				24,                   // Number of bits for the depthbuffer
				8,                    // Number of bits for the stencilbuffer
				0,                    // Number of Aux buffers in the framebuffer.
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};
			int  iPixelFormat;

			iPixelFormat = ChoosePixelFormat(hdc, &pfd);

			CHECK_FOR_ERROR(SetPixelFormat(hdc, iPixelFormat, &pfd), FALSE, GetLastErrorAsString().c_str());

			tempHGLRC = wglCreateContext(hdc);
			CHECK_FOR_ERROR(tempHGLRC, NULL, GetLastErrorAsString().c_str());
			wglMakeCurrent(hdc, tempHGLRC);

			auto err = glewInit();
			if (GLEW_OK != err)
			{
				LOG_ERR("GLEW wasn`t initialized", glewGetErrorString(err));
			}

			LOG_INFO((std::string("Using GLEW ") + (char*)glewGetString(GLEW_VERSION)).c_str());

			int attribs[] =
			{
			   WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			   WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			   WGL_CONTEXT_FLAGS_ARB, 0,
			   WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			   0
			};

			mainHGLRC = wglCreateContextAttribsARB(hdc, NULL, attribs);
			wglMakeCurrent(hdc, mainHGLRC);

			wglDeleteContext(tempHGLRC);
			ReleaseDC(hwnd, hdc);
		}
		break;

		case WM_DESTROY:
		{
			HGLRC hglrc = wglGetCurrentContext();
			wglMakeCurrent(NULL, NULL);

			CHECK_FOR_ERROR(wglDeleteContext(hglrc), FALSE, GetLastErrorAsString().c_str());
		}

			LOG_INFO("Rendering Contex Released");
			LOG_INFO("Destroing window..");
			PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}