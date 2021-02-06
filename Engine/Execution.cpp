// Parent Header
#include "Execution.hpp"



// Includes

#include "Cycler.hpp"
#include "Input.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "State.hpp"



// Functions

void PrepareModules(void)
{
	Cycler  ::LoadModule();
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
	if CompileTime(DebugEnabled)
	{
		try
		{
			// Setup engine components.

			PrepareModules();

			PrintStartMessage();

			// Core Engine Loop

			Cycler::Initialize();

			// Shutdown

			UnloadModules();

			return ExitCode::Success;
		}
		catch (ro Exception& _exception)
		{
			cerr << _exception.what() << endl;

			exit(1);
		}
	}
	else
	{
		// Setup engine components.

		PrepareModules();

		PrintStartMessage();

		// Core Engine Loop

		Cycler::Initialize();

		// Shutdown

		UnloadModules();

		return ExitCode::Success;
	}
}
