// Tools_main.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "Tools_local.h"
#include "../renderer/tr_gamerender.h"
#include "../memoryinject/MinHook.h"

void(*Tools_RunFrameEngine)(void);

/*
==================
Tools_RunFrame
==================
*/
void Tools_RunFrame(void) {
	HWND hwnd = R_GetWindowHandle();

	// Do not run any MFC events if the game window is visible. 
	if (IsWindowVisible(hwnd))
		return;

	Tools_RunFrameEngine();
}

/*
==================
Tools_Init
==================
*/
void Tools_Init(void) {
	char				dllPath[MAX_OSPATH];
	fileSystem->FindDLL("Tools", dllPath, false);
	int toolsDLLHandle = sys->DLL_Load(dllPath);

	if (!toolsDLLHandle) {
		common->Warning("Failed to hook tools dll, because its missing\n");
		return;
	}

	{
		void *function = (void *)0x30136920;
		MH_CreateHook(function, Tools_RunFrame, (LPVOID *)&Tools_RunFrameEngine);
		MH_EnableHook(function);
	}
}