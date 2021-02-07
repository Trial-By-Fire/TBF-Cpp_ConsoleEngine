#pragma once



#include "LAL.hpp"



// Remove this definition if debug build is undesired.
//#define BuildDebug


#ifdef BuildDebug

	CompileTime bool DebugEnabled = true;

#else

	CompileTime bool DebugEnabled = false;

#endif
