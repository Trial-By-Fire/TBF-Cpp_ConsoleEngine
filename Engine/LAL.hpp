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
//#define ro
//const

// Creates a static duration variable accessible to any file or linkage. (Global/File scope Only)
#define ExportLinkage \
extern

// Define a internal linkage variable accessible to only the file or linkage.
#define NoLinkage \
static

//
//#define unbound
//static

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

template<bool Constraint, typename ReturnType>
using Where = typename std::enable_if<Constraint, ReturnType>::type;

template<typename Type>
constexpr 
bool IsWithinDataModelSize()
{
	return sizeof(Type) <= sizeof(uIntDM);
}

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

/*
Reinterpret Cast (Pointer)
*/
template<typename Derived, typename Base> 
constexpr
Derived& RCast(Base& _ptr)
{
	return reinterpret_cast< Derived& >(_ptr);
}

/*
Reinterpret Cast (Pointer)
*/
template<typename Derived, typename Base> 
constexpr
Derived* RCast(Base* _ptr)
{
	return reinterpret_cast< Derived* >(_ptr);
}

/*
Static Cast (Value)
*/
template<typename Derived, typename Base>
constexpr 
Where<IsWithinDataModelSize<Base>(),
Derived> SCast(Base _obj)
{
	return static_cast<Derived>(_obj);
}

/*
Static Cast (Reference)
*/
template<typename Derived, typename Base> 
constexpr
Where<! IsWithinDataModelSize<Base>(),
Derived&>  SCast(Base& _obj)
{
	return static_cast< Derived& >(_obj);
}

/*
Static Cast (Pointer)
*/
template<typename Derived, typename Base> 
constexpr 
Derived* SCast(Base* _ptr)
{
	return static_cast< Derived* >(_ptr);
}


ptr<const WChar> operator""_wc(ptr<const wchar_t> _um, uIntDM _umSize);


template<uIntDM NumberOfBits>
using Bitset = std::bitset<NumberOfBits>;

#pragma region BitmaskableTraits

template<typename Enum, typename = void>
/**
@brief Used when the enum does not meet the criteria for bitmaskable.
*/
struct IsBitmaskable : std::false_type
{};

#define HasMemberSymbol(__MEMBER_SYMBOL)\
	decltype(static_cast<void>(__MEMBER_SYMBOL))

template<typename Enum>
/**
@brief Will be defined with a true_type when enum has the SpecifyBitmaskable enum value.
*/
struct IsBitmaskable<Enum, HasMemberSymbol(Enum::SpecifyBitmaskable)> : std::is_enum<Enum>
{};

template <typename Enum>
/**
@brief Returns true if IsBitmaskable is false.
*/
constexpr Where<IsBitmaskable<Enum>::value, 
bool> Bitmaskable() noexcept
{
	// Make sure enum member is greater than 0 (as it was supposed to be one of the last if not last entry).
	return SCast<uIntDM>(Enum::SpecifyBitmaskable) > uIntDM(0) ? true : false;
}

template <typename Enum> 
/**
@brief Returns false if bitmaskable is false (Default case).
*/
constexpr Where<! IsBitmaskable<Enum>::value, 
bool> Bitmaskable() noexcept
{
	return false;
}

#pragma endregion BitmaskableTraits

template
<
	typename EnumType             ,
	typename BitmaskRepresentation
>
/**
A wrapper object for bitfields that allows for typesafe bitmask operations.
*/
class Bitfield
{
private:
	static_assert(Bitmaskable<EnumType>(), "EnumType must be of Bitmaskable type.");

	using _ThisType = Bitfield<EnumType, BitmaskRepresentation>;

public:

	using Enum           = EnumType             ;
	using Representation = BitmaskRepresentation;

	static constexpr uIntDM NumBits = sizeof(Representation) * 8;

	Bitfield() : bitfield(0) {}

	Bitfield(Representation _mask) : bitfield(_mask)
	{}

	template<typename... BitTypes>
	constexpr
	Bitfield(const BitTypes... _bits) : bitfield(0)
	{
		bitfield = (Representation(_bits) | ...);
	}

	template<typename... BitType>
	inline
	void Add(const BitType... _bits)
	{
		bitfield |= (Representation(_bits) | ...);
	}

	template<typename... BitType>
	inline
	bool CheckForEither(const BitType... _bits) const
	{
		return (bitfield & (Representation(_bits) | ...)) != 0;
	}

	template<typename... BitType>
	inline
	void Clear(const BitType... _bits)
	{
		if (bitfield <= 0) 
			return;

		bitfield &= ~(Representation(_bits) | ...);
	}

	inline
	bool HasFlag(const Enum _bit) const
	{
		return (bitfield & Representation(_bit)) == Representation(_bit);
	}

	template<typename... BitType>
	inline
	bool HasExactly(const BitType... _bits) const
	{
		return (bitfield & (Representation(_bits) | ...)) == bitfield;
	}

	inline
	bool HasAnyFlag() const 
	{
		return bitfield != 0 ? true : false; 
	}

	inline
	bool IsZero() const 
	{
		return bitfield == 0 ? true : false; 
	}	

	inline
	void Reset() 
	{
		bitfield = 0; 
	}

	template<typename... BitType>
	inline
	void Set(const BitType... _bits)
	{
		bitfield = (Representation(_bits) | ...);
	}

	template<typename... BitType>
	inline
	void Toggle(const BitType... _bits)
	{
		bitfield ^= (Representation(_bits) | ...);
	}

	operator Representation() const 
	{
		return bitfield; 
	}

	operator Bitset<NumBits>()
	{
		return bitfield;
	}

	String ToString()
	{
		std::stringstream stream;

		stream << Bitset<NumBits>(bitfield);

		return stream.str();
	}

	_ThisType& operator= (const Representation _mask ) { bitfield = _mask; return *this; }
	_ThisType& operator= (const _ThisType      _other) { bitfield = _other.bitfield; return *this; }

	_ThisType& operator&= (const Representation _mask ) { bitfield &= _mask; return *this; }
	_ThisType& operator&= (const _ThisType      _other) { bitfield &= _other.bitfield; return *this; }

	_ThisType& operator|= (const Representation _mask ) { bitfield |= _mask; return *this; }
	_ThisType& operator|= (const _ThisType      _other) { bitfield |= _other.bitfield; return *this; }	

	_ThisType& operator^= (const Representation _mask ) { bitfield ^= _mask; return *this; }
	_ThisType& operator^= (const _ThisType      _other) { bitfield ^= _other.bitfield; return *this; }	

	_ThisType& operator<<= (const Representation _mask ) { bitfield <<= _mask; return *this; }
	_ThisType& operator>>= (const _ThisType      _other) { bitfield >>= _other.bitfield; return *this; }	

	_ThisType operator~ () const { return ~bitfield; }

	Representation operator& (const Representation _other) const { return bitfield & _other; }
	_ThisType      operator& (const _ThisType      _other) const { return bitfield & _other.bitfield; }

	Representation operator| (const Representation _other) const { return bitfield | _other; }
	_ThisType      operator| (const _ThisType      _other) const { return bitfield | _other.bitfield; }

	Representation operator^ (const Representation _other) const { return bitfield ^ _other; }
	_ThisType      operator^ (const _ThisType      _other) const { return bitfield ^ _other.bitfield; }

	Representation operator<< (const Representation _other) const { return bitfield << _other; }
	_ThisType      operator>> (const _ThisType      _other) const { return bitfield >> _other.bitfield; }

	bool operator== (const Representation _other) const { return bitfield == _other; }
	bool operator== (const _ThisType      _other) const { return bitfield == _other.bitfield; }

	bool operator!= (const Representation _other) const { return bitfield != _other; }
	bool operator!= (const _ThisType      _other) const { return bitfield != _other.bitfield; }

private:

	Representation bitfield;
};
