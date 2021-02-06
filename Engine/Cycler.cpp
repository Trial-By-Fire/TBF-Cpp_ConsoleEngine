// Parent Header
#include "Cycler.hpp"


// Includes
#include "Input.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "Timing.hpp"
#include "State.hpp"




// Static Data

// Private

bool Exist = false;   // Sentinel value use to exist core engine loop.



// Functions

// Public

void Cycler::Initialize(void)
{
	Exist = true;

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

void Cycler::Lapse()
{
	Exist = false;
}
