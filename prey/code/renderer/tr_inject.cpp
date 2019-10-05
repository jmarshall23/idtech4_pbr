// tr_inject.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "tr_gamerender.h"

void (*RB_ARB2_DrawInteractionEngine)(drawInteraction_t *din) = nullptr;
void(*RB_CreateSingleDrawInteractionsEngine)(const drawSurf_t *surf, void(*DrawInteraction)(drawInteraction_t *));

/*
===================
RB_CreateSingleDrawInteractions
===================
*/
void RB_CreateSingleDrawInteractions(const drawSurf_t *surf, void(*DrawInteraction)(drawInteraction_t *)) {
	RB_ARB2_DrawInteractionEngine = DrawInteraction;
	RB_CreateSingleDrawInteractionsEngine(surf, RB_ARB2_DrawInteraction); // Use our function instead!
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
		void *function = (void *)0x004E5540;
		MH_CreateHook(function, RB_CreateSingleDrawInteractions, (LPVOID *)&RB_CreateSingleDrawInteractionsEngine);
		MH_EnableHook(function); 
	}
}