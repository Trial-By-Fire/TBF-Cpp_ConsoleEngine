// Parent Header
// Memory.hpp



// C-API

template<typename Type>
static ptr<Type> Memory::HeapAllocate(uIntDM _numberToAllocate)
{
	return new Type[_numberToAllocate];
};

template<typename Type>
ptr<Type> Memory::HeapReallocate(ptr<Type> _memoryToReallocate, uIntDM _originalNum, uIntDM _numberDesired)
{
	ptr<Type> resizeIntermediary = new Type[_numberDesired];

	if (resizeIntermediary == nullptr) 
		return nullptr;

	FormatWithData(resizeIntermediary, _memoryToReallocate, _originalNum);

	delete[] _memoryToReallocate;

	return resizeIntermediary;
}

template<typename Type>
ptr<void> Memory::FormatByFill(ptr<Type> _memoryAddress, Type& _fillValue, uIntDM _sizeOfData)
{
	//return memset(_memoryAddress, _fillValue, _sizeOfData * sizeof(Type));
	return fill_n(_memoryAddress, _sizeOfData, _fillValue);
}

template<typename Type>
ptr<void> Memory::FormatWithData(ptr<Type> _memoryAddress, ptr<const Type> _dataSource, uIntDM _sizeOfData)
{
	//return memcpy(_memoryAddress, _dataSource, _sizeOfData * sizeof(Type));
	return copy_n(_dataSource, _sizeOfData, _memoryAddress);
}

// Memory Management

template<typename Type>
ptr<Type> Memory::Allocate(uIntDM _numberToAllocate)
{
	records.push_back(Block());

	Block& newBlock = records.back();

	newBlock.resize(_numberToAllocate * sizeof(Type));

	return RCast<Type>(newBlock.data());
}

template<typename Type>
ptr<Type> Memory::GlobalAllocate(uIntDM _numberToAllocate)
{
	return GlobalMemory.Allocate<Type>(_numberToAllocate);
}

template<typename Type>
ptr<Type> Memory::GlobalReallocate(ptr<Type> _location, uIntDM _sizeForReallocation)
{
	for (auto& block : GlobalMemory.records)
	{
		if (RCast<Type>(block.data()) == _location)
		{
			block.resize(_sizeForReallocation * sizeof(Type));

			return RCast<Type>(block.data());
		}
	}

	return nullptr;
}

