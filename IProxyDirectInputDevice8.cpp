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

IProxyDirectInputDevice8::IProxyDirectInputDevice8(IDirectInputDevice8* p)
{
	this->pIDID8 = p;
}

IProxyDirectInputDevice8::~IProxyDirectInputDevice8()
{
}

HRESULT IProxyDirectInputDevice8::QueryInterface(THIS_ REFIID riid, LPVOID * ppvObj)
{
	if (riid == IID_IUnknown)
	{
		HRESULT hRes;
		if ((hRes = this->pIDID8->QueryInterface(riid, ppvObj)) == S_OK)
		{
			*ppvObj = this;
		}
		return hRes;
	}
	return  this->pIDID8->QueryInterface(riid, ppvObj);
}

ULONG IProxyDirectInputDevice8::AddRef()
{
	return this->pIDID8->AddRef();
}

ULONG IProxyDirectInputDevice8::Release()
{
	ULONG refcnt = this->pIDID8->Release();
	if (refcnt == 0)
	{
		delete this;
	}
	return refcnt;
}

HRESULT IProxyDirectInputDevice8::GetCapabilities(
	LPDIDEVCAPS lpDIDevCaps
	)
{
	return this->pIDID8->GetCapabilities(lpDIDevCaps);
}

HRESULT IProxyDirectInputDevice8::EnumObjects(
	LPDIENUMDEVICEOBJECTSCALLBACK lpCallback,
	LPVOID pvRef,
	DWORD dwFlags
	)
{
	return this->pIDID8->EnumObjects(lpCallback, pvRef, dwFlags);
}

HRESULT IProxyDirectInputDevice8::GetProperty(
	REFGUID rguidProp,
	LPDIPROPHEADER pdiph
	)
{
	return this->pIDID8->GetProperty(rguidProp, pdiph);
}

HRESULT IProxyDirectInputDevice8::SetProperty(
	REFGUID rguidProp,
	LPCDIPROPHEADER pdiph
	)
{
	return this->pIDID8->SetProperty(rguidProp, pdiph);
}

HRESULT IProxyDirectInputDevice8::Acquire()
{
	return DI_OK;
}

HRESULT IProxyDirectInputDevice8::Unacquire()
{
	return DI_OK;
}

HRESULT IProxyDirectInputDevice8::GetDeviceState(
	DWORD cbData,
	LPVOID lpvData
	)

{
	memcpy(lpvData, &g_MouseState, sizeof(DIMOUSESTATE2));

	g_MouseState.lX = 0;
	g_MouseState.lY = 0;
	g_MouseState.lZ = 0;

	return DI_OK;
}

HRESULT IProxyDirectInputDevice8::GetDeviceData(
	DWORD cbObjectData,
	LPDIDEVICEOBJECTDATA rgdod,
	LPDWORD pdwInOut,
	DWORD dwFlags
	)
{
	return this->pIDID8->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
}

HRESULT IProxyDirectInputDevice8::SetDataFormat(
	LPCDIDATAFORMAT lpdf
	)
{
	return this->pIDID8->SetDataFormat(lpdf);
}

HRESULT IProxyDirectInputDevice8::SetEventNotification(
	HANDLE hEvent
	)
{
	return this->pIDID8->SetEventNotification(hEvent);
}

HRESULT IProxyDirectInputDevice8::SetCooperativeLevel(
	HWND hwnd,
	DWORD dwFlags
	)
{
	return this->pIDID8->SetCooperativeLevel(hwnd, dwFlags);
}

HRESULT IProxyDirectInputDevice8::GetObjectInfo(
	LPDIDEVICEOBJECTINSTANCE pdidoi,
	DWORD dwObj,
	DWORD dwHow
	)
{
	return this->pIDID8->GetObjectInfo(pdidoi, dwObj, dwHow);
}

HRESULT IProxyDirectInputDevice8::GetDeviceInfo(
	LPDIDEVICEINSTANCE pdidi
	)

{
	return this->pIDID8->GetDeviceInfo(pdidi);
}

HRESULT IProxyDirectInputDevice8::RunControlPanel(
	HWND hwndOwner,
	DWORD dwFlags
	)
{
	return this->pIDID8->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT IProxyDirectInputDevice8::Initialize(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFGUID rguid
	)
{
	return this->pIDID8->Initialize(hinst, dwVersion, rguid);
}

HRESULT IProxyDirectInputDevice8::CreateEffect(
	REFGUID rguid,
	LPCDIEFFECT lpeff,
	LPDIRECTINPUTEFFECT * ppdeff,
	LPUNKNOWN punkOuter
	)
{
	return this->pIDID8->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
}

HRESULT IProxyDirectInputDevice8::EnumEffects(
	LPDIENUMEFFECTSCALLBACK lpCallback,
	LPVOID pvRef,
	DWORD dwEffType
	)
{
	return this->pIDID8->EnumEffects(lpCallback, pvRef, dwEffType);
}

HRESULT IProxyDirectInputDevice8::GetEffectInfo(
	LPDIEFFECTINFO pdei,
	REFGUID rguid
	)
{
	return this->pIDID8->GetEffectInfo(pdei, rguid);
}

HRESULT IProxyDirectInputDevice8::GetForceFeedbackState(
	LPDWORD pdwOut
	)
{
	return this->pIDID8->GetForceFeedbackState(pdwOut);
}

HRESULT IProxyDirectInputDevice8::SendForceFeedbackCommand(
	DWORD dwFlags
	)
{
	return this->pIDID8->SendForceFeedbackCommand(dwFlags);
}

HRESULT IProxyDirectInputDevice8::EnumCreatedEffectObjects(
	LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback,
	LPVOID pvRef,
	DWORD fl
	)
{
	return this->pIDID8->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
}

HRESULT IProxyDirectInputDevice8::Escape(
	LPDIEFFESCAPE pesc
	)
{
	return this->pIDID8->Escape(pesc);
}

HRESULT IProxyDirectInputDevice8::Poll()
{
	return this->pIDID8->Poll();
}

HRESULT IProxyDirectInputDevice8::SendDeviceData(
	DWORD cbObjectData,
	LPCDIDEVICEOBJECTDATA rgdod,
	LPDWORD pdwInOut,
	DWORD fl
	)

{
	return this->pIDID8->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
}

HRESULT IProxyDirectInputDevice8::EnumEffectsInFile(
	LPCSTR lpszFileName,
	LPDIENUMEFFECTSINFILECALLBACK pec,
	LPVOID pvRef,
	DWORD dwFlags
	)
{
	return this->pIDID8->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
}

HRESULT IProxyDirectInputDevice8::WriteEffectToFile(
	LPCSTR lpszFileName,
	DWORD dwEntries,
	LPDIFILEEFFECT rgDiFileEft,
	DWORD dwFlags
	)
{
	return this->pIDID8->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
}

HRESULT IProxyDirectInputDevice8::BuildActionMap(
	LPDIACTIONFORMAT lpdiaf,
	LPCTSTR lpszUserName,
	DWORD dwFlags
	)
{
	return this->pIDID8->BuildActionMap(lpdiaf, lpszUserName, dwFlags);
}

HRESULT IProxyDirectInputDevice8::SetActionMap(
	LPDIACTIONFORMAT lpdiActionFormat,
	LPCTSTR lptszUserName,
	DWORD dwFlags
	)

{
	return this->pIDID8->SetActionMap(lpdiActionFormat, lptszUserName, dwFlags);
}

HRESULT IProxyDirectInputDevice8::GetImageInfo(
	LPDIDEVICEIMAGEINFOHEADER lpdiDevImageInfoHeader
	)

{
	return this->pIDID8->GetImageInfo(lpdiDevImageInfoHeader);
}
