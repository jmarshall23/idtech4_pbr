// tr_gamerender.h
//

#pragma once

#include "../memoryinject/MinHook.h"

class idImage;

struct drawSurf_t {

};

struct drawInteraction_t {
	const drawSurf_t *	surf;

	idImage *			lightImage;
	idImage *			lightFalloffImage;
	idImage *			bumpImage;
	idImage *			diffuseImage;
	idImage *			specularImage;

	idVec4				diffuseColor;	// may have a light color baked into it, will be < tr.backEndRendererMaxLight
	idVec4				specularColor;	// may have a light color baked into it, will be < tr.backEndRendererMaxLight
};

void R_InitInjection(void);

void RB_ARB2_DrawInteraction(drawInteraction_t *din);

//
// Original Engine Addresses
//
extern void(*RB_ARB2_DrawInteractionEngine)(drawInteraction_t *din);