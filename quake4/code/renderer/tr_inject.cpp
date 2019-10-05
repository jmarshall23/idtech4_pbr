// tr_inject.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "tr_gamerender.h"

void (*RB_ARB2_DrawInteractionEngine)(drawInteraction_t *din);

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
}