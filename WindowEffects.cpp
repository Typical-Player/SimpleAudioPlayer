#include "WindowEffects.h"

SAP::WindowEffects::WindowEffects(void* windowPointer) {
	m_winIntegration = new Native::WindowsDWMIntegration((HWND)windowPointer);
}

SAP::WindowEffects::~WindowEffects() {
	delete m_winIntegration;
}

void SAP::WindowEffects::enableWindowShadows() {
	if (!m_winIntegration->enableDwmShadows()) {
		Debug::WriteLine("WindowEffects: ERROR: Unable to set DWM shadows");
		return;
	}

	Debug::WriteLine("WindowEffects: OK: DWM shadows set");
}

bool SAP::WindowEffects::enableDesktopDarkMode() {
	return m_winIntegration->enableInmersiveDarkMode();
}

bool SAP::WindowEffects::setCustomBorderColor() {
	return m_winIntegration->enableCustomBorderColor();
}

bool SAP::WindowEffects::enableModernRoundedCorners() {
	return m_winIntegration->enableRoundedCorners();
}

bool SAP::WindowEffects::enableWindowAnimations() {
	return m_winIntegration->enableTransitionAnimations();
}

bool SAP::WindowEffects::systemCompositionIsEnabled() {
	return m_winIntegration->DWMCompositionIsEnabled();
}

System::IntPtr SAP::WindowEffects::windowBorderlessHitTest(IntPtr wndProcLParam) {
	return (IntPtr)m_winIntegration->windowHitTest((LPARAM)wndProcLParam);
}
