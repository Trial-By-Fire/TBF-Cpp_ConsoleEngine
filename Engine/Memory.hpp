#pragma once



// Includes

#include "vld.h"
#include "LAL.hpp"



class Memory
{
public:

	using Block      = DynamicArray<Byte >;
	using BlockArray = DynamicArray<Block>;

	~Memory() {};


	// Functions

	// C-API

	template<typename Type> unbound ptr<Type> HeapAllocate  (uIntDM    _numberToAllocate                                              );
	template<typename Type>	unbound ptr<Type> HeapReallocate(ptr<Type> _memoryToReallocate, uIntDM _originalNum, uIntDM _numberDesired);
	                        unbound void      Deallocate    (ptr<void> _memoryToDeallocate                                            );

	template<typename Type>	unbound ptr<void> FormatByFill  (ptr<Type> _memoryAddress, sInt         _fillValue , uIntDM _sizeOfData);
	template<typename Type> unbound ptr<void> FormatWithData(ptr<Type> _memoryAddress, ptr<ro Type> _dataSource, uIntDM _sizeOfData);

	// Memory Management

	template<typename Type>
	ptr<Type> Allocate(uIntDM _numberToAllocate);

	template<typename Type> unbound ptr<Type> GlobalAllocate  (                     uIntDM _sizeOfAllocation   );
	template<typename Type> unbound ptr<Type> GlobalReallocate(ptr<Type> _location, uIntDM _sizeForReallocation);


private:

	BlockArray records;

	unbound Memory GlobalMemory;
};



// Template Implementation
#include "Memory.tpp"
