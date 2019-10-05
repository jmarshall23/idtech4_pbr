// draw_arb2.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
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

viewLight_t **backEndViewLight = (viewLight_t **)0x14E4910;

void RB_ARB2_DrawInteraction(drawInteraction_t *din) {
	viewLight_t *viewLight = *backEndViewLight;

	if (viewLight != nullptr)
	{
		if(viewLight->lightDef->parms.noSpecular)		
		{
			din->specularColor.Zero();
		}
	}

	RB_ARB2_DrawInteractionEngine(din);
}