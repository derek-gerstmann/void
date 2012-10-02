#ifndef VD_CONSTANTS_EXTRACTED_GLFW_SYMBOLS_SCOPE
#define VD_CONSTANTS_EXTRACTED_GLFW_SYMBOLS_SCOPE

// ================================================================================================ //

#include "vd.h"
#include "constants/constants.h"

// ================================================================================================ //

#ifndef VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
#define VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE	VD_CONSTANTS_NAMESPACE::Symbols::Demo
#endif // VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_BEGIN();

// ================================================================================================ //

namespace Symbols {
namespace Demo {

// ================================================================================================ //

struct VD_API Static {

// ================================================================================================ //

static const size_t Count;

enum Id
{
	IdInvalid                            = 0,
	IdApplication                        = 1,
	IdDemo                               = 2,
	IdUseGlfw                            = 3,
};

static const vd::uid Keys[4];

static const char* Strings[4];

static const vd::symbol Table[4];


// ================================================================================================ //

}; // end struct: Static

// ================================================================================================ //

} // end namespace: Demo
} // end namespace: Symbols

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_END();

// ================================================================================================ //

#endif // VD_CONSTANTS_EXTRACTED_GLFW_SYMBOLS_SCOPE

