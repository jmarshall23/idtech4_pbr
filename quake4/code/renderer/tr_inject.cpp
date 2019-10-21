// tr_inject.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "tr_gamerender.h"

void (*RB_ARB2_DrawInteractionEngine)(drawInteraction_t *din);
void(*RB_DrawViewEngine)(const void *data);
void (*R_LoadImageEngine)(const char *name, byte **pic, int *width, int *height, unsigned int *timestamp);

void *(*R_StaticAlloc)(int size) = (void *(__cdecl *)(int))0x10129100;
void *(__fastcall * idImage_GetDownsizeEngine)(void *_this, int &scaled_width, int &scaled_height);

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

	//{
	//	void *function = (void *)0x1012C3F0;
	//	MH_CreateHook(function, RB_DrawView, (LPVOID *)&RB_DrawViewEngine);
	//	MH_EnableHook(function);
	//}

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
}