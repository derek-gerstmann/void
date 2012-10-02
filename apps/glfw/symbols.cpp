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

const size_t Static::Count = 4UL;

// ================================================================================================ //

const vd::uid
Static::Keys[4] = 
{
	vd::uid(0x9257d8e8ed07c6a6ULL, 0x182278a768bf512fULL),
	vd::uid(0x8272f1fea9d93248ULL, 0xe50be92eff9879c1ULL),
	vd::uid(0x9f654999575a2d52ULL, 0xfc10f14ab67fbad9ULL),
	vd::uid(0x1924f97baf4ab65ULL, 0xe57d51957a5153d3ULL)
};

// ================================================================================================ //

const char*
Static::Strings[4] = 
{
	"Invalid",
	"Application",
	"Demo",
	"UseGlfw"
};


// ================================================================================================ //

const vd::symbol
Static::Table[4] = 
{
	vd::symbol( Static::Keys[ Static::IdInvalid ], Static::Strings[ Static::IdInvalid ] ),
	vd::symbol( Static::Keys[ Static::IdApplication ], Static::Strings[ Static::IdApplication ] ),
	vd::symbol( Static::Keys[ Static::IdDemo ], Static::Strings[ Static::IdDemo ] ),
	vd::symbol( Static::Keys[ Static::IdUseGlfw ], Static::Strings[ Static::IdUseGlfw ] )
};

// ================================================================================================ //

} // end namespace: Demo
} // end namespace: Symbols

// ================================================================================================ //

VD_CONSTANTS_NAMESPACE_END();

// ================================================================================================ //

