ifndef X64
.386
.model FLAT, C
endif

EXTRN g_pExports:PTR
EXTRN Load:PROC

.code

x_DllCanUnloadNow proc
	call Load
	jmp g_pExports[1 * SIZEOF g_pExports]
x_DllCanUnloadNow endp

x_DllGetClassObject proc
	call Load
	jmp g_pExports[2 * SIZEOF g_pExports]
x_DllGetClassObject endp

x_DllRegisterServer proc
	call Load
	jmp g_pExports[3 * SIZEOF g_pExports]
x_DllRegisterServer endp

x_DllUnregisterServer proc
	call Load
	jmp g_pExports[4 * SIZEOF g_pExports]
x_DllUnregisterServer endp

end