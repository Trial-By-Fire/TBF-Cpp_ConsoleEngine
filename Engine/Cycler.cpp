// Parent Header
#include "Cycler.hpp"


// Includes
#include "C_STL.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "Timing.hpp"
#include "State.hpp"




namespace Cycler
{
	// Static Data

	// Private

	bool Exist;   // Sentinel value use to exist core engine loop.



	// Functions

	// Public

	void Lapse(void)
	{
		Exist = false;
	}

	void Initialize(void)
	{
		while (Exist == true)
		{
			Timing::TakeInitialSnapshot();

			// Update Input

			Input::Update();

			// Update State

			State::Update();

			// Update Renderer

			Renderer::Update();

			// Update Timing

			Timing::TakeEndingSnapshot();

			Timing::Update();
		}
	}

	void LoadModule(void)
	{
		Exist = true;
	}
}

