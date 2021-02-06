/*
Language Abstraction Layer

(Previously termed as "C-Assist"
*/



#pragma once



#include "Cpp_STL.hpp"



// Macros (Preprocessed Definitions) ---------------------------------------------------------------

// Specifiers

#define CompileTime constexpr

// Memory

/*
Used for defining a non-compile time constant.
Where something is really just said to not be changeable instead of being a true constant.
const are put in read only memory.
*/
#define ro \
const

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

template<typename Type>
using uptr = std::unique_ptr<Type>;


template<typename Type, class... ArgumentsTypes>
uptr<Type> MakeUPtr(ArgumentsTypes&&... _arguments)
{
	return std::make_unique<Type>(_arguments...);
}


using uIntDM = size_t;   // Data Model Unsigned Integer

using Void_Function = void();


template<typename Type>
using DynamicArray = std::vector<Type>;


using CString_13   = char[13];
using CTS_CString  = char[];
using WideChar     = wchar_t;
using CTS_CWString = wchar_t[];

using String  = std::string;
using WString = std::wstring;

//template<typename Type>
//String ToString(Type _obj) { return std::to_String(_obj): };

inline String ToString(int                _val) { return std::to_string(_val); };
inline String ToString(long               _val) { return std::to_string(_val); };
inline String ToString(long long          _val) { return std::to_string(_val); };
inline String ToString(unsigned           _val) { return std::to_string(_val); };
inline String ToString(unsigned long      _val) { return std::to_string(_val); };
inline String ToString(unsigned long long _val) { return std::to_string(_val); };
inline String ToString(float              _val) { return std::to_string(_val); };
inline String ToString(double             _val) { return std::to_string(_val); };
inline String ToString(long double        _val) { return std::to_string(_val); };

template<typename Type>
WString ToWString(Type _obj) { return std::to_wstring(_obj); };

using StringStream  = std::stringstream;
using WStringStream = std::wstringstream;

template<typename Type>
using NumLimits = std::numeric_limits<Type>;


using Exception = std::exception;

using RuntimeError = std::runtime_error;

using std::cout;
using std::cerr;
using std::cin;

using std::endl;


using SystemClock      = std::chrono::system_clock;
using SystemTimePeriod = SystemClock::period      ;
using SysTimePoint     = SystemClock::time_point  ;

using SteadyClock      = std::chrono::steady_clock;
using SteadyTimePeriod = SteadyClock::period      ;
using SteadyTimePoint  = SteadyClock::time_point  ;

using HighResClock      = std::chrono::high_resolution_clock;
using HighResTimePeriod = HighResClock::period              ;
using HighResTimePoint  = HighResClock::time_point          ;


template<class RepresentiveType, class Period = std::ratio<1>>
using Duration = std::chrono::duration<RepresentiveType, Period>;

using Duration64 = Duration<float64>;

using CalendarDate = std::tm;

using Microseconds = std::chrono::microseconds;
using Milliseconds = std::chrono::milliseconds;
using Miliseconds  = std::chrono::nanoseconds ;

////////////////////////////////////////////////////////////////////////////////////////////////////


// Memory

using std::fill_n;

using std::copy_n;



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


ptr<ro WideChar> operator""_wc(ptr<ro wchar_t> _um, uIntDM _umSize);
