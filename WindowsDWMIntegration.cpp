#include "WindowsDWMIntegration.h"
#include "WindowEffects.h"

Native::WindowsDWMIntegration::WindowsDWMIntegration(HWND windowHandlerPointer) {
	m_windowMargins = { 1,1,1,1 };
	m_hwnd = windowHandlerPointer;
}

Native::WindowsDWMIntegration::~WindowsDWMIntegration() {}

bool Native::WindowsDWMIntegration::enableDwmShadows() {
	HRESULT hr = S_OK;
	const DWORD DWMWCP_ENABLED = DWMNCRENDERINGPOLICY::DWMNCRP_ENABLED;
	hr = DwmExtendFrameIntoClientArea(m_hwnd, &m_windowMargins);
	hr = DwmSetWindowAttribute(m_hwnd, DWMWA_NCRENDERING_POLICY, &DWMWCP_ENABLED, sizeof(DWMWCP_ENABLED));

	return SUCCEEDED(hr);
}

bool Native::WindowsDWMIntegration::enableInmersiveDarkMode() {
	HRESULT hr = S_OK;
	const BOOL idmENABLED = true;
	hr = DwmSetWindowAttribute(m_hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &idmENABLED, sizeof(idmENABLED));
	return SUCCEEDED(hr);
}

bool Native::WindowsDWMIntegration::enableCustomBorderColor() {
	HRESULT hr = S_OK;
	const COLORREF borderColor = 0x00344F69;
	hr = DwmSetWindowAttribute(m_hwnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
	return SUCCEEDED(hr);
}

bool Native::WindowsDWMIntegration::enableRoundedCorners() {
	HRESULT hr = S_OK;
	const DWORD DWMWCP_ROUND = DWM_WINDOW_CORNER_PREFERENCE::DWMWCP_DEFAULT;
	hr = DwmSetWindowAttribute(m_hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &DWMWCP_ROUND, sizeof(DWMWCP_ROUND));
	return SUCCEEDED(hr);
}

bool Native::WindowsDWMIntegration::DWMCompositionIsEnabled() {
	BOOL composition_enabled = false;
	bool ok = DwmIsCompositionEnabled(&composition_enabled) == S_OK;
	return composition_enabled && ok;
}

LRESULT Native::WindowsDWMIntegration::windowHitTest(LPARAM wndProcParam) {
	//Code from https://github.com/melak47/BorderlessWindow/blob/main/src/BorderlessWindow.cpp

	POINT cursor = {
		GET_X_LPARAM(wndProcParam),
		GET_Y_LPARAM(wndProcParam)
	};

	const POINT border{
		GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER),
		GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER)
	};

	RECT window{};
	if (!GetWindowRect(m_hwnd, &window)) {
		return HTNOWHERE;
	}

	const auto drag = HTCAPTION;

	enum region_mask {
		client = 0b0000,
		left = 0b0001,
		right = 0b0010,
		top = 0b0100,
		bottom = 0b1000,
	};

	const auto result =
		left * (cursor.x < (window.left + border.x)) |
		right * (cursor.x >= (window.right - border.x)) |
		top * (cursor.y < (window.top + border.y)) |
		bottom * (cursor.y >= (window.bottom - border.y));
	const auto borderless_resize = true;

	switch (result) {
	case left: return borderless_resize ? HTLEFT : drag;
	case right: return borderless_resize ? HTRIGHT : drag;
	case top: return borderless_resize ? HTTOP : drag;
	case bottom: return borderless_resize ? HTBOTTOM : drag;
	case top | left: return borderless_resize ? HTTOPLEFT : drag;
	case top | right: return borderless_resize ? HTTOPRIGHT : drag;
	case bottom | left: return borderless_resize ? HTBOTTOMLEFT : drag;
	case bottom | right: return borderless_resize ? HTBOTTOMRIGHT : drag;
	case client: return drag;
	default: return HTNOWHERE;
	}
}
