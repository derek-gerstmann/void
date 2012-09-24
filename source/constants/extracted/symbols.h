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
	IdGlutWindow                         = 45,
	IdGraphicsContext                    = 46,
	IdGraphicsShader                     = 47,
	IdGwenContext                        = 48,
	IdHiDistData                         = 49,
	IdHiDistDataset                      = 50,
	IdHiDistWorkItem                     = 51,
	IdHiDistWorkQueue                    = 52,
	IdImageInput                         = 53,
	IdImageOutput                        = 54,
	IdIntensityBias                      = 55,
	IdIntensityScale                     = 56,
	IdInterfaceWindow                    = 57,
	IdLogChannel                         = 58,
	IdLogContext                         = 59,
	IdLogFormat                          = 60,
	IdMainThread                         = 61,
	IdMaxPointScale                      = 62,
	IdMemoryIndex                        = 63,
	IdMinPointScale                      = 64,
	IdModelView                          = 65,
	IdModule                             = 66,
	IdMotionPointBasedRenderer           = 67,
	IdMotionSpeed                        = 68,
	IdMotionTime                         = 69,
	IdMotionTransform                    = 70,
	IdObject                             = 71,
	IdOpenGLContext                      = 72,
	IdOpenGLShader                       = 73,
	IdParamSet                           = 74,
	IdParticleColor                      = 75,
	IdParticleDensity                    = 76,
	IdParticleRadius                     = 77,
	IdPointScale                         = 78,
	IdProjection                         = 79,
	IdRawFileLog                         = 80,
	IdRepository                         = 81,
	IdRuntimeContext                     = 82,
	IdScreenSize                         = 83,
	IdSerializable                       = 84,
	IdSmoothingRadius                    = 85,
	IdSmoothingScale                     = 86,
	IdSphPointBasedRenderer              = 87,
	IdStream                             = 88,
	IdStreamLog                          = 89,
	IdSymbolRegistry                     = 90,
	IdThread                             = 91,
	IdTiledRenderer                      = 92,
	IdTimer                              = 93,
	IdVertexRange                        = 94,
	IdWdC                                = 95,
	IdWorkItem                           = 96,
	IdWorkQueue                          = 97,
	IdWorkThread                         = 98,
	IdX                                  = 99,
};

static const vd::uid Keys[100];

static const char* Strings[100];

static const vd::symbol Table[100];


// ================================================================================================ //

}; // end struct: Static

// ================================================================================================ //

} // end namespace: Global
} // end namespace: Symbols

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_END();

// ================================================================================================ //

#endif // VD_CONSTANTS_EXTRACTED_GLOBAL_SYMBOLS_INCLUDED

