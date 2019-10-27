// tr_gamerender.h
//

#pragma once

#include "../memoryinject/MinHook.h"
#include "tr_rendertexture.h"

void __fastcall idImage_GenerateImage(idImage *_this, byte *notUsed, const byte *pic, int width, int height, textureFilter_t filter, bool allowDownSize, textureRepeat_t repeat, textureDepth_t depth, int unknown);
extern void(__fastcall *GenerateImageEngine)(idImage *_this, byte *notUsed, const byte *pic, int width, int height, textureFilter_t filter, bool allowDownSize, textureRepeat_t repeat, textureDepth_t depth, int unknown);

// idScreenRect gets carried around with each drawSurf, so it makes sense
// to keep it compact, instead of just using the idBounds class
class idScreenRect {
public:
	short		x1, y1, x2, y2;							// inclusive pixel bounds inside viewport
	float       zmin, zmax;								// for depth bounds test
};

class idImage {
public:
	void GenerateImage(const byte *pic, int width, int height, textureFilter_t filter, bool allowDownSize, textureRepeat_t repeat, textureDepth_t depth, int unknown)
	{
		//void (__fastcall *GenerateImageEngine)(idImage *_this, byte *notUsed, const byte *pic, int width, int height, textureFilter_t filter, bool allowDownSize, textureRepeat_t repeat, textureDepth_t depth, int unknown) = (void(__fastcall *)(idImage *, byte *, const byte *, int, int, textureFilter_t, bool, textureRepeat_t, textureDepth_t, int))0x100BA4B0;
		GenerateImageEngine(this, nullptr, pic, width, height, filter, allowDownSize, repeat, depth, unknown);
	}

	static  GLenum __fastcall SelectInternalFormat(idImage *image, void *notUsed, const byte **dataPtrs, int numDataPtrs, int width, int height, textureDepth_t minimumDepth, bool *monochromeResult) {
		return GL_RGBA;
	}

	int unknown2;
	GLuint texnum;
	
	void Bind(void)
	{
		void(__fastcall *Bind)(idImage *_this, byte *notUsed) = (void(__fastcall *)(idImage *, byte *))0x100BAC20;
		Bind(this, nullptr);
	}
};

typedef struct drawSurf_s {
	const srfTriangles_t	*geo;
	const struct viewEntity_s *space;
	const idMaterial		*material;	// may be NULL for shadow volumes
	float					sort;		// material->sort, modified by gui / entity sort offsets
	const float				*shaderRegisters;	// evaluated and adjusted for referenceShaders
	const struct drawSurf_s	*nextOnLight;	// viewLight chains
	idScreenRect			scissorRect;	// for scissor clipping, local inside renderView viewport
	int						dsFlags;			// DSF_VIEW_INSIDE_SHADOW, etc
	struct vertCache_s		*dynamicTexCoords;	// float * in vertex cache memory
	// specular directions for non vertex program cards, skybox texcoords, etc
} drawSurf_t;

struct drawInteraction_t {
	const drawSurf_t *	surf;

	idImage *			lightImage;
	idImage *			lightFalloffImage;
	idImage *			bumpImage;
	idImage *			diffuseImage;
	idImage *			specularImage;

	idVec4				diffuseColor;	// may have a light color baked into it, will be < tr.backEndRendererMaxLight
	idVec4				specularColor;	// may have a light color baked into it, will be < tr.backEndRendererMaxLight
	//stageVertexColor_t	vertexColor;	// applies to both diffuse and specular
	void				*unknownData;

	int					ambientLight;	// use tr.ambientNormalMap instead of normalization cube map 
	// (not a bool just to avoid an uninitialized memory check of the pad region by valgrind)
	// these are loaded into the vertex program
	idVec4				localLightOrigin;
	idVec4				localViewOrigin;
	idVec4				lightProjection[4];	// in local coordinates, possibly with a texture matrix baked in
	idVec4				bumpMatrix[2];
	idVec4				diffuseMatrix[2];
	idVec4				specularMatrix[2];
};

class idImageManager {
public:
	virtual void				Init() = 0;
	virtual void				Shutdown() = 0;
	virtual idImage *			ImageFromFile(const char *name, textureFilter_t filter, bool allowDownSize, textureRepeat_t repeat, textureDepth_t depth, cubeFiles_t cubeMap = CF_2D) = 0;
	virtual idImage *			GetImage(const char *name) const = 0;
	virtual idImage *			ImageFromFunction(const char *name, void(*generatorFunction)(idImage *image)) = 0;
};

/*

  All vertex programs use the same constant register layout:

c[4]	localLightOrigin
c[5]	localViewOrigin
c[6]	lightProjection S
c[7]	lightProjection T
c[8]	lightProjection Q
c[9]	lightFalloff	S
c[10]	bumpMatrix S
c[11]	bumpMatrix T
c[12]	diffuseMatrix S
c[13]	diffuseMatrix T
c[14]	specularMatrix S
c[15]	specularMatrix T


c[20]	light falloff tq constant

// texture 0 was cube map
// texture 1 will be the per-surface bump map
// texture 2 will be the light falloff texture
// texture 3 will be the light projection texture
// texture 4 is the per-surface diffuse map
// texture 5 is the per-surface specular map
// texture 6 is the specular half angle cube map

*/

typedef enum {
	PP_LIGHT_ORIGIN = 4,
	PP_VIEW_ORIGIN,
	PP_LIGHT_PROJECT_S,
	PP_LIGHT_PROJECT_T,
	PP_LIGHT_PROJECT_Q,
	PP_LIGHT_FALLOFF_S,
	PP_BUMP_MATRIX_S,
	PP_BUMP_MATRIX_T,
	PP_DIFFUSE_MATRIX_S,
	PP_DIFFUSE_MATRIX_T,
	PP_SPECULAR_MATRIX_S,
	PP_SPECULAR_MATRIX_T,
	PP_COLOR_MODULATE,
	PP_COLOR_ADD,

	PP_LIGHT_FALLOFF_TQ = 20	// only for NV programs
} programParameter_t;

extern idImageManager *globalImages;

void R_InitInjection(void);

void R_InitGameRender(void);

HWND R_GetWindowHandle(void);

void RB_ARB2_DrawInteraction(drawInteraction_t *din);
void RB_STD_DrawView(void);
void R_LoadImage(const char *cname, byte **pic, int *width, int *height, unsigned int *timestamp, bool makePowerOf2);
void __fastcall idImage_GetDownsize(void *_this, void *notUsed, int &scaled_width, int &scaled_height);
void RB_T_FillDepthBuffer(const drawSurf_t *surf);

//
// Original Engine Addresses
//
extern void(*RB_ARB2_DrawInteractionEngine)(drawInteraction_t *din);
extern void *(*R_StaticAlloc)(int size);
extern int(*R_FindARBProgram)(GLenum target, const char *program);
extern void(*RB_T_FillDepthBufferEngine)(const drawSurf_t *surf);
extern void(*RB_DrawElementsWithCounters)(const srfTriangles_t *tri);

extern void(*GL_SelectTexture)(int unit);

extern GLuint basicOccluderVertex;
extern GLuint basicOccluderFragment;


const int GLS_SRCBLEND_ZERO = 0x00000001;
const int GLS_SRCBLEND_ONE = 0x0;
const int GLS_SRCBLEND_DST_COLOR = 0x00000003;
const int GLS_SRCBLEND_ONE_MINUS_DST_COLOR = 0x00000004;
const int GLS_SRCBLEND_SRC_ALPHA = 0x00000005;
const int GLS_SRCBLEND_ONE_MINUS_SRC_ALPHA = 0x00000006;
const int GLS_SRCBLEND_DST_ALPHA = 0x00000007;
const int GLS_SRCBLEND_ONE_MINUS_DST_ALPHA = 0x00000008;
const int GLS_SRCBLEND_ALPHA_SATURATE = 0x00000009;
const int GLS_SRCBLEND_BITS = 0x0000000f;

const int GLS_DSTBLEND_ZERO = 0x0;
const int GLS_DSTBLEND_ONE = 0x00000020;
const int GLS_DSTBLEND_SRC_COLOR = 0x00000030;
const int GLS_DSTBLEND_ONE_MINUS_SRC_COLOR = 0x00000040;
const int GLS_DSTBLEND_SRC_ALPHA = 0x00000050;
const int GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA = 0x00000060;
const int GLS_DSTBLEND_DST_ALPHA = 0x00000070;
const int GLS_DSTBLEND_ONE_MINUS_DST_ALPHA = 0x00000080;
const int GLS_DSTBLEND_BITS = 0x000000f0;


// these masks are the inverse, meaning when set the glColorMask value will be 0,
// preventing that channel from being written
const int GLS_DEPTHMASK = 0x00000100;
const int GLS_REDMASK = 0x00000200;
const int GLS_GREENMASK = 0x00000400;
const int GLS_BLUEMASK = 0x00000800;
const int GLS_ALPHAMASK = 0x00001000;
const int GLS_COLORMASK = (GLS_REDMASK | GLS_GREENMASK | GLS_BLUEMASK);

const int GLS_POLYMODE_LINE = 0x00002000;

const int GLS_DEPTHFUNC_ALWAYS = 0x00010000;
const int GLS_DEPTHFUNC_EQUAL = 0x00020000;
const int GLS_DEPTHFUNC_LESS = 0x0;

const int GLS_ATEST_EQ_255 = 0x10000000;
const int GLS_ATEST_LT_128 = 0x20000000;
const int GLS_ATEST_GE_128 = 0x40000000;
const int GLS_ATEST_BITS = 0x70000000;

const int GLS_DEFAULT = GLS_DEPTHFUNC_ALWAYS;

//
// Render Targets.
//

extern idRenderTexture *forwardRender;