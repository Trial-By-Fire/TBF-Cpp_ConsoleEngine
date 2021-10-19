#pragma once



// Includes

#include "LAL.hpp"



class Memory
{
public:

	using Block      = DynamicArray<Byte >;
	using BlockArray = DynamicArray<Block>;



	// Functions

	// C-API

	template<typename Type> static ptr<Type> HeapAllocate  (uIntDM    _numberToAllocate                                              );
	template<typename Type>	static ptr<Type> HeapReallocate(ptr<Type> _memoryToReallocate, uIntDM _originalNum, uIntDM _numberDesired);
	                        static void      Deallocate    (ptr<void> _memoryToDeallocate                                            );

	template<typename Type>	static ptr<void> FormatByFill  (ptr<Type> _memoryAddress, Type&        _fillValue , uIntDM _sizeOfData);
	template<typename Type> static ptr<void> FormatWithData(ptr<Type> _memoryAddress, ptr<const Type> _dataSource, uIntDM _sizeOfData);

	// Memory Management

	template<typename Type>
	ptr<Type> Allocate(uIntDM _numberToAllocate);

	template<typename Type> static ptr<Type> GlobalAllocate  (                     uIntDM _sizeOfAllocation   );
	template<typename Type> static ptr<Type> GlobalReallocate(ptr<Type> _location, uIntDM _sizeForReallocation);


private:

	BlockArray records;

	static Memory GlobalMemory;
};



// Template Implementation
#include "Memory.tpp"
