// Parent Header
#include "Memory.hpp"



using BlockArray = Memory::BlockArray;
using Block      = BlockArray::Block;



// Static Data

// Private

Memory Memory::GlobalMemory;



// Functions

// Public

void Memory::Deallocate(ptr<void> _memoryToDeallocate)
{
	free(_memoryToDeallocate);
}



// Block Array

BlockArray::~BlockArray()
{
	if (Array == nullptr) 
	{
		return;
	}

	for (uIntDM index = 0; index < Length; index++)
	{
		Deallocate(Array[index]->Location);

		Deallocate(Array[index]);
	}

	Deallocate(Array);
}

Block& BlockArray::Add()
{
	if (Array == nullptr)
	{
		Array = HeapAllocate< ptr<Block>>(1);

		Length = 1;
	}
	else
	{
		ptr< ptr<Block>> resizeIntermediary = HeapReallocate(Array, (Length + 1));

		if (resizeIntermediary != nullptr)
		{
			Array = resizeIntermediary;

			Length++;
		}
		else
		{
			perror("Failed to reallocate the global memory array. Exiting...");

			exit(1);
		}
	}

	Array[Length - 1] = HeapAllocate<Block>(1);

	return dref(Array[Length - 1]);
}

Block& BlockArray::LastEntry()
{
	return dref(Array[Length - 1]);
}



// Operator Overloads

ptr<void> operator new(uIntDM _numberDesired)
{
	 return Memory::GlobalAllocate<Byte>(_numberDesired);
}
