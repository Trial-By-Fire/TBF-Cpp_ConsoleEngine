#pragma once



#include "LAL.hpp"



// Aliases (Typedefs)

typedef enum ExecFlags_Def ExecFlags;



// Enumerators

enum ExecFlags_Def
{
	ExecFlags_Sucess = 0,
};



// Functions

ExecFlags EntryPoint(void);

