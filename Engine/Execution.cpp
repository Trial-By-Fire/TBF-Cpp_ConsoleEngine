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
	Cycler  ::LoadModule();
	CString ::LoadModule();
	Timing  ::LoadModule();
	Input   ::LoadModule();
	Renderer::LoadModule();
	State   ::LoadModule();
}

void PrintStartMessage(void)
{
	Renderer::WriteToLog((WideChar*)L"TBF C Engine");

	Renderer::WriteToLog((WideChar*)L"");

	Renderer::WriteToLog((WideChar*)L"Version: Phase 14");
}

void UnloadModules(void)
{
	Renderer::UnloadModule();
}

// Entry Point

ExecFlags EntryPoint(void)
{
	// Setup engine components.

	PrepareModules();

	PrintStartMessage();	

	// Core Engine Loop

	Cycler::Initialize();
	
	UnloadModules();

	Memory::GlobalDeallocate();

	return ExecFlags_Sucess;
}
