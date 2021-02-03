#pragma once



// Includes

#include "LAL.hpp"



class Memory
{
public:

	// Structures

	struct BlockArray
	{
		struct Block
		{
			ptr<void> Location = nullptr;

			uIntDM Size = 0;
		};


		~BlockArray();


		// Functions

		Block& Add       ();
		Block& LastEntry ();


		// Variables

		ptr< ptr<Block>> Array = nullptr;

		uIntDM Length = 0;
	};



	// Functions

	// C-API

	template<typename Type> unbound ptr<Type> HeapAllocate  (uIntDM    _numberToAllocate                         );
	template<typename Type>	unbound ptr<Type> HeapReallocate(ptr<Type> _memoryToReallocate, uIntDM _numberDesired);
	                        unbound void      Deallocate    (ptr<void> _memoryToDeallocate                       );

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



// Operator Overloads

ptr<void> operator new(uIntDM _numberDesired);



// Template Implementation
#include "Memory.tpp"
