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
	renderView_t		renderView;

	// MORE DATA HERE BUT I HAVEN'T DOCED UP YET.
} viewDef_t;

typedef struct {
	int		commandId, *next;
	viewDef_t	*viewDef;
} drawSurfsCommand_t;

viewDef_t **backEnd_viewDef = (viewDef_t **)0x11124D74;
byte *backeEnd_unknown = (byte *)0x11124D78;
bool *backEnd_currentRenderCopied = (bool *)0x11124DF8;
int *backEnd_pc_c_surfaces = (int *)0x11124DFC;

void(*GLimp_DeactivateContext)(void) = (void(__cdecl *)(void))0x1017C9B0;
void(*RB_ShowOverdraw)(void) = (void(__cdecl *)(void))0x1012D380;
void(*RB_STD_DrawView)(void) = (void(__cdecl *)(void))0x100A9EC0;
void(*GLimp_ActivateContext)(void) = (void(__cdecl *)(void))0x1017C990;
void(*RB_SetDefaultGLState)(void) = (void(__cdecl *)(void))0x1011FBD0;

/*
=============
RB_DrawView
=============
*/
void RB_DrawView(const void *data) {
	const drawSurfsCommand_t *cmd = (drawSurfsCommand_t *)data;
	struct viewEntity_s	*viewEntitys;
	int backEnd_numDrawSurfs;

	*backEnd_viewDef = cmd->viewDef; // dword_11124D74 = v1
	memcpy(backeEnd_unknown, (const void *)(((byte *)backEnd_viewDef) + 136), 0x40u);
	*backEnd_currentRenderCopied = false;

	viewEntitys = (struct viewEntity_s *)(((byte *)backEnd_viewDef) + 472);
	backEnd_numDrawSurfs = *(int *)(((byte *)backEnd_viewDef) + 460);

	// if there aren't any drawsurfs, do nothing
	//if (!backEnd_numDrawSurfs) {
	//	return;
	//}
	//
	//// skip render bypasses everything that has models, assuming
	//// them to be 3D views, but leaves 2D rendering visible
	//if (r_skipRender.GetBool() && viewEntitys) {
	//	return;
	//}
	//
	//// skip render context sets the wgl context to NULL,
	//// which should factor out the API cost, under the assumption
	//// that all gl calls just return if the context isn't valid
	//if (r_skipRenderContext.GetBool() && viewEntitys) {
	//	GLimp_DeactivateContext();
	//}

	*backEnd_pc_c_surfaces += backEnd_numDrawSurfs;

	RB_ShowOverdraw();

	// render the scene, jumping to the hardware specific interaction renderers
	RB_STD_DrawView();

	// restore the context for 2D drawing if we were stubbing it out
	if (r_skipRenderContext.GetBool() && viewEntitys) {
		GLimp_ActivateContext();
		RB_SetDefaultGLState();
	}
}