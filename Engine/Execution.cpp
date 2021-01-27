// Parent Header
#include "Execution.hpp"



// Includes

#include "C_STL.hpp"
#include "CString.hpp"
#include "Cycler.hpp"
#include "Input.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "State.hpp"



// Functions

void PrepareModules(void)
{
	Cycler_LoadModule  ();
	CString_LoadModule ();
	Timing_LoadModule  ();
	Input_LoadModule   ();
	Renderer_LoadModule();
	State_LoadModule   ();
}

void PrintStartMessage(void)
{
	Renderer_WriteToLog((WideChar*)L"TBF C Engine");

	Renderer_WriteToLog((WideChar*)L"");

	Renderer_WriteToLog((WideChar*)L"Version: Phase 14");
}

void UnloadModules(void)
{
	Renderer_UnloadModule();
}

// Entry Point

ExecFlags EntryPoint(void)
{
	// Setup engine components.

	PrepareModules();

	PrintStartMessage();	

	// Core Engine Loop

	Cycler_Initialize();
	
	UnloadModules();

	GlobalDeallocate();

	return ExecFlags_Sucess;
}
