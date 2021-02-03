/*
Language Abstraction Layer

(Previously termed as "C-Assist"
*/



#pragma once



#include "C_STL.hpp"
#include "Cpp_STL.hpp"



// Macros (Preprocessed Definitions) ---------------------------------------------------------------

// Specifiers

#define CompileTime constexpr

//#define override virtual


// Memory

/*
Used for defining a non-compile time constant.
Where something is really just said to not be changeable instead of being a true constant.
const are put in read only memory.
*/
#define ro \
const

// Stores the address for a data instance of the specified type. (Pointer declaration, using indirection operator)
//#define Ptr(_type) 
//_type*

//Provides a more explicit method for getting a reference.
//#define getPtr(_instance) 
//&_instance

// Gets the data instance from the specified address (AKA Dereference, using indirection operator)
//#define dref(_address) 
//*_address

// Creates a static duration variable accessible to any file or linkage. (Global/File scope Only)
#define foreign \
extern

// Define a internal linkage variable accessible to only the file or linkage.
#define noLinkage \
static

//
#define unbound \
static

//////////////////////////////////// End of Macros   ///////////////////////////////////////////////



// Aliases (Typedefs) ------------------------------------------------------------------------------

using Byte = char; 

using sInt16 = signed           short;
using sInt64 = signed long long int  ;

using uInt   = unsigned            int;
using uInt16 = unsigned short      int;
using uInt64 = unsigned long  long int;

using sInt = signed int;

using Key = char;

using float32 = float;
using float64 = double;


template<typename Type>
using ptr = Type*;


using uIntDM = size_t;   // Data Model Unsigned Integer

using Void_Function = void();

////////////////////////////////////////////////////////////////////////////////////////////////////



// Indirection

template<typename Type>
ptr<Type> getPtr(Type& _ref)
{
	return &_ref;
}

template<typename Type>
Type& dref(ptr<Type> _ptr)
{
	return *_ptr;
}


// Casting

template<typename Derived, typename Base>
Derived RCast(Base _obj)
{
	return reinterpret_cast<Derived>(_obj);
}

template<typename Derived, typename Base>
ptr<Derived> RCast(ptr<Base> _ref)
{
	return reinterpret_cast< ptr<Derived>>(_ref);
}

template<typename Derived, typename Base>
Derived SCast(Base _obj)
{
	return static_cast<Derived>(_obj);
}

template<typename Derived, typename Base>
ptr<Derived> SCast(ptr<Base> _ref)
{
	return static_cast< ptr<Derived>>(_ref);
}
