/*  Fixes for GTA:SA
*
*  Copyright (c) bartekdvd, 2014
*
*  This software is provided "as-is", without any express or implied warranty.
*  In no event will the authors be held liable for any damages arising from
*  the use of this software.
*
*  Permission is granted to anyone to use this software for any purpose,
*  including commercial applications, and to alter it and redistribute it
*  freely, subject to the following restrictions:
*
*  1.  The origin of this software must not be misrepresented; you must not
*      claim that you wrote the original software. If you use this software in
*      a product, an acknowledgment in the product documentation would be
*      appreciated but is not required.
*  2.  Altered source versions must be plainly marked as such, and must not be
*      misrepresented as being the original software.
*  3.  This notice may not be removed or altered from any source distribution.
*/

#include "fixes.h"

WindowProc_t GTAWindowProc = (WindowProc_t)GTA_ADDR_WINDOWPROC;

HWND* phWnd = (HWND*)GTA_ADDR_HWND;
D3DPRESENT_PARAMETERS* g_pGTAD3Dpp = (D3DPRESENT_PARAMETERS*)GTA_ADDR_D3DPP;
DIMOUSESTATE2 g_MouseState = { 0 };
Direct3D9Create_t Direct3D9CreateOrigin = NULL;
CreateDevice_t CreateDeviceOrigin = NULL;
Reset_t ResetOrigin = NULL;
int iWindowed = 0;
int iHDMouse = 0;

void VTableRedirect(LPVOID* vtbl, LPVOID hooked, LPVOID* origin, int idx)
{
	DWORD dwProtOld;
	VirtualProtect(&vtbl[idx], sizeof(LPVOID), PAGE_EXECUTE_READWRITE, &dwProtOld);
	if (origin)
		*origin = vtbl[idx];
	vtbl[idx] = hooked;
}

HRESULT WINAPI ResetHooked(IDirect3DDevice9* pDev, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	pPresentationParameters->Windowed = 1;
	pPresentationParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pPresentationParameters->Flags = 0;
	g_pGTAD3Dpp->Windowed = 1;
	g_pGTAD3Dpp->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	g_pGTAD3Dpp->Flags = 0;

	HRESULT hRes = ResetOrigin(pDev, pPresentationParameters);
	if (hRes == D3D_OK)
	{
		if (pPresentationParameters->BackBufferWidth != 0 && pPresentationParameters->BackBufferHeight != 0)
		{
			int width, height;
			width = GetSystemMetrics(SM_CXSCREEN);
			height = GetSystemMetrics(SM_CYSCREEN);
			SetWindowLongPtr(pPresentationParameters->hDeviceWindow, GWL_STYLE,
				WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE);
			SetWindowPos(pPresentationParameters->hDeviceWindow, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
			RECT rect;
			GetClientRect(pPresentationParameters->hDeviceWindow, &rect);
			if (rect.right != pPresentationParameters->BackBufferWidth || rect.bottom != pPresentationParameters->BackBufferHeight)
			{
				SetWindowPos(pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST,
					0,
					0,
					pPresentationParameters->BackBufferWidth + (pPresentationParameters->BackBufferWidth - rect.right),
					pPresentationParameters->BackBufferHeight +
					(pPresentationParameters->BackBufferHeight - rect.bottom), SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_NOMOVE);
			}
		}
	}
	return hRes;
}

HRESULT WINAPI CreateDeviceHooked(
	IDirect3D9* pD3D9,
	UINT Adapter,
	D3DDEVTYPE DeviceType,
	HWND hFocusWindow,
	DWORD BehaviorFlags,
	D3DPRESENT_PARAMETERS *pPresentationParameters,
	IDirect3DDevice9 **ppReturnedDeviceInterface
	)
{
	int width, height;
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	SetWindowLong(pPresentationParameters->hDeviceWindow, GWL_STYLE, WS_POPUP);
	SetWindowPos(pPresentationParameters->hDeviceWindow, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	MoveWindow(pPresentationParameters->hDeviceWindow, (width / 2) - (pPresentationParameters->BackBufferWidth / 2),
		(height / 2) - (pPresentationParameters->BackBufferHeight / 2), pPresentationParameters->BackBufferWidth,
		pPresentationParameters->BackBufferHeight, TRUE);
	
	pPresentationParameters->Windowed = 1;
	pPresentationParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pPresentationParameters->Flags = 0;
	g_pGTAD3Dpp->Windowed = 1;
	g_pGTAD3Dpp->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	g_pGTAD3Dpp->Flags = 0;

	HRESULT hRes = CreateDeviceOrigin(pD3D9, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	if (hRes == D3D_OK)
	{
		static int done = 0;
		if (!done)
		{
			VTableRedirect(**(LPVOID***)ppReturnedDeviceInterface, ResetHooked, (LPVOID*)&ResetOrigin, 16);
			done = 1;
		}
	}
	return hRes;
}

IDirect3D9* WINAPI Direct3D9CreateHooked(UINT SDKVersion)
{
	IDirect3D9* pD3D9 = Direct3D9CreateOrigin(SDKVersion);
	if (pD3D9 != NULL)
	{
		static int done = 0;
		if (!done)
		{
			VTableRedirect(*(LPVOID**)pD3D9, CreateDeviceHooked, (LPVOID*)&CreateDeviceOrigin, 16);
			done = 1;
		}
	}
	return pD3D9;
}

FARPROC WINAPI GetProcAddressHooked(HMODULE hModule, LPCSTR lpProcName)
{
	if (hModule == GetModuleHandle("d3d9.dll") && _stricmp("Direct3DCreate9", lpProcName) == 0)
	{
		if ((Direct3D9CreateOrigin = (Direct3D9Create_t)GetProcAddress(hModule, lpProcName)) != NULL)
			return (FARPROC)Direct3D9CreateHooked;
	}

	return GetProcAddress(hModule, lpProcName);
}

void SetUpFixes(LPVOID* ppIDI8)
{
	INI* ini = new INI("fixes.ini");
	if (ini->open())
	{
		if (ini->GoToSection("[VIDEO]"))
		{
			char buf[256];
			ini->GetValue("windowed", buf, sizeof(buf));
			if (_stricmp("true", buf) == 0)
				iWindowed = 1;
			else
			{
				if (_stricmp("false", buf) == 0)
					iWindowed = 0;
				else
					iWindowed = ini->GetIntegerValue("windowed");
			}
		}

		if (ini->GoToSection("[INPUT]"))
		{
			char buf[256];
			ini->GetValue("hd_mouse", buf, sizeof(buf));
			if (_stricmp("true", buf) == 0)
				iHDMouse = 1;
			else
			{
				if (_stricmp("false", buf) == 0)
					iHDMouse = 0;
				else
					iHDMouse = ini->GetIntegerValue("hd_mouse");
			}
		}

		ini->close();
	}
	delete(ini);

	if (iHDMouse)
	{
		SetProcessAffinityMask(GetCurrentProcess(), 1);
		SetWindowLongPtr(phWnd[0], GWLP_WNDPROC, (LONG_PTR)WindowProc);

		*ppIDI8 = new IProxyDirectInput8((IProxyDirectInput8*)*ppIDI8);

		RAWINPUTDEVICE Rid[1];
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = 0;
		Rid[0].hwndTarget = phWnd[0];
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
	}

	if (iWindowed)
	{
		VTableRedirect((LPVOID*)GTA_ADDR_GETPROCADDRESS, GetProcAddressHooked, NULL, 0);
	}
}

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	if (uMsg == WM_MOUSEWHEEL)
	{
		g_MouseState.lZ += GET_WHEEL_DELTA_WPARAM(wParam);
	}

	if (uMsg == WM_INPUT)
	{
		UINT dwSize;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
			sizeof(RAWINPUTHEADER));

		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL)
			return 0;

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			return 0;

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			g_MouseState.lX += raw->data.mouse.lLastX;
			g_MouseState.lY += raw->data.mouse.lLastY;

			if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN)
			{
				g_MouseState.rgbButtons[0] = 0x80;
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP)
			{
				g_MouseState.rgbButtons[0] = 0x00;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN)
			{
				g_MouseState.rgbButtons[1] = 0x80;
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP)
			{
				g_MouseState.rgbButtons[1] = 0x00;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN)
			{
				g_MouseState.rgbButtons[2] = 0x80;
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP)
			{
				g_MouseState.rgbButtons[2] = 0x00;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
			{
				g_MouseState.rgbButtons[3] = 0x80;
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
			{
				g_MouseState.rgbButtons[3] = 0x00;
			}
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
			{
				g_MouseState.rgbButtons[4] = 0x80;
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
			{
				g_MouseState.rgbButtons[4] = 0x00;
			}
		}
	}

	return GTAWindowProc(hwnd, uMsg, wParam, lParam);
}