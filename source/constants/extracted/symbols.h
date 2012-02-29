#ifndef VD_CONSTANTS_EXTRACTED_SYMBOLS_INCLUDED
#define VD_CONSTANTS_EXTRACTED_SYMBOLS_INCLUDED

// ================================================================================================ //

#include "core/core.h"
#include "core/symbol.h"
#include "constants/constants.h"

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_BEGIN();

// ================================================================================================ //

#define VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE	VD_CONSTANTS_NAMESPACE::extracted::Symbols

namespace extracted {
namespace Symbols {

// ================================================================================================ //

static const size_t Count = 73UL;

// ================================================================================================ //

enum Id
{
	IdInvalid                            = 0,
	IdAlphaScale                         = 1,
	IdApplication                        = 2,
	IdBoxSize                            = 3,
	IdBuffer                             = 4,
	IdColorRange                         = 5,
	IdDefaultLogFormat                   = 6,
	IdDensityRange                       = 7,
	IdDensityScale                       = 8,
	IdExposureScale                      = 9,
	IdFileAccess                         = 10,
	IdFileSystem                         = 11,
	IdFramebuffer                        = 12,
	IdGL_BOOL_ARB                        = 13,
	IdGL_BOOL_VEC2_ARB                   = 14,
	IdGL_BOOL_VEC3_ARB                   = 15,
	IdGL_BOOL_VEC4_ARB                   = 16,
	IdGL_FLOAT                           = 17,
	IdGL_FLOAT_MAT2_ARB                  = 18,
	IdGL_FLOAT_MAT3_ARB                  = 19,
	IdGL_FLOAT_MAT4_ARB                  = 20,
	IdGL_FLOAT_VEC2_ARB                  = 21,
	IdGL_FLOAT_VEC3_ARB                  = 22,
	IdGL_FLOAT_VEC4_ARB                  = 23,
	IdGL_INT                             = 24,
	IdGL_INT_VEC2_ARB                    = 25,
	IdGL_INT_VEC3_ARB                    = 26,
	IdGL_INT_VEC4_ARB                    = 27,
	IdGL_INVALID                         = 28,
	IdGL_SAMPLER_1D                      = 29,
	IdGL_SAMPLER_1D_SHADOW_ARB           = 30,
	IdGL_SAMPLER_2D                      = 31,
	IdGL_SAMPLER_2D_SHADOW_ARB           = 32,
	IdGL_SAMPLER_3D                      = 33,
	IdGadgetDataset                      = 34,
	IdGadgetSnapshot                     = 35,
	IdGadgetWorkItem                     = 36,
	IdGadgetWorkQueue                    = 37,
	IdGeometry                           = 38,
	IdGlutWindow                         = 39,
	IdGraphicsContext                    = 40,
	IdImageInput                         = 41,
	IdImageOutput                        = 42,
	IdIntensityScale                     = 43,
	IdInterfaceWindow                    = 44,
	IdLogChannel                         = 45,
	IdLogContext                         = 46,
	IdLogFormat                          = 47,
	IdMainThread                         = 48,
	IdMemoryIndex                        = 49,
	IdObject                             = 50,
	IdOpenGLContext                      = 51,
	IdParamSet                           = 52,
	IdParticleColor                      = 53,
	IdParticleDensity                    = 54,
	IdParticleRadius                     = 55,
	IdPointScale                         = 56,
	IdRawFileLog                         = 57,
	IdRuntimeContext                     = 58,
	IdShader                             = 59,
	IdSmoothingRadius                    = 60,
	IdSmoothingScale                     = 61,
	IdSphPointBasedRenderer              = 62,
	IdStreamLog                          = 63,
	IdSymbolRegistry                     = 64,
	IdThread                             = 65,
	IdTiledRenderer                      = 66,
	IdTimer                              = 67,
	IdWdC                                = 68,
	IdWorkItem                           = 69,
	IdWorkQueue                          = 70,
	IdWorkThread                         = 71,
	IdX                                  = 72,
};

// ================================================================================================ //

static const uid
Keys[] = 
{
	uid(0x9257d8e8ed07c6a6ULL, 0x182278a768bf512fULL),
	uid(0xf7a5e1e1b0a43e0fULL, 0xee0f564ed6568c9fULL),
	uid(0x8272f1fea9d93248ULL, 0xe50be92eff9879c1ULL),
	uid(0x2867e0d06cc69100ULL, 0xc52eb489ca5d1203ULL),
	uid(0x6d4d5724ca8621f5ULL, 0xb6014f4d67820d68ULL),
	uid(0x6230e30dfa392b01ULL, 0x42eb3774bbb1594dULL),
	uid(0xe006d4826b43f7dbULL, 0x2a7d42d9b5972e5fULL),
	uid(0xa3da548c49690203ULL, 0x986f0b1a223cc5d8ULL),
	uid(0x804cd2168fcebdf6ULL, 0xd770c8db75aa3dd3ULL),
	uid(0x257ceb25eaf425b4ULL, 0xf01843878f788c19ULL),
	uid(0x32b711a0ae3f8386ULL, 0xf0677852def9680eULL),
	uid(0xd61ea30157d2f009ULL, 0x334860bd152d6fe6ULL),
	uid(0x5cb490c5d6ba7eedULL, 0x5fbdc4372701f773ULL),
	uid(0x506eb025bdd1adf6ULL, 0x3bc855ac45b7cdccULL),
	uid(0x4f8de78673f628c7ULL, 0x1c6fff5d1ce9188fULL),
	uid(0x849f41753c6a4ad1ULL, 0xd5d515741de499c2ULL),
	uid(0x9034ed59f1e7c0bULL, 0x117984158e6797cULL),
	uid(0x4f6616108e877474ULL, 0x77a6de6a6298af28ULL),
	uid(0xbea38a7f13bccd83ULL, 0x88d01b3b21a14c59ULL),
	uid(0xfb54a513497129bcULL, 0x9fc012db099b95fULL),
	uid(0xfc77ebdc592c4a25ULL, 0xbfe524c97a1e6100ULL),
	uid(0x129150c18d5e2731ULL, 0x98869529ea7a866bULL),
	uid(0x2e47c4d51a1712aULL, 0x2e5737d88d70c47bULL),
	uid(0x8b8e392a5bfe0cfaULL, 0xf12da652b4376d07ULL),
	uid(0xfa97c51475a05a78ULL, 0x63f42193f0fd46b4ULL),
	uid(0x474323f9e55dfd46ULL, 0x5e91adc334607686ULL),
	uid(0x7b1bfa0a7d9c6a48ULL, 0x1f03f87b26dc707aULL),
	uid(0xf7315cf81cb697f3ULL, 0xd04246831ea97a7cULL),
	uid(0x7057b55850765c74ULL, 0x124809be662470b7ULL),
	uid(0x91168490a6c17bfbULL, 0x1e24e2b3557529c2ULL),
	uid(0x52d113913ef68e91ULL, 0x5ad7ea5e03c79b4eULL),
	uid(0x9d35966aab0546aULL, 0xd7708280f99466abULL),
	uid(0x1234c608369737a3ULL, 0x30cd673199b06d08ULL),
	uid(0x9f4084cb41c0351aULL, 0x9b4062c757ff5c02ULL),
	uid(0xf6501d985a469fb9ULL, 0x49e27e704f9b5c2dULL),
	uid(0xd006c19f67d8f1c9ULL, 0x3c6bd6d2c4c7b88fULL),
	uid(0x1be1dea5a25f68ebULL, 0xdf893f5f52792da2ULL),
	uid(0x3499c912f3ba7560ULL, 0x956956440590a658ULL),
	uid(0x77803ec821355be4ULL, 0x4db32050b620b54fULL),
	uid(0xecd7bf0c4671276ULL, 0x6952f5a37acc7faULL),
	uid(0x339ff38f209e31bcULL, 0x4c9c96f31025bd41ULL),
	uid(0x597088442f491675ULL, 0xd68f54695c1aa3d7ULL),
	uid(0xabcf53c42812fc39ULL, 0x661d98a8b0a5404fULL),
	uid(0x3b54f992c6735d45ULL, 0x56413b11549e259cULL),
	uid(0x95959fc3e4273398ULL, 0x3ecde25d7a4799c0ULL),
	uid(0xcd234439ca656683ULL, 0x25ebd467a13b049cULL),
	uid(0x86bb7831d33d5c25ULL, 0xd040d67ea1c5c6ceULL),
	uid(0x6b5e533a2e0ef822ULL, 0xb90a00086abfa2dbULL),
	uid(0xd6fc2ae139dea31ULL, 0x88f06e8a635393beULL),
	uid(0xf5fbf743d6c90a84ULL, 0xfe1c2e562001b235ULL),
	uid(0x38de351009f1b563ULL, 0x1cc23e5c8fdd1d2cULL),
	uid(0x755d15cca45e76beULL, 0x33fb62ec7a739967ULL),
	uid(0xd3660f2a4587910ULL, 0xd7f001c31d93cf7ULL),
	uid(0x533c85d0778cb4c0ULL, 0x6e2e452aac49bf94ULL),
	uid(0x902f444ba738e261ULL, 0xb62e24acb57fb34eULL),
	uid(0x5467857f4d23285dULL, 0xa3f637897b7d6098ULL),
	uid(0x7c8bf944104aff2cULL, 0xc5c5a20a782ef4a1ULL),
	uid(0x44dd0d29429301fbULL, 0x72f240f69c598b45ULL),
	uid(0x28db03d1259d66f9ULL, 0xc0cef3de79af561aULL),
	uid(0xa342eff3e70b7cc6ULL, 0x93bc9486ac9ff9f2ULL),
	uid(0xf10ad93e5855bb63ULL, 0xd2abde72cfe1cc0ULL),
	uid(0xefe15dcb11dec185ULL, 0xeb666484834d1bfeULL),
	uid(0x2a24fcecba8f5d36ULL, 0xbee8a95ea69ee7e3ULL),
	uid(0x8266b0c1ae2b74e6ULL, 0xfc64826b002423bcULL),
	uid(0xb76cc6154ca627c3ULL, 0xfa4ada2c621acd88ULL),
	uid(0xe36ddb2774996e9eULL, 0xac5db6abe00a11eaULL),
	uid(0x134f64ef6673aae8ULL, 0xf7c96dd6fb8e4f98ULL),
	uid(0x9b4531040adad9f2ULL, 0xded4ed4b440082e0ULL),
	uid(0xcede3fa2803f46dbULL, 0x2fab4397b79ed7dcULL),
	uid(0xac11f9120897396fULL, 0xd6a52caf1511783cULL),
	uid(0xddcdf775d664febaULL, 0x9f177465443d1d2aULL),
	uid(0x7f7418e1c61a3125ULL, 0x231253563b3cfd3fULL),
	uid(0xc9ddf94e64e9f364ULL, 0x20d3cfa192b26d6cULL),
	uid(0,0)
};

// ================================================================================================ //

static const char*
Strings[] = 
{
	"Invalid",
	"AlphaScale",
	"Application",
	"BoxSize",
	"Buffer",
	"ColorRange",
	"DefaultLogFormat",
	"DensityRange",
	"DensityScale",
	"ExposureScale",
	"FileAccess",
	"FileSystem",
	"Framebuffer",
	"GL_BOOL_ARB",
	"GL_BOOL_VEC2_ARB",
	"GL_BOOL_VEC3_ARB",
	"GL_BOOL_VEC4_ARB",
	"GL_FLOAT",
	"GL_FLOAT_MAT2_ARB",
	"GL_FLOAT_MAT3_ARB",
	"GL_FLOAT_MAT4_ARB",
	"GL_FLOAT_VEC2_ARB",
	"GL_FLOAT_VEC3_ARB",
	"GL_FLOAT_VEC4_ARB",
	"GL_INT",
	"GL_INT_VEC2_ARB",
	"GL_INT_VEC3_ARB",
	"GL_INT_VEC4_ARB",
	"GL_INVALID",
	"GL_SAMPLER_1D",
	"GL_SAMPLER_1D_SHADOW_ARB",
	"GL_SAMPLER_2D",
	"GL_SAMPLER_2D_SHADOW_ARB",
	"GL_SAMPLER_3D",
	"GadgetDataset",
	"GadgetSnapshot",
	"GadgetWorkItem",
	"GadgetWorkQueue",
	"Geometry",
	"GlutWindow",
	"GraphicsContext",
	"ImageInput",
	"ImageOutput",
	"IntensityScale",
	"InterfaceWindow",
	"LogChannel",
	"LogContext",
	"LogFormat",
	"MainThread",
	"MemoryIndex",
	"Object",
	"OpenGLContext",
	"ParamSet",
	"ParticleColor",
	"ParticleDensity",
	"ParticleRadius",
	"PointScale",
	"RawFileLog",
	"RuntimeContext",
	"Shader",
	"SmoothingRadius",
	"SmoothingScale",
	"SphPointBasedRenderer",
	"StreamLog",
	"SymbolRegistry",
	"Thread",
	"TiledRenderer",
	"Timer",
	"WdC",
	"WorkItem",
	"WorkQueue",
	"WorkThread",
	"X",
	0,0
};


// ================================================================================================ //

static const VD_CORE_NAMESPACE::Symbol
Table[] = 
{
	VD_CORE_NAMESPACE::Symbol( Keys[ IdInvalid ], Strings[ IdInvalid ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdAlphaScale ], Strings[ IdAlphaScale ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdApplication ], Strings[ IdApplication ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdBoxSize ], Strings[ IdBoxSize ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdBuffer ], Strings[ IdBuffer ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdColorRange ], Strings[ IdColorRange ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdDefaultLogFormat ], Strings[ IdDefaultLogFormat ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdDensityRange ], Strings[ IdDensityRange ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdDensityScale ], Strings[ IdDensityScale ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdExposureScale ], Strings[ IdExposureScale ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdFileAccess ], Strings[ IdFileAccess ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdFileSystem ], Strings[ IdFileSystem ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdFramebuffer ], Strings[ IdFramebuffer ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_BOOL_ARB ], Strings[ IdGL_BOOL_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_BOOL_VEC2_ARB ], Strings[ IdGL_BOOL_VEC2_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_BOOL_VEC3_ARB ], Strings[ IdGL_BOOL_VEC3_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_BOOL_VEC4_ARB ], Strings[ IdGL_BOOL_VEC4_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_FLOAT ], Strings[ IdGL_FLOAT ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_FLOAT_MAT2_ARB ], Strings[ IdGL_FLOAT_MAT2_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_FLOAT_MAT3_ARB ], Strings[ IdGL_FLOAT_MAT3_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_FLOAT_MAT4_ARB ], Strings[ IdGL_FLOAT_MAT4_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_FLOAT_VEC2_ARB ], Strings[ IdGL_FLOAT_VEC2_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_FLOAT_VEC3_ARB ], Strings[ IdGL_FLOAT_VEC3_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_FLOAT_VEC4_ARB ], Strings[ IdGL_FLOAT_VEC4_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_INT ], Strings[ IdGL_INT ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_INT_VEC2_ARB ], Strings[ IdGL_INT_VEC2_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_INT_VEC3_ARB ], Strings[ IdGL_INT_VEC3_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_INT_VEC4_ARB ], Strings[ IdGL_INT_VEC4_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_INVALID ], Strings[ IdGL_INVALID ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_SAMPLER_1D ], Strings[ IdGL_SAMPLER_1D ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_SAMPLER_1D_SHADOW_ARB ], Strings[ IdGL_SAMPLER_1D_SHADOW_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_SAMPLER_2D ], Strings[ IdGL_SAMPLER_2D ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_SAMPLER_2D_SHADOW_ARB ], Strings[ IdGL_SAMPLER_2D_SHADOW_ARB ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGL_SAMPLER_3D ], Strings[ IdGL_SAMPLER_3D ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGadgetDataset ], Strings[ IdGadgetDataset ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGadgetSnapshot ], Strings[ IdGadgetSnapshot ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGadgetWorkItem ], Strings[ IdGadgetWorkItem ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGadgetWorkQueue ], Strings[ IdGadgetWorkQueue ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGeometry ], Strings[ IdGeometry ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGlutWindow ], Strings[ IdGlutWindow ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdGraphicsContext ], Strings[ IdGraphicsContext ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdImageInput ], Strings[ IdImageInput ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdImageOutput ], Strings[ IdImageOutput ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdIntensityScale ], Strings[ IdIntensityScale ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdInterfaceWindow ], Strings[ IdInterfaceWindow ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdLogChannel ], Strings[ IdLogChannel ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdLogContext ], Strings[ IdLogContext ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdLogFormat ], Strings[ IdLogFormat ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdMainThread ], Strings[ IdMainThread ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdMemoryIndex ], Strings[ IdMemoryIndex ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdObject ], Strings[ IdObject ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdOpenGLContext ], Strings[ IdOpenGLContext ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdParamSet ], Strings[ IdParamSet ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdParticleColor ], Strings[ IdParticleColor ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdParticleDensity ], Strings[ IdParticleDensity ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdParticleRadius ], Strings[ IdParticleRadius ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdPointScale ], Strings[ IdPointScale ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdRawFileLog ], Strings[ IdRawFileLog ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdRuntimeContext ], Strings[ IdRuntimeContext ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdShader ], Strings[ IdShader ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdSmoothingRadius ], Strings[ IdSmoothingRadius ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdSmoothingScale ], Strings[ IdSmoothingScale ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdSphPointBasedRenderer ], Strings[ IdSphPointBasedRenderer ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdStreamLog ], Strings[ IdStreamLog ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdSymbolRegistry ], Strings[ IdSymbolRegistry ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdThread ], Strings[ IdThread ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdTiledRenderer ], Strings[ IdTiledRenderer ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdTimer ], Strings[ IdTimer ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdWdC ], Strings[ IdWdC ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdWorkItem ], Strings[ IdWorkItem ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdWorkQueue ], Strings[ IdWorkQueue ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdWorkThread ], Strings[ IdWorkThread ] ),
	VD_CORE_NAMESPACE::Symbol( Keys[ IdX ], Strings[ IdX ] ),
	VD_CORE_NAMESPACE::Symbol( 0, 0 )
};

// ================================================================================================ //

} // end namespace: Symbols
} // end namespace: extracted

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_END();

// ================================================================================================ //

#endif // VD_CONSTANTS_EXTRACTED_SYMBOLS_INCLUDED

