// Parent Header
#include "Memory.hpp"



// Static Data

// Private

Memory Memory::GlobalMemory;



// Functions

// Public

void Memory::Deallocate(ptr<void> _memoryToDeallocate)
{
	delete[] _memoryToDeallocate;
}