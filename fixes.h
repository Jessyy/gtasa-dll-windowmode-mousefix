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

#ifndef _FIXES_H
#define _FIXES_H

#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <WindowsX.h>
#include <InitGuid.h>
#include <dinput.h>
#include <d3d9.h>
#include "IProxyDirectInput8.h"
#include "IProxyDirectInputDevice8.h"
#include "INI.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

#define GTA_ADDR_WINDOWPROC 0x00747EB0
#define GTA_ADDR_HWND 0x00C8CF88
#define GTA_ADDR_D3DPP 0xC9C040
#define GTA_ADDR_D3D 0xC97C20
#define GTA_ADDR_D3DDEV 0xC97C28
#define GTA_ADDR_GETPROCADDRESS 0x0085806C

typedef LRESULT(CALLBACK* WindowProc_t)(HWND, UINT, WPARAM, LPARAM);
typedef HRESULT(WINAPI* Reset_t)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
typedef IDirect3D9*(WINAPI* Direct3D9Create_t)(UINT);
typedef HRESULT(WINAPI* CreateDevice_t)(
	IDirect3D9 *,
	UINT,
	D3DDEVTYPE,
	HWND,
	DWORD,
	D3DPRESENT_PARAMETERS *,
	IDirect3DDevice9 **
	);
void SetUpFixes(LPVOID* ppIDI8);
LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	);

extern DIMOUSESTATE2 g_MouseState;

#endif