#pragma once


// Includes

#include "LAL.hpp"
#include "CString.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Timing.hpp"



namespace Memory
{
	// Public



	// Aliases (Typedefs)

	using Address = void*;



	// Structures

	struct Block
	{
		Address Location;

		uIntDM Size;
	};

	struct BlockArray
	{
		Block** Array;

		uIntDM Length;
	};



	// Functions

	// C-API

	void* AllocateMemory(uIntDM _amountToAllocate  );
	void  Deallocate    (void*  _memoryToDeallocate);

	void* Internal_FormatByFill(void* _memoryAddress,    sInt  _fillValue,  uIntDM _sizeOfData);
	void* FormatWithData       (void* _memoryAddress, ro void* _dataSource, uIntDM _sizeOfData);

	// Memory Allocation Array

	void   BlockArray_Add       (BlockArray* _memoryArray, Block* _memoryAllocation);
	Block* BlockArray_LastEntry (BlockArray* _memoryArray                          );
	void   BlockArray_Deallocate(BlockArray* _memoryArray                          );

	// Memory Management

	Address Internal_ScopedAllocate(BlockArray* _scopedAllocations, uIntDM _sizeOfAllocation);
	void    ScopedDeallocate       (BlockArray* _scopedAllocations                          );

	Address Internal_GlobalAllocate  (                   uIntDM _sizeOfAllocation   );
	Address Internal_GlobalReallocate(Address _location, uIntDM _sizeForReallocation);
	void    GlobalDeallocate         (void                                          );



	// Macros

	#define GlobalAllocate(_type, _numberToAllocate) \
	Memory::Internal_GlobalAllocate(sizeof(_type) * _numberToAllocate)

	#define GlobalReallocate(_type, _address, _numberToReallocate) \
	Memory::Internal_GlobalReallocate(_address, sizeof(_type) * _numberToReallocate);

	#define ScopedAllocate(_type, _numberToAllocate)  \
	Memory::Internal_ScopedAllocate(&scopedMemory, sizeof(_type) * _numberToAllocate)

	#define Memory_FormatByFill(_type, _memoryAddress, _fillValue, _sizeOfAllocation) \
	Memory::Internal_FormatByFill(_memoryAddress, _fillValue, sizeof(_type) * _sizeOfAllocation);

	#define SmartScope                    \
	{					                  \
		Memory::BlockArray scopedMemory = \
		{ nullptr, 0U };


	#define SmartScope_End                           \
		if (scopedMemory.Array != nullptr)              \
		{								             \
			Memory::ScopedDeallocate(&scopedMemory); \
		}											 \
	}
}

