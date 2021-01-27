// Parent Header
#include "Memory.hpp"



// Static Data

// Private

MemoryBlockArray GlobalMemory =
{ NULL, 0U };



// Functions

// Public

// C-API

void* AllocateMemory(size_t _amountToAllocate)
{
	return malloc(_amountToAllocate);
}

void Deallocate(void* _memoryToDeallocate)
{
	free(_memoryToDeallocate);

	return;
}

void* Reallocate(void* _memoryToReallocate, size_t _sizeDesired)
{
	return realloc(_memoryToReallocate, _sizeDesired);
}

void* Internal_Memory_FormatByFill(void* _memoryAddress, sInt _fillValue, size_t _sizeOfData)
{
	return memset(_memoryAddress, _fillValue, _sizeOfData);
}

void* Memory_FormatWithData(void* _memoryAddress, const void* _dataSource, size_t _sizeOfData)
{
	return memcpy(_memoryAddress, _dataSource, _sizeOfData);
}

// Memory Allocation Array

void MemoryBlockArray_Add(MemoryBlockArray* _memoryArray, MemoryBlock* _memoryAllocation)
{
	if (_memoryArray->Array == NULL)
	{
		_memoryArray->Array = (MemoryBlock**)AllocateMemory(sizeof(MemoryBlock));

		_memoryArray->Length = 1;
	}
	else
	{
		Address resizeIntermediary = Reallocate(_memoryArray->Array, _memoryArray->Length + 1);

		if (resizeIntermediary != NULL)
		{
			_memoryArray->Array = (MemoryBlock**)resizeIntermediary;

			_memoryArray->Length++;
		}
		else
		{
			perror("Failed to reallocate the global memory array. Exiting...");
		}
	}
}

MemoryBlock* MemoryBlockArray_LastEntry(MemoryBlockArray* _memoryArray)
{
	return (MemoryBlock*)&_memoryArray->Array[_memoryArray->Length - 1];
}

// Memory Management

Address Internal_ScopedAllocate(MemoryBlockArray* _scopedMemory, size_t _sizeOfAllocation)
{
	if (_scopedMemory->Array == NULL)
	{
		_scopedMemory->Array = (MemoryBlock**)AllocateMemory(sizeof(MemoryBlock*));

		_scopedMemory->Length = 1;
	}
	else
	{
		Address resizeIntermediary = Reallocate(_scopedMemory->Array, sizeof(MemoryBlock*) * (_scopedMemory->Length + 1));

		if (resizeIntermediary != NULL)
		{
			_scopedMemory->Array = (MemoryBlock**)resizeIntermediary;

			_scopedMemory->Length++;
		}
		else
		{
			perror("Failed to reallocate the scoped memory array. Exiting...");
		}
	}

	_scopedMemory->Array[_scopedMemory->Length - 1] = (MemoryBlock*)AllocateMemory(sizeof(MemoryBlock));

	MemoryBlock* newBlock = _scopedMemory->Array[_scopedMemory->Length - 1];

	newBlock->Size     = _sizeOfAllocation;
	newBlock->Location = AllocateMemory(_sizeOfAllocation);

	if (newBlock->Location != NULL)
	{
		return newBlock->Location;
	}
	else
	{
		perror("Failed to scope allocate memory.");

		exit(1);
	}
}

void ScopedDeallocate(MemoryBlockArray* _scopedMemory)
{
	for (size_t index = 0; index < _scopedMemory->Length; index++)
	{
		Deallocate(_scopedMemory->Array[index]->Location);

		Deallocate(_scopedMemory->Array[index]);
	}

	Deallocate(_scopedMemory->Array);

	return;
}

Address Internal_GlobalAllocate(size_t _sizeOfAllocation)
{
	if (GlobalMemory.Array == NULL)
	{
		GlobalMemory.Array = (MemoryBlock**)AllocateMemory(sizeof(MemoryBlock*));

		GlobalMemory.Length = 1;
	}
	else
	{
		Address resizeIntermediary = Reallocate(GlobalMemory.Array, sizeof(MemoryBlock*) * (GlobalMemory.Length + 1));

		if (resizeIntermediary != NULL)
		{
			GlobalMemory.Array = (MemoryBlock**)resizeIntermediary;

			GlobalMemory.Length++;
		}
		else
		{
			perror("Failed to reallocate the global memory array. Exiting...");
		}
	}

	GlobalMemory.Array[GlobalMemory.Length -1] = (MemoryBlock*)AllocateMemory(sizeof(MemoryBlock));

	MemoryBlock* newBlock = GlobalMemory.Array[GlobalMemory.Length -1];
		
	newBlock->Size     = _sizeOfAllocation;
	newBlock->Location = AllocateMemory(_sizeOfAllocation);

	if (newBlock->Location != NULL)
	{
		return newBlock->Location;
	}
	else
	{
		perror("Failed to globally allocate memory.");

		exit(1);
	}
}

Address Internal_GlobalReallocate(Address _location, size_t _sizeForReallocation)
{
	for (size_t index = 0; index < GlobalMemory.Length; index++)
	{
		if (GlobalMemory.Array[index]->Location == _location)
		{
			Address resizeIntermediary = Reallocate(_location, _sizeForReallocation);

			if (resizeIntermediary != NULL)
			{
				GlobalMemory.Array[index]->Location = resizeIntermediary;

				return GlobalMemory.Array[index]->Location;
			}
			else
			{
				return NULL;
			}
		}
	}

	return NULL;
}

void GlobalDeallocate(void)
{
	for (size_t index = 0; index < GlobalMemory.Length; index++)
	{
		Deallocate(GlobalMemory.Array[index]->Location);

		Deallocate(GlobalMemory.Array[index]);
	}

	Deallocate(GlobalMemory.Array);

	return;
}
