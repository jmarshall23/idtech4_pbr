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
void RB_DrawView(const void *data);
void R_LoadImage(const char *cname, byte **pic, int *width, int *height, unsigned int *timestamp, bool makePowerOf2);
void __fastcall idImage_GetDownsize(void *_this, void *notUsed, int &scaled_width, int &scaled_height);

//
// Original Engine Addresses
//
extern void(*RB_ARB2_DrawInteractionEngine)(drawInteraction_t *din);
extern void *(*R_StaticAlloc)(int size);