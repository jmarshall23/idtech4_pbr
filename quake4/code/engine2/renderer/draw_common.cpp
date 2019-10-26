// draw_common.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "tr_gamerender.h"

/*
=========================
RB_T_FillDepthBuffer

This adds a albedo write pass to color target 1.
=========================
*/
void RB_T_FillDepthBuffer(const drawSurf_t *surf) {
	GLuint diffuseTexNum = -1;
	
	for(int i = 0; i < surf->material->GetNumStages(); i++)
	{
		if(surf->material->GetStage(i)->lighting == SL_DIFFUSE)
		{
			diffuseTexNum = surf->material->GetStage(i)->texture.image->texnum;
		}
	}
	
	if (diffuseTexNum != -1)
	{
		GL_SelectTexture(4);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, diffuseTexNum);
	
		glEnable(GL_VERTEX_PROGRAM_ARB);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, basicOccluderVertex);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, basicOccluderFragment);
		GL_SelectTexture(0);
	}
	RB_T_FillDepthBufferEngine(surf);
	if (diffuseTexNum != -1)
	{
		glDisable(GL_VERTEX_PROGRAM_ARB);
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
	
		GL_SelectTexture(4);
		glDisable(GL_TEXTURE_2D);
		GL_SelectTexture(0);
	}
}