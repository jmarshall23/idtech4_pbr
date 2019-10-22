// tr_inject.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "tr_gamerender.h"

void (*RB_ARB2_DrawInteractionEngine)(drawInteraction_t *din);
void (*R_LoadImageEngine)(const char *name, byte **pic, int *width, int *height, unsigned int *timestamp);
void(*RB_STD_DrawViewEngine)(void);

void *(*R_StaticAlloc)(int size) = (void *(__cdecl *)(int))0x10129100;
void *(__fastcall * idImage_GetDownsizeEngine)(void *_this, int &scaled_width, int &scaled_height);

idImageManager *globalImages = *((idImageManager **)0x10316B10);

void R_MemPatch(char* dst, char* src, int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

/*
===================
R_NonPowerOfTwoHack
===================
*/
void R_NonPowerOfTwoHack(void) {
	char memPatch[6] = { 0xFF, 0x91, 0x88, 0x00, 0x00, 0x00 }; // NOP, NOP
	char memPatch2[1] = { 0xEB }; // JMP was JZ

	// This function patches out generateImage throwing a error if we use non power of two.
	R_MemPatch((char *)0x100BA581, memPatch, 6);
	//R_MemPatch((char *)0x100BA572, memPatch2, 1);
}

/*
===================
R_InitInjection
===================
*/
void R_InitInjection(void) {
	common->Printf("R_InitInjections: Creating Injection Points...\n");
	
	MH_Initialize();

	{
		void *function = (void *)0x100A5790;
		MH_CreateHook(function, RB_ARB2_DrawInteraction, (LPVOID *)&RB_ARB2_DrawInteractionEngine);
		MH_EnableHook(function); 
	}

	{
		void *function = (void *)0x100A9EC0;
		MH_CreateHook(function, RB_STD_DrawView, (LPVOID *)&RB_STD_DrawViewEngine);
		MH_EnableHook(function);
	}

	{
		void *function = (void *)0x100AFA20;
		MH_CreateHook(function, R_LoadImage, (LPVOID *)&R_LoadImageEngine);
		MH_EnableHook(function);
	}

	{
		void *function = (void *)0x100B7A70;
		MH_CreateHook(function, idImage_GetDownsize, (LPVOID *)&idImage_GetDownsizeEngine);
		MH_EnableHook(function);
	}

	{
		void *function = (void *)0x100B7730;
		void(*notUsed)(void);
		MH_CreateHook(function, idImage::SelectInternalFormat, (LPVOID *)&notUsed);
		MH_EnableHook(function);
	}

	R_NonPowerOfTwoHack();
}