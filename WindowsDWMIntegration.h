#pragma once
#ifndef _WINDOWSDWMINTEGRATION_H_
#define _WINDOWSDWMINTEGRATION_H_
#include <dwmapi.h>
#include <windowsx.h>
#pragma comment(lib, "dwmapi.lib")

namespace Native {
	class WindowsDWMIntegration {
	public:
		WindowsDWMIntegration(HWND windowHandlerPointer);
		~WindowsDWMIntegration();

		bool enableDwmShadows();
		bool enableInmersiveDarkMode();
		bool enableCustomBorderColor();
		bool enableRoundedCorners();
		bool enableTransitionAnimations();

		bool DWMCompositionIsEnabled();

		LRESULT windowHitTest(LPARAM wndProcParam);
	private:
		MARGINS m_windowMargins;
		HWND m_handle;
	};
}
#endif // !_WINDOWSDWMINTEGRATION_H_


