#pragma once



// Includes

#include "LAL.hpp"



// Aliases (Typedefs)

using CString_13   = char[13];
using CTS_CString  = char[];
using WideChar     = wchar_t;
using CTS_CWString = wchar_t[];



namespace CString
{
}



ptr<ro WideChar> operator""_wc(ptr<ro wchar_t> _um, uIntDM _umSize);
