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
using uIntDM = size_t                 ;   // Data Model Unsigned Integer

using sInt = signed int;

using Key = char;

using float32 = float;
using float64 = double;

using WChar = wchar_t;

using Void_Function = void();

template<typename Type, uIntDM _size> using StaticArray  = std::array <Type, _size>;
template<typename Type              > using DynamicArray = std::vector<Type       >;

using String  = std::string;
using WString = std::wstring;

template<typename Type> String  ToString (Type _obj) { return std::to_string (_obj); };
template<typename Type> WString ToWString(Type _obj) { return std::to_wstring(_obj); };

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

template<typename Type>
using NumLimits = std::numeric_limits<Type>;

using Exception    = std::exception;
using RuntimeError = std::runtime_error;

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

template<typename Type>
using uptr = std::unique_ptr<Type>;

template<typename Type, class... ArgumentsTypes>
uptr<Type> MakeUPtr(ArgumentsTypes&&... _arguments)
{
	return std::make_unique<Type>(_arguments...);
}


// Indirection

template<typename Type>
using ptr = Type*;


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


ptr<ro WChar> operator""_wc(ptr<ro wchar_t> _um, uIntDM _umSize);



template<typename Enum, typename = void>
/**
@brief Used when the enum does not meet the criteria for bitmaskable.
*/
struct IsBitmaskable : std::false_type
{};

template<typename Enum>
/**
@brief Will be defined with a true_type when enum has the SpecifyBitmaskable enum value.
*/
struct IsBitmaskable<Enum, decltype(static_cast<void>(Enum::SpecifyBitmaskable))> : std::is_enum<Enum>
{};

template <typename Enum>
/**
@brief Returns true if IsBitmaskable is false.
*/
constexpr typename std::enable_if<IsBitmaskable<Enum>::value, bool>::
type Bitmaskable() noexcept
{
	return static_cast<std::size_t>(Enum::SpecifyBitmaskable) > std::size_t(0) ? true : false;
}

template <typename Enum> 
/**
@brief Returns false if bitmaskable is false (Default case).
*/
constexpr typename std::enable_if<!IsBitmaskable<Enum>::value, bool>::
type Bitmaskable() noexcept
{
	return false;
}

template
<
	typename EnumType             ,
	typename BitmaskRepresentation
>
class Bitfield
{
private:
	static_assert(Bitmaskable<EnumType>(), "EnumType must be of Bitmaskable type.");

	using _ThisType = Bitfield<EnumType, BitmaskRepresentation>;

public:

	using Enum           = EnumType             ;
	using Representation = BitmaskRepresentation;

	Bitfield() : mask(0) {}

	CompileTime Bitfield(Representation _mask) : mask(_mask)
	{}

	template<typename... BitTypes>
	CompileTime Bitfield(const BitTypes... _bits) : mask(0)
	{
		mask = (Representation(_bits) | ...);
	}

	template<typename... BitType>
	void Add(const BitType... _bits)
	{
		mask |= (Representation(_bits) | ...);
	}

	template<typename... BitType>
	bool CheckForEither(const BitType... _bits) const
	{
		return (mask & (Representation(_bits) | ...)) != 0;
	}

	template<typename... BitType>
	void Clear(const BitType... _bits)
	{
		if (mask <= 0) return;

		mask &= ~(Representation(_bits) | ...);
	}

	bool HasFlag(const Enum _bit) const
	{
		return (mask & Representation(_bit)) == Representation(_bit);
	}

	template<typename... BitType>
	bool HasExactly(const BitType... _bits) const
	{
		return (mask & (Representation(_bits) | ...)) == mask;
	}

	bool HasAnyFlag() const { return mask != 0 ? true : false; }
	bool IsZero    () const { return mask == 0 ? true : false; }	

	void Reset() { mask = 0; }

	template<typename... BitType>
	void Set(const BitType... _bits)
	{
		mask = (Representation(_bits) | ...);
	}

	template<typename... BitType>
	void Toggle(const BitType... _bits)
	{
		mask ^= (Representation(_bits) | ...);
	}

	operator Representation() const { return mask; }

	_ThisType& operator= (const Representation _mask ) { mask = _mask      ; return *this; }
	_ThisType& operator= (const _ThisType      _other) { mask = _other.mask; return *this; }

	_ThisType& operator&= (const Representation _mask ) { mask &= mask       ; return *this; }
	_ThisType& operator&= (const _ThisType      _other) { mask &= _other.mask; return *this; }

	_ThisType& operator|= (const Representation _mask ) { mask |= mask       ; return *this; }
	_ThisType& operator|= (const _ThisType      _other) { mask |= _other.mask; return *this; }	

	_ThisType& operator^= (const Representation _mask ) { mask ^= mask       ; return *this; }
	_ThisType& operator^= (const _ThisType      _other) { mask ^= _other.mask; return *this; }	

	_ThisType& operator<<= (const Representation _mask ) { mask <<= mask       ; return *this; }
	_ThisType& operator>>= (const _ThisType      _other) { mask >>= _other.mask; return *this; }	

	_ThisType operator~ () const { return ~mask; }

	Representation operator& (const Representation _other) const { return mask & _other     ; }
	_ThisType      operator& (const _ThisType      _other) const { return mask & _other.mask; }

	Representation operator| (const Representation _other) const { return mask | _other     ; }
	_ThisType      operator| (const _ThisType      _other) const { return mask | _other.mask; }

	Representation operator^ (const Representation _other) const { return mask ^ _other     ; }
	_ThisType      operator^ (const _ThisType      _other) const { return mask ^ _other.mask; }

	Representation operator<< (const Representation _other) const { return mask << _other     ; }
	_ThisType      operator>> (const _ThisType      _other) const { return mask >> _other.mask; }

	bool operator== (const Representation _other) const { return mask == _other     ; }
	bool operator== (const _ThisType      _other) const { return mask == _other.mask; }

	bool operator!= (const Representation _other) const { return mask != _other     ; }
	bool operator!= (const _ThisType      _other) const { return mask != _other.mask; }

private:

	Representation mask;
};
