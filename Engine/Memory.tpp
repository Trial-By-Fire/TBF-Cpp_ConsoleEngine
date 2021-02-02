// Parent Header
// Memory.hpp




template<typename Type>
unbound ptr<Type> Memory::HeapAllocate(uIntDM _numberToAllocate)
{
	return RCast<Type>(malloc(sizeof(Type) * _numberToAllocate));
};

template<typename Type>
ptr<Type> Memory::HeapReallocate(ptr<Type> _memoryToReallocate, uIntDM _numberDesired)
{
	return RCast<Type>(realloc(_memoryToReallocate, _numberDesired * sizeof(Type)) );
}

template<typename Type>
ptr<void> Memory::FormatByFill(ptr<Type> _memoryAddress, sInt _fillValue, uIntDM _sizeOfData)
{
	return memset(_memoryAddress, _fillValue, _sizeOfData * sizeof(Type));
}

template<typename Type>
ptr<void> Memory::FormatWithData(ptr<Type> _memoryAddress, ptr<ro Type> _dataSource, uIntDM _sizeOfData)
{
	return memcpy(_memoryAddress, _dataSource, _sizeOfData * sizeof(Type));
}


template<typename Type>
ptr<Type> Memory::Allocate(uIntDM _numberToAllocate)
{
	BlockArray::Block& newBlock = records.Add();

	newBlock.Size     = sizeof(Type);
	newBlock.Location = HeapAllocate<Type>(_numberToAllocate);

	if (newBlock.Location != nullptr)
	{
		return RCast<Type>(newBlock.Location);
	}
	else
	{
		perror("Failed to scope allocate memory.");

		exit(1);
	}
}

template<typename Type>
ptr<Type> Memory::GlobalAllocate(uIntDM _numberToAllocate)
{
	return GlobalMemory.Allocate<Type>(_numberToAllocate);
}

template<typename Type>
ptr<Type> Memory::GlobalReallocate(ptr<Type> _location, uIntDM _sizeForReallocation)
{
	for (uIntDM index = 0; index < GlobalMemory.records.Length; index++)
	{
		if (GlobalMemory.records.Array[index]->Location == _location)
		{
			ptr<Type> resizeIntermediary = HeapReallocate(_location, _sizeForReallocation);

			if (resizeIntermediary != nullptr)
			{
				GlobalMemory.records.Array[index]->Location = resizeIntermediary;

				return RCast<Type>(GlobalMemory.records.Array[index]->Location);
			}
			else
			{
				return nullptr;
			}
		}
	}

	return nullptr;
}
