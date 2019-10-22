// tr_main.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "tr_gamerender.h"

idImage *hdrAlbedoImage = nullptr;
bool *r_isInitilized = (bool *)(0x111243D7);

/*
===================
R_InitHDRAlbedo
===================
*/
void R_InitHDRAlbedo(idImage *image) {
	int numPixels = renderSystem->GetScreenWidth() * renderSystem->GetScreenHeight();

	byte *tempData = new byte[numPixels  * 4];
	for(int i = 0; i < numPixels; i++)
	{
		tempData[(i * 4) + 0] = 0;
		tempData[(i * 4) + 1] = 255;
		tempData[(i * 4) + 2] = 0;
		tempData[(i * 4) + 3] = 255;
	}

	image->GenerateImage(tempData, renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight(), TF_DEFAULT, false, TR_REPEAT, TD_HIGH_QUALITY, 0);
	glBindTexture(GL_TEXTURE_2D, image->texnum);

	for (int i = 0; i < numPixels; i++)
	{
		tempData[(i * 4) + 0] = 0;
		tempData[(i * 4) + 1] = 0;
		tempData[(i * 4) + 2] = 255;
		tempData[(i * 4) + 3] = 255;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tempData);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	delete tempData;
}

/*
===================
R_InitGameRender
===================
*/
void R_InitGameRender(void) {
	hdrAlbedoImage = globalImages->ImageFromFunction("_hdrAlbedo", R_InitHDRAlbedo);
}