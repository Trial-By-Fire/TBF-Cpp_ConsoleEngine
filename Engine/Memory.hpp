#pragma once


// Includes

#include "LAL.hpp"
#include "CString.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Timing.hpp"



class Memory
{
public:

	// Aliases (Typedefs)

	using Address = void*;



	// Structures

	struct BlockArray
	{
		struct Block
		{
			Address Location = nullptr;

			uIntDM Size = 0;
		};

		void   Add       (Block* _memoryAllocation);
		Block& LastEntry ();
		void   Deallocate();


		Block** Array = nullptr;

		uIntDM Length = 0;
	};



	~Memory();



	// Functions


	// C-API

	unbound void* Allocate  (uIntDM _amountToAllocate  );
	unbound void  Deallocate(void*  _memoryToDeallocate);

	unbound void* Internal_FormatByFill(void* _memoryAddress,    sInt  _fillValue,  uIntDM _sizeOfData);
	unbound void* FormatWithData       (void* _memoryAddress, ro void* _dataSource, uIntDM _sizeOfData);

	// Memory Management

	Address Internal_Allocate(uIntDM _sizeOfAllocation);
	void    Deallocate       ();

	unbound Address Internal_GlobalAllocate  (                   uIntDM _sizeOfAllocation   );
	unbound Address Internal_GlobalReallocate(Address _location, uIntDM _sizeForReallocation);
	unbound void    GlobalDeallocate         (void                                          );


private:

	BlockArray records = { nullptr, 0 };



	// Macros

	#define GlobalAllocate(_type, _numberToAllocate) \
	Memory::Internal_GlobalAllocate(sizeof(_type) * _numberToAllocate)

	#define GlobalReallocate(_type, _address, _numberToReallocate) \
	Memory::Internal_GlobalReallocate(_address, sizeof(_type) * _numberToReallocate);

	#define ScopedAllocate(_type, _numberToAllocate)  \
	scopedMemory.Internal_Allocate(sizeof(_type) * _numberToAllocate)

	#define Memory_FormatByFill(_type, _memoryAddress, _fillValue, _sizeOfAllocation) \
	Memory::Internal_FormatByFill(_memoryAddress, _fillValue, sizeof(_type) * _sizeOfAllocation);

	#define SmartScope                    \
	{					                  \
		Memory scopedMemory;

	#define SmartScope_End \
	}
};

