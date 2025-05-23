#pragma once
#ifndef _WINDOWEFFECTS_H_
#define _WINDOWEFFECTS_H_
#include "WindowsDWMIntegration.h"

namespace SAP {
	using namespace System;
	using namespace System::Diagnostics;
	ref class WindowEffects {
	public:
		WindowEffects(void* windowPointer);
		~WindowEffects();
		void enableWindowShadows();
		bool enableDesktopDarkMode();
		bool setCustomBorderColor();
		bool enableModernRoundedCorners();
		bool enableWindowAnimations();

		bool systemCompositionIsEnabled();

		IntPtr windowBorderlessHitTest(IntPtr wndProcLParam);
	private:
		Native::WindowsDWMIntegration* m_winIntegration;
	};
}
#endif // !_WINDOWEFFECTS_H_


