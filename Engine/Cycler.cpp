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

void Cycler_Lapse(void)
{
	Exist = false;
}

void Cycler_Initialize(void)
{
	while (Exist == true)
	{
		Timing_TakeInitialSnapshot();

		// Update Input

		Input_Update();

		// Update State

		State_Update();
		
		// Update Renderer

		//Stack()

			//ro Ptr(InputData   ) inputContext    = Input_GetContext   ();
			//ro Ptr(RendererData) rendererContext = Renderer_GetContext();
			//ro Ptr(TimingData  ) timingContext   = Timing_GetContext  ();

		//Renderer_WriteToPersistentSection(1, L"Tick Elapsed        : %llu" , timingContext  ->Cycle_TicksElapsed);
		//Renderer_WriteToPersistentSection(2, L"Timer      (Seconds): %.7Lf", rendererContext->RefeshTimer       );
		//Renderer_WriteToPersistentSection(3, L"Delta Time (Seconds): %.7Lf", timingContext  ->DeltaTime         );

		Renderer_Update();

		// Update Timing

		Timing_TakeEndingSnapshot();

		Timing_Update();
	}
}

void Cycler_LoadModule(void)
{
	Exist = true;
}

void Cycler_Quit(void)
{
	Exist = false;
}



// Private

bool ShouldQuit(void)
{
	return false;
}
