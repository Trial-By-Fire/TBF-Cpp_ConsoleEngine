#pragma once


// Includes

#include "LAL.hpp"
#include "Memory.hpp"



// Aliases (Typedefs)

typedef struct CString_Def CString;

typedef char CString_13[13];

typedef char CTS_CString[];

typedef wchar_t WideChar;

typedef wchar_t CTS_CWString[];



// Structures

struct CString_Def
{
	char* Array;

	uInt64 Length;
};



// Constants

#define SizeOf_CString \
	sizeof(CString)



// Functions

void CString_LoadModule(void);
