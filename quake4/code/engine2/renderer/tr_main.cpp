// tr_main.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "tr_gamerender.h"

idImage *hdrLightingImage = nullptr;
idImage *forwardAlbedoImage = nullptr;
idImage *forwardDepthImage = nullptr;
idImage *forwardGlowImage = nullptr;

bool *r_isInitilized = (bool *)(0x111243D7);

idRenderTexture *forwardRender;
GLuint basicOccluderVertex;
GLuint basicOccluderFragment;

/*
===================
R_InitHDRLighting
===================
*/
void R_InitHDRLighting(idImage *image) {
	int numPixels = renderSystem->GetScreenWidth() * renderSystem->GetScreenHeight();

	byte *tempData = new byte[numPixels  * 4];

	image->GenerateImage(tempData, renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight(), TF_DEFAULT, false, TR_REPEAT, TD_HIGH_QUALITY, 0);
	glBindTexture(GL_TEXTURE_2D, image->texnum);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	delete tempData;
}

/*
===================
R_InitAlbedo
===================
*/
void R_InitAlbedo(idImage *image) {
	int numPixels = renderSystem->GetScreenWidth() * renderSystem->GetScreenHeight();

	byte *tempData = new byte[numPixels * 4];

	image->GenerateImage(tempData, renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight(), TF_DEFAULT, false, TR_REPEAT, TD_HIGH_QUALITY, 0);
	glBindTexture(GL_TEXTURE_2D, image->texnum);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	delete tempData;
}


/*
===================
R_InitDepthTexture
===================
*/
void R_InitDepthTexture(idImage *image) {
	int numPixels = renderSystem->GetScreenWidth() * renderSystem->GetScreenHeight();

	byte *tempData = new byte[numPixels * 4];

	image->GenerateImage(tempData, renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight(), TF_DEFAULT, false, TR_REPEAT, TD_HIGH_QUALITY, 0);
	glBindTexture(GL_TEXTURE_2D, image->texnum);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, renderSystem->GetScreenWidth(), renderSystem->GetScreenHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
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
	common->Printf("------ R_InitGameRender --------\n");
	if(glewInit() != GLEW_OK) {
		common->FatalError("Failed to init GLEW\n");
	}

	hdrLightingImage = globalImages->ImageFromFunction("_forwardHDRLighting", R_InitHDRLighting);
	forwardAlbedoImage = globalImages->ImageFromFunction("_forwardAlbedo", R_InitAlbedo);
	forwardGlowImage = globalImages->ImageFromFunction("_forwardGlow", R_InitAlbedo);
	forwardDepthImage = globalImages->ImageFromFunction("_forwardDepth", R_InitDepthTexture);

	basicOccluderVertex = R_FindARBProgram(GL_VERTEX_PROGRAM_ARB, "basicoccluder.vfp");
	basicOccluderFragment = R_FindARBProgram(GL_FRAGMENT_PROGRAM_ARB, "basicoccluder.vfp");

	forwardRender = new idRenderTexture(hdrLightingImage, forwardDepthImage);
	forwardRender->AddRenderImage(forwardAlbedoImage);
	forwardRender->AddRenderImage(forwardGlowImage);
	forwardRender->InitRenderTexture(false, true);

	// Disable hardware brightness/gamma
	cvarSystem->SetCVarInteger("r_brightness", 1);
	cvarSystem->SetCVarInteger("r_gamma", 1);
}