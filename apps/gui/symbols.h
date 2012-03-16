#ifndef VD_CONSTANTS_EXTRACTED_DEMO_SYMBOLS_SCOPE
#define VD_CONSTANTS_EXTRACTED_DEMO_SYMBOLS_SCOPE

// ================================================================================================ //

#include "vd.h"
#include "constants/constants.h"

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_BEGIN();

// ================================================================================================ //

#ifndef VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
#define VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE	VD_CONSTANTS_NAMESPACE::Symbols::Demo
#endif // VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE

namespace Symbols {
namespace Demo {

// ================================================================================================ //

static const size_t Count = 75UL;

// ================================================================================================ //

enum Id
{
	IdInvalid                            = 0,
	IdAlphaScale                         = 1,
	IdApplication                        = 2,
	IdBoxSize                            = 3,
	IdBuffer                             = 4,
	IdCocoaWindow                        = 5,
	IdColorRange                         = 6,
	IdDefaultLogFormat                   = 7,
	IdDemo                               = 8,
	IdDensityRange                       = 9,
	IdDensityScale                       = 10,
	IdExposureScale                      = 11,
	IdFileAccess                         = 12,
	IdFramebuffer                        = 13,
	IdGL_BOOL_ARB                        = 14,
	IdGL_BOOL_VEC2_ARB                   = 15,
	IdGL_BOOL_VEC3_ARB                   = 16,
	IdGL_BOOL_VEC4_ARB                   = 17,
	IdGL_FLOAT                           = 18,
	IdGL_FLOAT_MAT2_ARB                  = 19,
	IdGL_FLOAT_MAT3_ARB                  = 20,
	IdGL_FLOAT_MAT4_ARB                  = 21,
	IdGL_FLOAT_VEC2_ARB                  = 22,
	IdGL_FLOAT_VEC3_ARB                  = 23,
	IdGL_FLOAT_VEC4_ARB                  = 24,
	IdGL_INT                             = 25,
	IdGL_INT_VEC2_ARB                    = 26,
	IdGL_INT_VEC3_ARB                    = 27,
	IdGL_INT_VEC4_ARB                    = 28,
	IdGL_INVALID                         = 29,
	IdGL_SAMPLER_1D                      = 30,
	IdGL_SAMPLER_1D_SHADOW_ARB           = 31,
	IdGL_SAMPLER_2D                      = 32,
	IdGL_SAMPLER_2D_SHADOW_ARB           = 33,
	IdGL_SAMPLER_3D                      = 34,
	IdGadgetDataset                      = 35,
	IdGadgetSnapshot                     = 36,
	IdGadgetWorkItem                     = 37,
	IdGadgetWorkQueue                    = 38,
	IdGeometry                           = 39,
	IdGlutWindow                         = 40,
	IdGraphicsContext                    = 41,
	IdGwenContext                        = 42,
	IdImageInput                         = 43,
	IdImageOutput                        = 44,
	IdIntensityScale                     = 45,
	IdInterfaceWindow                    = 46,
	IdLogChannel                         = 47,
	IdLogContext                         = 48,
	IdLogFormat                          = 49,
	IdMainThread                         = 50,
	IdMemoryIndex                        = 51,
	IdObject                             = 52,
	IdOpenGLContext                      = 53,
	IdParamSet                           = 54,
	IdParticleColor                      = 55,
	IdParticleDensity                    = 56,
	IdParticleRadius                     = 57,
	IdPointScale                         = 58,
	IdRawFileLog                         = 59,
	IdRuntimeContext                     = 60,
	IdShader                             = 61,
	IdSmoothingRadius                    = 62,
	IdSmoothingScale                     = 63,
	IdSphPointBasedRenderer              = 64,
	IdStreamLog                          = 65,
	IdSymbolRegistry                     = 66,
	IdThread                             = 67,
	IdTiledRenderer                      = 68,
	IdTimer                              = 69,
	IdWdC                                = 70,
	IdWorkItem                           = 71,
	IdWorkQueue                          = 72,
	IdWorkThread                         = 73,
	IdX                                  = 74,
};

// ================================================================================================ //

static const vd::uid
Keys[] = 
{
	vd::uid(0x9257d8e8ed07c6a6ULL, 0x182278a768bf512fULL),
	vd::uid(0xf7a5e1e1b0a43e0fULL, 0xee0f564ed6568c9fULL),
	vd::uid(0x8272f1fea9d93248ULL, 0xe50be92eff9879c1ULL),
	vd::uid(0x2867e0d06cc69100ULL, 0xc52eb489ca5d1203ULL),
	vd::uid(0x6d4d5724ca8621f5ULL, 0xb6014f4d67820d68ULL),
	vd::uid(0x12050b769199541cULL, 0xe9f408022c5fe9d1ULL),
	vd::uid(0x6230e30dfa392b01ULL, 0x42eb3774bbb1594dULL),
	vd::uid(0xe006d4826b43f7dbULL, 0x2a7d42d9b5972e5fULL),
	vd::uid(0x9f654999575a2d52ULL, 0xfc10f14ab67fbad9ULL),
	vd::uid(0xa3da548c49690203ULL, 0x986f0b1a223cc5d8ULL),
	vd::uid(0x804cd2168fcebdf6ULL, 0xd770c8db75aa3dd3ULL),
	vd::uid(0x257ceb25eaf425b4ULL, 0xf01843878f788c19ULL),
	vd::uid(0x32b711a0ae3f8386ULL, 0xf0677852def9680eULL),
	vd::uid(0x5cb490c5d6ba7eedULL, 0x5fbdc4372701f773ULL),
	vd::uid(0x506eb025bdd1adf6ULL, 0x3bc855ac45b7cdccULL),
	vd::uid(0x4f8de78673f628c7ULL, 0x1c6fff5d1ce9188fULL),
	vd::uid(0x849f41753c6a4ad1ULL, 0xd5d515741de499c2ULL),
	vd::uid(0x9034ed59f1e7c0bULL, 0x117984158e6797cULL),
	vd::uid(0x4f6616108e877474ULL, 0x77a6de6a6298af28ULL),
	vd::uid(0xbea38a7f13bccd83ULL, 0x88d01b3b21a14c59ULL),
	vd::uid(0xfb54a513497129bcULL, 0x9fc012db099b95fULL),
	vd::uid(0xfc77ebdc592c4a25ULL, 0xbfe524c97a1e6100ULL),
	vd::uid(0x129150c18d5e2731ULL, 0x98869529ea7a866bULL),
	vd::uid(0x2e47c4d51a1712aULL, 0x2e5737d88d70c47bULL),
	vd::uid(0x8b8e392a5bfe0cfaULL, 0xf12da652b4376d07ULL),
	vd::uid(0xfa97c51475a05a78ULL, 0x63f42193f0fd46b4ULL),
	vd::uid(0x474323f9e55dfd46ULL, 0x5e91adc334607686ULL),
	vd::uid(0x7b1bfa0a7d9c6a48ULL, 0x1f03f87b26dc707aULL),
	vd::uid(0xf7315cf81cb697f3ULL, 0xd04246831ea97a7cULL),
	vd::uid(0x7057b55850765c74ULL, 0x124809be662470b7ULL),
	vd::uid(0x91168490a6c17bfbULL, 0x1e24e2b3557529c2ULL),
	vd::uid(0x52d113913ef68e91ULL, 0x5ad7ea5e03c79b4eULL),
	vd::uid(0x9d35966aab0546aULL, 0xd7708280f99466abULL),
	vd::uid(0x1234c608369737a3ULL, 0x30cd673199b06d08ULL),
	vd::uid(0x9f4084cb41c0351aULL, 0x9b4062c757ff5c02ULL),
	vd::uid(0xf6501d985a469fb9ULL, 0x49e27e704f9b5c2dULL),
	vd::uid(0xd006c19f67d8f1c9ULL, 0x3c6bd6d2c4c7b88fULL),
	vd::uid(0x1be1dea5a25f68ebULL, 0xdf893f5f52792da2ULL),
	vd::uid(0x3499c912f3ba7560ULL, 0x956956440590a658ULL),
	vd::uid(0x77803ec821355be4ULL, 0x4db32050b620b54fULL),
	vd::uid(0xecd7bf0c4671276ULL, 0x6952f5a37acc7faULL),
	vd::uid(0x339ff38f209e31bcULL, 0x4c9c96f31025bd41ULL),
	vd::uid(0xf7149230303400bcULL, 0x1f470fff1ffbb0b1ULL),
	vd::uid(0x597088442f491675ULL, 0xd68f54695c1aa3d7ULL),
	vd::uid(0xabcf53c42812fc39ULL, 0x661d98a8b0a5404fULL),
	vd::uid(0x3b54f992c6735d45ULL, 0x56413b11549e259cULL),
	vd::uid(0x95959fc3e4273398ULL, 0x3ecde25d7a4799c0ULL),
	vd::uid(0xcd234439ca656683ULL, 0x25ebd467a13b049cULL),
	vd::uid(0x86bb7831d33d5c25ULL, 0xd040d67ea1c5c6ceULL),
	vd::uid(0x6b5e533a2e0ef822ULL, 0xb90a00086abfa2dbULL),
	vd::uid(0xd6fc2ae139dea31ULL, 0x88f06e8a635393beULL),
	vd::uid(0xf5fbf743d6c90a84ULL, 0xfe1c2e562001b235ULL),
	vd::uid(0x38de351009f1b563ULL, 0x1cc23e5c8fdd1d2cULL),
	vd::uid(0x755d15cca45e76beULL, 0x33fb62ec7a739967ULL),
	vd::uid(0xd3660f2a4587910ULL, 0xd7f001c31d93cf7ULL),
	vd::uid(0x533c85d0778cb4c0ULL, 0x6e2e452aac49bf94ULL),
	vd::uid(0x902f444ba738e261ULL, 0xb62e24acb57fb34eULL),
	vd::uid(0x5467857f4d23285dULL, 0xa3f637897b7d6098ULL),
	vd::uid(0x7c8bf944104aff2cULL, 0xc5c5a20a782ef4a1ULL),
	vd::uid(0x44dd0d29429301fbULL, 0x72f240f69c598b45ULL),
	vd::uid(0x28db03d1259d66f9ULL, 0xc0cef3de79af561aULL),
	vd::uid(0xa342eff3e70b7cc6ULL, 0x93bc9486ac9ff9f2ULL),
	vd::uid(0xf10ad93e5855bb63ULL, 0xd2abde72cfe1cc0ULL),
	vd::uid(0xefe15dcb11dec185ULL, 0xeb666484834d1bfeULL),
	vd::uid(0x2a24fcecba8f5d36ULL, 0xbee8a95ea69ee7e3ULL),
	vd::uid(0x8266b0c1ae2b74e6ULL, 0xfc64826b002423bcULL),
	vd::uid(0xb76cc6154ca627c3ULL, 0xfa4ada2c621acd88ULL),
	vd::uid(0xe36ddb2774996e9eULL, 0xac5db6abe00a11eaULL),
	vd::uid(0x134f64ef6673aae8ULL, 0xf7c96dd6fb8e4f98ULL),
	vd::uid(0x9b4531040adad9f2ULL, 0xded4ed4b440082e0ULL),
	vd::uid(0xcede3fa2803f46dbULL, 0x2fab4397b79ed7dcULL),
	vd::uid(0xac11f9120897396fULL, 0xd6a52caf1511783cULL),
	vd::uid(0xddcdf775d664febaULL, 0x9f177465443d1d2aULL),
	vd::uid(0x7f7418e1c61a3125ULL, 0x231253563b3cfd3fULL),
	vd::uid(0xc9ddf94e64e9f364ULL, 0x20d3cfa192b26d6cULL)
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
	"CocoaWindow",
	"ColorRange",
	"DefaultLogFormat",
	"Demo",
	"DensityRange",
	"DensityScale",
	"ExposureScale",
	"FileAccess",
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
	"GwenContext",
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
	"X"
};


// ================================================================================================ //

static const vd::symbol
Table[] = 
{
	vd::symbol( Keys[ IdInvalid ], Strings[ IdInvalid ] ),
	vd::symbol( Keys[ IdAlphaScale ], Strings[ IdAlphaScale ] ),
	vd::symbol( Keys[ IdApplication ], Strings[ IdApplication ] ),
	vd::symbol( Keys[ IdBoxSize ], Strings[ IdBoxSize ] ),
	vd::symbol( Keys[ IdBuffer ], Strings[ IdBuffer ] ),
	vd::symbol( Keys[ IdCocoaWindow ], Strings[ IdCocoaWindow ] ),
	vd::symbol( Keys[ IdColorRange ], Strings[ IdColorRange ] ),
	vd::symbol( Keys[ IdDefaultLogFormat ], Strings[ IdDefaultLogFormat ] ),
	vd::symbol( Keys[ IdDemo ], Strings[ IdDemo ] ),
	vd::symbol( Keys[ IdDensityRange ], Strings[ IdDensityRange ] ),
	vd::symbol( Keys[ IdDensityScale ], Strings[ IdDensityScale ] ),
	vd::symbol( Keys[ IdExposureScale ], Strings[ IdExposureScale ] ),
	vd::symbol( Keys[ IdFileAccess ], Strings[ IdFileAccess ] ),
	vd::symbol( Keys[ IdFramebuffer ], Strings[ IdFramebuffer ] ),
	vd::symbol( Keys[ IdGL_BOOL_ARB ], Strings[ IdGL_BOOL_ARB ] ),
	vd::symbol( Keys[ IdGL_BOOL_VEC2_ARB ], Strings[ IdGL_BOOL_VEC2_ARB ] ),
	vd::symbol( Keys[ IdGL_BOOL_VEC3_ARB ], Strings[ IdGL_BOOL_VEC3_ARB ] ),
	vd::symbol( Keys[ IdGL_BOOL_VEC4_ARB ], Strings[ IdGL_BOOL_VEC4_ARB ] ),
	vd::symbol( Keys[ IdGL_FLOAT ], Strings[ IdGL_FLOAT ] ),
	vd::symbol( Keys[ IdGL_FLOAT_MAT2_ARB ], Strings[ IdGL_FLOAT_MAT2_ARB ] ),
	vd::symbol( Keys[ IdGL_FLOAT_MAT3_ARB ], Strings[ IdGL_FLOAT_MAT3_ARB ] ),
	vd::symbol( Keys[ IdGL_FLOAT_MAT4_ARB ], Strings[ IdGL_FLOAT_MAT4_ARB ] ),
	vd::symbol( Keys[ IdGL_FLOAT_VEC2_ARB ], Strings[ IdGL_FLOAT_VEC2_ARB ] ),
	vd::symbol( Keys[ IdGL_FLOAT_VEC3_ARB ], Strings[ IdGL_FLOAT_VEC3_ARB ] ),
	vd::symbol( Keys[ IdGL_FLOAT_VEC4_ARB ], Strings[ IdGL_FLOAT_VEC4_ARB ] ),
	vd::symbol( Keys[ IdGL_INT ], Strings[ IdGL_INT ] ),
	vd::symbol( Keys[ IdGL_INT_VEC2_ARB ], Strings[ IdGL_INT_VEC2_ARB ] ),
	vd::symbol( Keys[ IdGL_INT_VEC3_ARB ], Strings[ IdGL_INT_VEC3_ARB ] ),
	vd::symbol( Keys[ IdGL_INT_VEC4_ARB ], Strings[ IdGL_INT_VEC4_ARB ] ),
	vd::symbol( Keys[ IdGL_INVALID ], Strings[ IdGL_INVALID ] ),
	vd::symbol( Keys[ IdGL_SAMPLER_1D ], Strings[ IdGL_SAMPLER_1D ] ),
	vd::symbol( Keys[ IdGL_SAMPLER_1D_SHADOW_ARB ], Strings[ IdGL_SAMPLER_1D_SHADOW_ARB ] ),
	vd::symbol( Keys[ IdGL_SAMPLER_2D ], Strings[ IdGL_SAMPLER_2D ] ),
	vd::symbol( Keys[ IdGL_SAMPLER_2D_SHADOW_ARB ], Strings[ IdGL_SAMPLER_2D_SHADOW_ARB ] ),
	vd::symbol( Keys[ IdGL_SAMPLER_3D ], Strings[ IdGL_SAMPLER_3D ] ),
	vd::symbol( Keys[ IdGadgetDataset ], Strings[ IdGadgetDataset ] ),
	vd::symbol( Keys[ IdGadgetSnapshot ], Strings[ IdGadgetSnapshot ] ),
	vd::symbol( Keys[ IdGadgetWorkItem ], Strings[ IdGadgetWorkItem ] ),
	vd::symbol( Keys[ IdGadgetWorkQueue ], Strings[ IdGadgetWorkQueue ] ),
	vd::symbol( Keys[ IdGeometry ], Strings[ IdGeometry ] ),
	vd::symbol( Keys[ IdGlutWindow ], Strings[ IdGlutWindow ] ),
	vd::symbol( Keys[ IdGraphicsContext ], Strings[ IdGraphicsContext ] ),
	vd::symbol( Keys[ IdGwenContext ], Strings[ IdGwenContext ] ),
	vd::symbol( Keys[ IdImageInput ], Strings[ IdImageInput ] ),
	vd::symbol( Keys[ IdImageOutput ], Strings[ IdImageOutput ] ),
	vd::symbol( Keys[ IdIntensityScale ], Strings[ IdIntensityScale ] ),
	vd::symbol( Keys[ IdInterfaceWindow ], Strings[ IdInterfaceWindow ] ),
	vd::symbol( Keys[ IdLogChannel ], Strings[ IdLogChannel ] ),
	vd::symbol( Keys[ IdLogContext ], Strings[ IdLogContext ] ),
	vd::symbol( Keys[ IdLogFormat ], Strings[ IdLogFormat ] ),
	vd::symbol( Keys[ IdMainThread ], Strings[ IdMainThread ] ),
	vd::symbol( Keys[ IdMemoryIndex ], Strings[ IdMemoryIndex ] ),
	vd::symbol( Keys[ IdObject ], Strings[ IdObject ] ),
	vd::symbol( Keys[ IdOpenGLContext ], Strings[ IdOpenGLContext ] ),
	vd::symbol( Keys[ IdParamSet ], Strings[ IdParamSet ] ),
	vd::symbol( Keys[ IdParticleColor ], Strings[ IdParticleColor ] ),
	vd::symbol( Keys[ IdParticleDensity ], Strings[ IdParticleDensity ] ),
	vd::symbol( Keys[ IdParticleRadius ], Strings[ IdParticleRadius ] ),
	vd::symbol( Keys[ IdPointScale ], Strings[ IdPointScale ] ),
	vd::symbol( Keys[ IdRawFileLog ], Strings[ IdRawFileLog ] ),
	vd::symbol( Keys[ IdRuntimeContext ], Strings[ IdRuntimeContext ] ),
	vd::symbol( Keys[ IdShader ], Strings[ IdShader ] ),
	vd::symbol( Keys[ IdSmoothingRadius ], Strings[ IdSmoothingRadius ] ),
	vd::symbol( Keys[ IdSmoothingScale ], Strings[ IdSmoothingScale ] ),
	vd::symbol( Keys[ IdSphPointBasedRenderer ], Strings[ IdSphPointBasedRenderer ] ),
	vd::symbol( Keys[ IdStreamLog ], Strings[ IdStreamLog ] ),
	vd::symbol( Keys[ IdSymbolRegistry ], Strings[ IdSymbolRegistry ] ),
	vd::symbol( Keys[ IdThread ], Strings[ IdThread ] ),
	vd::symbol( Keys[ IdTiledRenderer ], Strings[ IdTiledRenderer ] ),
	vd::symbol( Keys[ IdTimer ], Strings[ IdTimer ] ),
	vd::symbol( Keys[ IdWdC ], Strings[ IdWdC ] ),
	vd::symbol( Keys[ IdWorkItem ], Strings[ IdWorkItem ] ),
	vd::symbol( Keys[ IdWorkQueue ], Strings[ IdWorkQueue ] ),
	vd::symbol( Keys[ IdWorkThread ], Strings[ IdWorkThread ] ),
	vd::symbol( Keys[ IdX ], Strings[ IdX ] )
};

// ================================================================================================ //

} // end namespace: Demo
} // end namespace: Symbols

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_END();

// ================================================================================================ //

#endif // VD_CONSTANTS_EXTRACTED_DEMO_SYMBOLS_SCOPE

