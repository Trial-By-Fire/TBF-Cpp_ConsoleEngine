// Parent Header
#include "Memory.hpp"



using BlockArray = Memory::BlockArray;
using Block      = BlockArray::Block;



// Static Data

// Private

BlockArray GlobalMemory =
{ nullptr, 0U };



// Functions

// Public

// C-API

void* Memory::Allocate(uIntDM _amountToAllocate)
{
	return malloc(_amountToAllocate);
}

void Memory::Deallocate(void* _memoryToDeallocate)
{
	free(_memoryToDeallocate);

	return;
}

void* Reallocate(void* _memoryToReallocate, uIntDM _sizeDesired)
{
	return realloc(_memoryToReallocate, _sizeDesired);
}

void* Memory::Internal_FormatByFill(void* _memoryAddress, sInt _fillValue, uIntDM _sizeOfData)
{
	return memset(_memoryAddress, _fillValue, _sizeOfData);
}

void* Memory::FormatWithData(void* _memoryAddress, ro void* _dataSource, uIntDM _sizeOfData)
{
	return memcpy(_memoryAddress, _dataSource, _sizeOfData);
}

// Memory Allocation Array

void BlockArray::Add(Block* _memoryAllocation)
{
	if (Array == nullptr)
	{
		Array = (Block**)Allocate(sizeof(Block));

		Length = 1;
	}
	else
	{
		Address resizeIntermediary = Reallocate(Array, Length + 1);

		if (resizeIntermediary != nullptr)
		{
			Array = (Block**)resizeIntermediary;

			Length++;
		}
		else
		{
			perror("Failed to reallocate the global memory array. Exiting...");
		}
	}
}



Block* BlockArray::LastEntry()
{
	return (Block*)&Array[Length - 1];
}

// Memory Management

Memory::Address Memory::Internal_Allocate(uIntDM _sizeOfAllocation)
{
	if (records.Array == nullptr)
	{
		records.Array = (Block**)Allocate(sizeof(Block*));

		records.Length = 1;
	}
	else
	{
		Address resizeIntermediary = Reallocate(records.Array, sizeof(Block*) * (records.Length + 1));

		if (resizeIntermediary != nullptr)
		{
			records.Array = (Block**)resizeIntermediary;

			records.Length++;
		}
		else
		{
			perror("Failed to reallocate the scoped memory array. Exiting...");
		}
	}

	records.Array[records.Length - 1] = (Block*)Allocate(sizeof(Block));

	Block* newBlock = records.Array[records.Length - 1];

	newBlock->Size     = _sizeOfAllocation;
	newBlock->Location = Allocate(_sizeOfAllocation);

	if (newBlock->Location != nullptr)
	{
		return newBlock->Location;
	}
	else
	{
		perror("Failed to scope allocate memory.");

		exit(1);
	}
}

void Memory::Deallocate()
{
	if (records.Array == nullptr) return;

	for (uIntDM index = 0; index < records.Length; index++)
	{
		Deallocate(records.Array[index]->Location);

		Deallocate(records.Array[index]);
	}

	Deallocate(records.Array);

	return;
}

Memory::Address Memory::Internal_GlobalAllocate(uIntDM _sizeOfAllocation)
{
	if (GlobalMemory.Array == nullptr)
	{
		GlobalMemory.Array = (Block**)Allocate(sizeof(Block*));

		GlobalMemory.Length = 1;
	}
	else
	{
		Address resizeIntermediary = Reallocate(GlobalMemory.Array, sizeof(Block*) * (GlobalMemory.Length + 1));

		if (resizeIntermediary != nullptr)
		{
			GlobalMemory.Array = (Block**)resizeIntermediary;

			GlobalMemory.Length++;
		}
		else
		{
			perror("Failed to reallocate the global memory array. Exiting...");
		}
	}

	GlobalMemory.Array[GlobalMemory.Length -1] = (Block*)Allocate(sizeof(Block));

	Block* newBlock = GlobalMemory.Array[GlobalMemory.Length -1];

	newBlock->Size     = _sizeOfAllocation;
	newBlock->Location = Allocate(_sizeOfAllocation);

	if (newBlock->Location != nullptr)
	{
		return newBlock->Location;
	}
	else
	{
		perror("Failed to globally allocate memory.");

		exit(1);
	}
}

Memory::Address Memory::Internal_GlobalReallocate(Address _location, uIntDM _sizeForReallocation)
{
	for (uIntDM index = 0; index < GlobalMemory.Length; index++)
	{
		if (GlobalMemory.Array[index]->Location == _location)
		{
			Address resizeIntermediary = Reallocate(_location, _sizeForReallocation);

			if (resizeIntermediary != nullptr)
			{
				GlobalMemory.Array[index]->Location = resizeIntermediary;

				return GlobalMemory.Array[index]->Location;
			}
			else
			{
				return nullptr;
			}
		}
	}

	return nullptr;
}

void Memory::GlobalDeallocate(void)
{
	for (uIntDM index = 0; index < GlobalMemory.Length; index++)
	{
		Deallocate(GlobalMemory.Array[index]->Location);

		Deallocate(GlobalMemory.Array[index]);
	}

	Deallocate(GlobalMemory.Array);

	return;
}
