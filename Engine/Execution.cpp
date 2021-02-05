// Parent Header
#include "Execution.hpp"



// Includes

#include "C_STL.hpp"
#include "Cycler.hpp"
#include "Input.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "State.hpp"



// Functions

void PrepareModules(void)
{
	Cycler  ::LoadModule();
	Timing  ::LoadModule();
	Input   ::LoadModule();
	Renderer::LoadModule();
	State   ::LoadModule();
}

void PrintStartMessage(void)
{
	Renderer::WriteToLog(L"TBF C++ Engine");

	Renderer::WriteToLog(L"" );

	Renderer::WriteToLog(L"Version: Phase 18");
}

void UnloadModules(void)
{
	Renderer::UnloadModule();
}

// Entry Point

ExitCode EntryPoint(void)
{
	// Setup engine components.

	PrepareModules();

	PrintStartMessage();	

	// Core Engine Loop

	Cycler::Initialize();
	
	UnloadModules();

	/*try
	{
		Memory::GlobalDeallocate();
	}
	catch (const std::exception& _what)
	{
		
	}*/
	

	return ExitCode::Success;
}
