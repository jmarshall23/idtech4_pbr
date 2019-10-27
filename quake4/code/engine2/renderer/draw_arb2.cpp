// draw_arb2.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "tr_gamerender.h"

class idRenderLight {
public:
	virtual					~idRenderLight() { };

	virtual void			FreeRenderLight() = 0;
	virtual void			UpdateRenderLight(const renderLight_t *re, bool forceUpdate = false) = 0;
	virtual void			GetRenderLight(renderLight_t *re) = 0;
	virtual void			ForceUpdate() = 0;
	virtual int				GetIndex() = 0;

	renderLight_t			parms;
};

struct viewLight_t {
	struct viewLight_s *	next;

	// back end should NOT reference the lightDef, because it can change when running SMP
	idRenderLight *	lightDef;
};

void(*RB_Unknown_ARB2Function1)(void *param, int unknown);

viewLight_t **backEndViewLight = (viewLight_t **)0x11124E58;

void RB_ARB2_DrawInteraction(drawInteraction_t *din) {
	viewLight_t *viewLight = *backEndViewLight;

	if (viewLight != nullptr)
	{
		if(viewLight->lightDef->parms.noSpecular)		
		{
			din->specularColor.Zero();
		}
	}

	// load all the vertex program parameters
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_LIGHT_ORIGIN, din->localLightOrigin.ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_VIEW_ORIGIN, din->localViewOrigin.ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_LIGHT_PROJECT_S, din->lightProjection[0].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_LIGHT_PROJECT_T, din->lightProjection[1].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_LIGHT_PROJECT_Q, din->lightProjection[2].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_LIGHT_FALLOFF_S, din->lightProjection[3].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_BUMP_MATRIX_S, din->bumpMatrix[0].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_BUMP_MATRIX_T, din->bumpMatrix[1].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_DIFFUSE_MATRIX_S, din->diffuseMatrix[0].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_DIFFUSE_MATRIX_T, din->diffuseMatrix[1].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_SPECULAR_MATRIX_S, din->specularMatrix[0].ToFloatPtr());
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_SPECULAR_MATRIX_T, din->specularMatrix[1].ToFloatPtr());

	// testing fragment based normal mapping
	//if (r_testARBProgram.GetBool()) {
	//	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 2, din->localLightOrigin.ToFloatPtr());
	//	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 3, din->localViewOrigin.ToFloatPtr());
	//}

	//static const float zero[4] = { 0, 1, 0, 0 };
	//static const float one[4] = { 1, 1, 1, 1 };
	//static const float negOne[4] = { -1, -1, -1, -1 };
	//
	//switch (din->vertexColor) {
	//case SVC_IGNORE:
	//	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_COLOR_MODULATE, zero);
	//	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_COLOR_ADD, one);
	//	break;
	//case SVC_MODULATE:
	//	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_COLOR_MODULATE, one);
	//	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_COLOR_ADD, zero);
	//	break;
	//case SVC_INVERSE_MODULATE:
	//	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_COLOR_MODULATE, negOne);
	//	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_COLOR_ADD, one);
	//	break;
	//}
	static const float zero[4] = { 0, 1, 0, 0 };
	glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, PP_COLOR_MODULATE, zero);

	// set the constant colors
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 0, din->diffuseColor.ToFloatPtr());
	idVec4 specColor;

	// According to the assembly they are adding the spec contribution by itself before passing it to the shader.
	specColor = din->specularColor + din->specularColor;
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 1, specColor.ToFloatPtr());

	//RB_Unknown_ARB2Function1(din->unknownData, 1);

	// set the textures

	// texture 1 will be the per-surface bump map
	GL_SelectTexture(1);
	din->bumpImage->Bind();

	// texture 2 will be the light falloff texture
	GL_SelectTexture(2);
	din->lightFalloffImage->Bind();

	// texture 3 will be the light projection texture
	GL_SelectTexture(3);
	din->lightImage->Bind();

	// texture 4 is the per-surface diffuse map
	GL_SelectTexture(4);
	din->diffuseImage->Bind();

	// texture 5 is the per-surface specular map
	GL_SelectTexture(5);
	din->specularImage->Bind();

	// draw it
	RB_DrawElementsWithCounters(din->surf->geo);

	//RB_Unknown_ARB2Function1(din->unknownData, 0);
}