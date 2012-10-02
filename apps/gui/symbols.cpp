#include "symbols.h"

// ================================================================================================ //

#include "vd.h"
#include "constants/constants.h"

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_BEGIN();

// ================================================================================================ //

namespace Symbols {
namespace Demo {

// ================================================================================================ //

const size_t Static::Count = 3UL;

// ================================================================================================ //

const vd::uid
Static::Keys[3] = 
{
	vd::uid(0x9257d8e8ed07c6a6ULL, 0x182278a768bf512fULL),
	vd::uid(0x8272f1fea9d93248ULL, 0xe50be92eff9879c1ULL),
	vd::uid(0x9f654999575a2d52ULL, 0xfc10f14ab67fbad9ULL)
};

// ================================================================================================ //

const char*
Static::Strings[3] = 
{
	"Invalid",
	"Application",
	"Demo"
};


// ================================================================================================ //

const vd::symbol
Static::Table[3] = 
{
	vd::symbol( Static::Keys[ Static::IdInvalid ], Static::Strings[ Static::IdInvalid ] ),
	vd::symbol( Static::Keys[ Static::IdApplication ], Static::Strings[ Static::IdApplication ] ),
	vd::symbol( Static::Keys[ Static::IdDemo ], Static::Strings[ Static::IdDemo ] )
};

// ================================================================================================ //

} // end namespace: Demo
} // end namespace: Symbols

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_END();

// ================================================================================================ //

