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
	IdCompiler                           = 9,
	IdDefaultLogFormat                   = 10,
	IdDensityRange                       = 11,
	IdDensityScale                       = 12,
	IdEntity                             = 13,
	IdExposureScale                      = 14,
	IdFieldOfView                        = 15,
	IdFileAccess                         = 16,
	IdFramebuffer                        = 17,
	IdGL_BOOL_ARB                        = 18,
	IdGL_BOOL_VEC2_ARB                   = 19,
	IdGL_BOOL_VEC3_ARB                   = 20,
	IdGL_BOOL_VEC4_ARB                   = 21,
	IdGL_FLOAT                           = 22,
	IdGL_FLOAT_MAT2_ARB                  = 23,
	IdGL_FLOAT_MAT3_ARB                  = 24,
	IdGL_FLOAT_MAT4_ARB                  = 25,
	IdGL_FLOAT_VEC2_ARB                  = 26,
	IdGL_FLOAT_VEC3_ARB                  = 27,
	IdGL_FLOAT_VEC4_ARB                  = 28,
	IdGL_INT                             = 29,
	IdGL_INT_VEC2_ARB                    = 30,
	IdGL_INT_VEC3_ARB                    = 31,
	IdGL_INT_VEC4_ARB                    = 32,
	IdGL_INVALID                         = 33,
	IdGL_SAMPLER_1D                      = 34,
	IdGL_SAMPLER_1D_SHADOW_ARB           = 35,
	IdGL_SAMPLER_2D                      = 36,
	IdGL_SAMPLER_2D_SHADOW_ARB           = 37,
	IdGL_SAMPLER_3D                      = 38,
	IdGadgetDataFile                     = 39,
	IdGadgetDataSet                      = 40,
	IdGadgetSnapshot                     = 41,
	IdGadgetWorkItem                     = 42,
	IdGadgetWorkQueue                    = 43,
	IdGeometry                           = 44,
	IdGlfwWindow                         = 45,
	IdGlutWindow                         = 46,
	IdGraphicsContext                    = 47,
	IdGraphicsShader                     = 48,
	IdGwenContext                        = 49,
	IdHiDistData                         = 50,
	IdHiDistDataset                      = 51,
	IdHiDistWorkItem                     = 52,
	IdHiDistWorkQueue                    = 53,
	IdImageInput                         = 54,
	IdImageOutput                        = 55,
	IdIntensityBias                      = 56,
	IdIntensityScale                     = 57,
	IdInterfaceWindow                    = 58,
	IdLogChannel                         = 59,
	IdLogContext                         = 60,
	IdLogFormat                          = 61,
	IdMainThread                         = 62,
	IdMaxPointScale                      = 63,
	IdMemoryIndex                        = 64,
	IdMinPointScale                      = 65,
	IdModelView                          = 66,
	IdModule                             = 67,
	IdMotionPointBasedRenderer           = 68,
	IdMotionSpeed                        = 69,
	IdMotionTime                         = 70,
	IdMotionTransform                    = 71,
	IdObject                             = 72,
	IdOpenGLContext                      = 73,
	IdOpenGLShader                       = 74,
	IdParamSet                           = 75,
	IdParticleColor                      = 76,
	IdParticleDensity                    = 77,
	IdParticleRadius                     = 78,
	IdPointScale                         = 79,
	IdProjection                         = 80,
	IdRawFileLog                         = 81,
	IdRepository                         = 82,
	IdRuntimeContext                     = 83,
	IdScreenSize                         = 84,
	IdSerializable                       = 85,
	IdSmoothingRadius                    = 86,
	IdSmoothingScale                     = 87,
	IdSphPointBasedRenderer              = 88,
	IdStream                             = 89,
	IdStreamLog                          = 90,
	IdSymbolRegistry                     = 91,
	IdThread                             = 92,
	IdTiledRenderer                      = 93,
	IdTimer                              = 94,
	IdUseGlfw                            = 95,
	IdUseGlut                            = 96,
	IdVertexRange                        = 97,
	IdWdC                                = 98,
	IdWorkItem                           = 99,
	IdWorkQueue                          = 100,
	IdWorkThread                         = 101,
	IdX                                  = 102,
};

static const vd::uid Keys[103];

static const char* Strings[103];

static const vd::symbol Table[103];


// ================================================================================================ //

}; // end struct: Static

// ================================================================================================ //

} // end namespace: Global
} // end namespace: Symbols

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_END();

// ================================================================================================ //

#endif // VD_CONSTANTS_EXTRACTED_GLOBAL_SYMBOLS_INCLUDED

