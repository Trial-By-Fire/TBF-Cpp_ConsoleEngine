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

			//Stack()

			//ro Ptr(Data   ) inputContext    = Input_GetContext   ();
			//ro Ptr(Data) rendererContext = GetContext();
			//ro Ptr(Data  ) timingContext   = GetContext  ();

			//WriteToPersistentSection(1, L"Tick Elapsed        : %llu" , timingContext  ->Cycle_TicksElapsed);
			//WriteToPersistentSection(2, L"Timer      (Seconds): %.7Lf", rendererContext->RefeshTimer       );
			//WriteToPersistentSection(3, L"Delta Time (Seconds): %.7Lf", timingContext  ->DeltaTime         );

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

	void Quit(void)
	{
		Exist = false;
	}



	// Private

	bool ShouldQuit(void)
	{
		return false;
	}
}

