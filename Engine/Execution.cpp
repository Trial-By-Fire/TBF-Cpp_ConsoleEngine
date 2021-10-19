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
	Renderer::LoadModule();
	State   ::LoadModule();
}

void PrintStartMessage(void)
{
	Renderer::WriteToLog(L"TBF C++ Engine");
}

void UnloadModules(void)
{
	Renderer::UnloadModule();
}

// Entry Point

ExitCode EntryPoint(void)
{
	if constexpr(DebugEnabled)
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
		catch (const Exception& _exception)
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
