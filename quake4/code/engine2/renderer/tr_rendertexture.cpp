// RenderTexture.cpp
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "tr_gamerender.h"

/*
========================
idRenderTexture::idRenderTexture
========================
*/
idRenderTexture::idRenderTexture(idImage *colorImage, idImage *depthImage) {
	deviceHandle = -1;
	if (colorImage != nullptr)
	{
		AddRenderImage(colorImage);
	}
	this->depthImage = depthImage;
}

/*
========================
idRenderTexture::~idRenderTexture
========================
*/
idRenderTexture::~idRenderTexture() {
	if (deviceHandle != -1)
	{
		glDeleteFramebuffers(1, &deviceHandle);
		deviceHandle = -1;
	}
}
/*
================
idRenderTexture::AddRenderImage
================
*/
void idRenderTexture::AddRenderImage(idImage *image) {
	if (deviceHandle != -1) {
		common->FatalError("idRenderTexture::AddRenderImage: Can't add render image after FBO has been created!");
	}

	colorImages.Append(image);
}

/*
================
idRenderTexture::InitRenderTexture
================
*/
void idRenderTexture::InitRenderTexture(bool hasMSAA, bool hasDepthStencil) {
	glGenFramebuffers(1, &deviceHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, deviceHandle);

	for (int i = 0; i < colorImages.Num(); i++) {
		if (hasMSAA && i == 0)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, colorImages[i]->texnum, 0);			
		}
		else
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorImages[i]->texnum, 0);
		}
	}

	if (depthImage != nullptr) {
		if (!hasMSAA)
		{
			if (!hasDepthStencil) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthImage->texnum, 0);
			}
			else {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthImage->texnum, 0);
			}
		}
		else
		{
			if (!hasDepthStencil) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthImage->texnum, 0);
			}
			else {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthImage->texnum, 0);
			}
		}
	}

	if (colorImages.Num() > 0)
	{
		GLenum DrawBuffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		if (colorImages.Num() >= 5) {
			common->FatalError("InitRenderTextures: Too many render targets!");
		}
		glDrawBuffers(colorImages.Num(), &DrawBuffers[0]);
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		common->FatalError("idRenderTexture::InitRenderTexture: Failed to create rendertexture!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
================
idRenderTexture::MakeCurrent
================
*/
void idRenderTexture::MakeCurrent(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, deviceHandle);
}

/*
================
idRenderTexture::BindNull
================
*/
void idRenderTexture::BindNull(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
================
idRenderTexture::Resize
================
*/
void idRenderTexture::Resize(int width, int height) {
	//idImage *target = nullptr;
	//
	//if (colorImages.Num() > 0) {
	//	target = colorImages[0];
	//}
	//else {
	//	target = depthImage;
	//}
	//
	//if (target->GetOpts().width == width && target->GetOpts().height == height) {
	//	return;
	//}
	//
	//for(int i = 0; i < colorImages.Num(); i++) {
	//	colorImages[i]->Resize(width, height);
	//}
	//
	//if (depthImage != nullptr) {
	//	depthImage->Resize(width, height);
	//}
	//
	//if (deviceHandle != -1)
	//{
	//	glDeleteFramebuffers(1, &deviceHandle);
	//	deviceHandle = -1;
	//}
	//
	//InitRenderTexture();
}
