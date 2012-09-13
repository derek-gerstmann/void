#ifndef VD_CONSTANTS_EXTRACTED_GLOBAL_SYMBOLS_INCLUDED
#define VD_CONSTANTS_EXTRACTED_GLOBAL_SYMBOLS_INCLUDED

// ================================================================================================ //

#include "vd.h"
#include "constants/constants.h"

// ================================================================================================ //

#ifndef VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
#define VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE	VD_CONSTANTS_NAMESPACE::Symbols::Global
#endif // VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_BEGIN();

// ================================================================================================ //

namespace Symbols {
namespace Global {

// ================================================================================================ //

struct VD_API Static {

// ================================================================================================ //

static const size_t Count;

enum Id
{
	IdInvalid                            = 0,
	IdAlphaBias                          = 1,
	IdAlphaScale                         = 2,
	IdApplication                        = 3,
	IdBoxSize                            = 4,
	IdBuffer                             = 5,
	IdCameraPosition                     = 6,
	IdCocoaWindow                        = 7,
	IdColorRange                         = 8,
	IdDefaultLogFormat                   = 9,
	IdDensityRange                       = 10,
	IdDensityScale                       = 11,
	IdEntity                             = 12,
	IdExposureScale                      = 13,
	IdFieldOfView                        = 14,
	IdFileAccess                         = 15,
	IdFramebuffer                        = 16,
	IdGL_BOOL_ARB                        = 17,
	IdGL_BOOL_VEC2_ARB                   = 18,
	IdGL_BOOL_VEC3_ARB                   = 19,
	IdGL_BOOL_VEC4_ARB                   = 20,
	IdGL_FLOAT                           = 21,
	IdGL_FLOAT_MAT2_ARB                  = 22,
	IdGL_FLOAT_MAT3_ARB                  = 23,
	IdGL_FLOAT_MAT4_ARB                  = 24,
	IdGL_FLOAT_VEC2_ARB                  = 25,
	IdGL_FLOAT_VEC3_ARB                  = 26,
	IdGL_FLOAT_VEC4_ARB                  = 27,
	IdGL_INT                             = 28,
	IdGL_INT_VEC2_ARB                    = 29,
	IdGL_INT_VEC3_ARB                    = 30,
	IdGL_INT_VEC4_ARB                    = 31,
	IdGL_INVALID                         = 32,
	IdGL_SAMPLER_1D                      = 33,
	IdGL_SAMPLER_1D_SHADOW_ARB           = 34,
	IdGL_SAMPLER_2D                      = 35,
	IdGL_SAMPLER_2D_SHADOW_ARB           = 36,
	IdGL_SAMPLER_3D                      = 37,
	IdGadgetDataFile                     = 38,
	IdGadgetDataSet                      = 39,
	IdGadgetSnapshot                     = 40,
	IdGadgetWorkItem                     = 41,
	IdGadgetWorkQueue                    = 42,
	IdGeometry                           = 43,
	IdGlutWindow                         = 44,
	IdGraphicsContext                    = 45,
	IdGwenContext                        = 46,
	IdHiDistData                         = 47,
	IdHiDistDataset                      = 48,
	IdHiDistWorkItem                     = 49,
	IdHiDistWorkQueue                    = 50,
	IdImageInput                         = 51,
	IdImageOutput                        = 52,
	IdIntensityBias                      = 53,
	IdIntensityScale                     = 54,
	IdInterfaceWindow                    = 55,
	IdLogChannel                         = 56,
	IdLogContext                         = 57,
	IdLogFormat                          = 58,
	IdMainThread                         = 59,
	IdMaxPointScale                      = 60,
	IdMemoryIndex                        = 61,
	IdMinPointScale                      = 62,
	IdModelView                          = 63,
	IdModule                             = 64,
	IdMotionPointBasedRenderer           = 65,
	IdMotionSpeed                        = 66,
	IdMotionTime                         = 67,
	IdMotionTransform                    = 68,
	IdObject                             = 69,
	IdOpenGLContext                      = 70,
	IdParamSet                           = 71,
	IdParticleColor                      = 72,
	IdParticleDensity                    = 73,
	IdParticleRadius                     = 74,
	IdPointScale                         = 75,
	IdProjection                         = 76,
	IdRawFileLog                         = 77,
	IdRepository                         = 78,
	IdRuntimeContext                     = 79,
	IdScreenSize                         = 80,
	IdSerializable                       = 81,
	IdShader                             = 82,
	IdSmoothingRadius                    = 83,
	IdSmoothingScale                     = 84,
	IdSphPointBasedRenderer              = 85,
	IdStream                             = 86,
	IdStreamLog                          = 87,
	IdSymbolRegistry                     = 88,
	IdThread                             = 89,
	IdTiledRenderer                      = 90,
	IdTimer                              = 91,
	IdVertexRange                        = 92,
	IdWdC                                = 93,
	IdWorkItem                           = 94,
	IdWorkQueue                          = 95,
	IdWorkThread                         = 96,
	IdX                                  = 97,
};

static const vd::uid Keys[98];

static const char* Strings[98];

static const vd::symbol Table[98];


// ================================================================================================ //

}; // end struct: Static

// ================================================================================================ //

} // end namespace: Global
} // end namespace: Symbols

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_END();

// ================================================================================================ //

#endif // VD_CONSTANTS_EXTRACTED_GLOBAL_SYMBOLS_INCLUDED

