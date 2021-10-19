#pragma once



#include "LAL.hpp"



// Remove this definition if debug build is undesired.
//#define BuildDebug


#ifdef BuildDebug

	constexpr bool DebugEnabled = true;

#else

	constexpr bool DebugEnabled = false;

#endif
