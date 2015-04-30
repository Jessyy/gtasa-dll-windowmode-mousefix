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

HINSTANCE g_hInst = NULL;
HMODULE g_hModule = NULL;
extern "C" LPVOID g_pExports[5] = {0};

extern "C" void Load()
{
	if (g_hModule != NULL)
		return;

	char szLibFile[264];
	GetSystemDirectory(szLibFile, sizeof(szLibFile));
	strcat_s(szLibFile, sizeof(szLibFile), "\\dinput8.dll");
	g_hModule = LoadLibrary(szLibFile);
	if (g_hModule == NULL)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(0, "File not found.", szLibFile, MB_OK | MB_ICONERROR);
		return;
	}
	
	g_pExports[0] = GetProcAddress(g_hModule, "DirectInput8Create");
	g_pExports[1] = GetProcAddress(g_hModule, "DllCanUnloadNow");
	g_pExports[2] = GetProcAddress(g_hModule, "DllGetClassObject");
	g_pExports[3] = GetProcAddress(g_hModule, "DllRegisterServer");
	g_pExports[4] = GetProcAddress(g_hModule, "DllUnregisterServer");
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		g_hInst = hinstDLL;
		break;

	case DLL_PROCESS_DETACH:
		if (g_hModule != NULL)
		{
			FreeLibrary(g_hModule);
			g_hModule = NULL;
		}
		break;
	}

	return TRUE;
}

extern "C" HRESULT _stdcall x_DirectInput8Create(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFIID riidltf,
	LPVOID * ppvOut,
	LPUNKNOWN punkOuter
)
{
	Load();
	HRESULT hres = ((HRESULT(_stdcall*)(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN))g_pExports[0])(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	if (hres == 0)
		SetUpFixes(ppvOut);
	return hres;
}

extern "C" void _stdcall x_DllCanUnloadNow();
extern "C" void _stdcall x_DllGetClassObject();
extern "C" void _stdcall x_DllRegisterServer();
extern "C" void _stdcall x_DllUnregisterServer();