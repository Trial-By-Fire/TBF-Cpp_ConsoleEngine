// Parent Header
#include "Cycler.hpp"


// Includes
#include "C_STL.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "Timing.hpp"
#include "State.hpp"




// Static Data

// Private

bool Exist;   // Sentinel value use to exist core engine loop.



// Functions

// Public

void Cycler::Lapse(void)
{
	Exist = false;
}

void Cycler::Initialize(void)
{
	while (Exist == true)
	{
		Timing::TakeInitialSnapshot();

		// Update Input

		Input::Update();

		// Update State

		State::GetEngineState()->Update();

		// Update Renderer

		Renderer::Update();

		// Update Timing

		Timing::TakeEndingSnapshot();

		Timing::Update();
	}
}

void Cycler::LoadModule(void)
{
	Exist = true;
}

