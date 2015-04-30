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

IProxyDirectInput8::IProxyDirectInput8(IDirectInput8* p)
{
	this->pIDI8 = p;
}

IProxyDirectInput8::~IProxyDirectInput8()
{

}

HRESULT IProxyDirectInput8::QueryInterface(THIS_ REFIID riid, LPVOID * ppvObj)
{
	if (riid == IID_IUnknown)
	{
		HRESULT hRes;
		if ((hRes = this->pIDI8->QueryInterface(riid, ppvObj)) == S_OK)
		{
			*ppvObj = this;
		}
		return hRes;
	}
	return  this->pIDI8->QueryInterface(riid, ppvObj);
}

ULONG IProxyDirectInput8::AddRef()
{
	return this->pIDI8->AddRef();
}

ULONG IProxyDirectInput8::Release()
{
	ULONG refcnt = this->pIDI8->Release();
	if (refcnt == 0)
	{
		delete this;
	}
	return refcnt;
}

HRESULT IProxyDirectInput8::CreateDevice(THIS_ REFGUID rguid, LPDIRECTINPUTDEVICE8A* lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	if (rguid == GUID_SysMouse)
	{
		HRESULT hRes;
		if ((hRes = this->pIDI8->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter)) == DI_OK)
		{
			*lplpDirectInputDevice = new IProxyDirectInputDevice8(*lplpDirectInputDevice);
		}
		return hRes;
	}
	return this->pIDI8->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter);
}

HRESULT IProxyDirectInput8::EnumDevices(
	DWORD dwDevType,
	LPDIENUMDEVICESCALLBACK lpCallback,
	LPVOID pvRef,
	DWORD dwFlags
	)
{
	return this->pIDI8->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
}

HRESULT IProxyDirectInput8::GetDeviceStatus(
	REFGUID rguidInstance
	)
{
	return this->pIDI8->GetDeviceStatus(rguidInstance);
}

HRESULT IProxyDirectInput8::RunControlPanel(
	HWND hwndOwner,
	DWORD dwFlags
	)
{
	return this->pIDI8->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT IProxyDirectInput8::Initialize(
	HINSTANCE hinst,
	DWORD dwVersion
	)
{
	return this->pIDI8->Initialize(hinst, dwVersion);
}

HRESULT IProxyDirectInput8::FindDevice(
	REFGUID rguidClass,
	LPCTSTR ptszName,
	LPGUID pguidInstance
	)
{
	return this->pIDI8->FindDevice(rguidClass, ptszName, pguidInstance);
}

HRESULT IProxyDirectInput8::EnumDevicesBySemantics(
	LPCTSTR ptszUserName,
	LPDIACTIONFORMAT lpdiActionFormat,
	LPDIENUMDEVICESBYSEMANTICSCB lpCallback,
	LPVOID pvRef,
	DWORD dwFlags
	)
{
	return this->pIDI8->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
}

HRESULT IProxyDirectInput8::ConfigureDevices(
	LPDICONFIGUREDEVICESCALLBACK lpdiCallback,
	LPDICONFIGUREDEVICESPARAMS lpdiCDParams,
	DWORD dwFlags,
	LPVOID pvRefData
	)
{
	return this->pIDI8->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
}
