// tr_renderer.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "tr_gamerender.h"

idCVar r_skipRender("r_skipRender", "0", CVAR_RENDERER | CVAR_BOOL, "skip 3D rendering, but pass 2D");
idCVar r_skipRenderContext("r_skipRenderContext", "0", CVAR_RENDERER | CVAR_BOOL, "NULL the rendering context during backend 3D rendering");

const int	MAX_CLIP_PLANES = 1;				// we may expand this to six for some subview issues

// viewDefs are allocated on the frame temporary stack memory
typedef struct viewDef_s {
	// specified in the call to DrawScene()
	//renderView_t		renderView;

	byte dummy[456];
	drawSurf_t **drawSurfs;
	int numDrawSurfs;
	int unknown1;
	int unknown2;
	int unknown3;
} viewDef_t;

typedef struct {
	int		commandId, *next;
	viewDef_t	*viewDef;
} drawSurfsCommand_t;

viewDef_t **backEnd_viewDef = (viewDef_t **)0x11124D74;
byte *backeEnd_unknown = (byte *)0x11124D78;
int *backEnd_depthFunc = (int *)0x11124E5C;

bool *backEnd_currentRenderCopied = (bool *)0x11124DF8;
int *backEnd_pc_c_surfaces = (int *)0x11124DFC;

void(*GLimp_DeactivateContext)(void) = (void(__cdecl *)(void))0x1017C9B0;
void(*RB_ShowOverdraw)(void) = (void(__cdecl *)(void))0x1012D380;
void(*GLimp_ActivateContext)(void) = (void(__cdecl *)(void))0x1017C990;
void(*RB_SetDefaultGLState)(void) = (void(__cdecl *)(void))0x1011FBD0;

void(*RB_BeginDrawingView)(void) = (void(__cdecl *)(void))0x1012B0F0;
void(*RB_DetermineLightScale)(void) = (void(__cdecl *)(void))0x1012BFB0;
void(*RB_STD_FillDepthBuffer)(drawSurf_t **drawSurfs, int numDrawSurfs) = (void(__cdecl *)(drawSurf_t **, int))0x100A7B70;
void(*RB_ARB2_DrawInteractions)(void) = (void(__cdecl *)(void))0x100A5D50;
void(*RB_DrawSurfacesWithFlags)(drawSurf_t **drawSurfs, int numDrawSurfs, int unknown, int unknown2) = (void(__cdecl *)(drawSurf_t **, int, int, int))0x100A9D10;
void(*RB_STD_FogAllLights)(void) = (void(__cdecl *)(void))0x100A8DE0;
void(*RB_STD_DrawShaderPasses)(drawSurf_t **drawSurfs, int numDrawSurfs) = (void(__cdecl *)(drawSurf_t **, int))0x10131570;
void(*RB_Unknown)(int unknown1, int unknown2) = (void(__cdecl *)(int, int))0x10112AA0;
int(*R_FindARBProgram)(GLenum target, const char *program) = (int(__cdecl *)(GLenum, const char *))0x100A6CE0;
void(*GL_SelectTexture)(int unit) = (void(__cdecl *)(int))0x1011EDA0;

/*
=============
RB_STD_DrawView

=============
*/
void RB_STD_DrawView(void) {
	drawSurf_t	 **drawSurfs;
	int			numDrawSurfs;

	*backEnd_depthFunc = GLS_DEPTHFUNC_EQUAL;

	drawSurfs = (*backEnd_viewDef)->drawSurfs;
	numDrawSurfs = (*backEnd_viewDef)->numDrawSurfs;

	if ((*backEnd_viewDef)->unknown3)
	{
		forwardRender->MakeCurrent();
	}

	RB_Unknown((*backEnd_viewDef)->unknown3, 1);

	// clear the z buffer, set the projection matrix, etc
	RB_BeginDrawingView();

	// decide how much overbrighting we are going to do
	RB_DetermineLightScale();

	// fill the depth buffer and clear color buffer to black except on
	// subviews
	RB_STD_FillDepthBuffer(drawSurfs, numDrawSurfs);

	RB_Unknown((*backEnd_viewDef)->unknown3, 0);

	// main light renderer
	RB_ARB2_DrawInteractions();

	// disable stencil shadow test
	glStencilFunc(GL_ALWAYS, 128, 255);

	// Draw the decals
	RB_DrawSurfacesWithFlags(drawSurfs, numDrawSurfs, -10000, 3);

	// Draw the VFX.
	RB_DrawSurfacesWithFlags(drawSurfs, numDrawSurfs, 4, 4);

	idRenderTexture::BindNull();

	// Draw the ambient
	if (!(*backEnd_viewDef)->unknown3)
	{
		RB_DrawSurfacesWithFlags(drawSurfs, numDrawSurfs, 5, 7);
	}

	// Draw the post process
	//RB_DrawSurfacesWithFlags(drawSurfs, numDrawSurfs, 100, 10000);
	//
	//
	//RB_STD_DrawShaderPasses(drawSurfs, numDrawSurfs);
}
