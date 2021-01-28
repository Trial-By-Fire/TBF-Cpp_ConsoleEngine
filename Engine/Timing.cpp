// Parent Header
#include "Timing.hpp"



// Includes
#include "Memory.hpp"
#include "Renderer.hpp"



// Static Data

// Private

Timing::Data Timing::Context;



// Functions

// Public

ro Timing::Data* Timing::GetContext(void)
{
	return &Context;
}

void Timing::LoadModule(void)
{
	InitalizeData();
}

void Timing::TakeInitialSnapshot(void)
{
	QueryPerformanceCounter(&Context.TimeSnapshot_Initial);
}

void Timing::TakeEndingSnapshot(void)
{
	QueryPerformanceCounter(&Context.TimeSnapshot_End);
}

void Timing::Update(void)
{
	Context.Cycle_TicksElapsed = Context.TimeSnapshot_End.QuadPart - Context.TimeSnapshot_Initial.QuadPart;

	Context.Cycle_Microseconds = (float64)(Context.Cycle_TicksElapsed * TickToMicroseconds);
	Context.Cycle_Microseconds = Context.Cycle_Microseconds / (float64)Context.TimeFrequency.QuadPart;

	Context.DeltaTime = (float64)Context.Cycle_TicksElapsed / (float64)MicrosecondToSecond;

	Renderer::ProcessTiming(Context.DeltaTime);
}

// Private

void Timing::InitalizeData(void)
{
	Context.Cycle_TicksElapsed = 0;
	Context.Cycle_Microseconds = 0.0L;
	Context.DeltaTime          = 0.0L;

	QueryPerformanceFrequency(&Context.TimeFrequency);

	return;
}



// Timer

bool Timer::Ended()
{
	return
		Float64_ApproxGreater(Elapsed, EndTime) ||
		Float64_ApproxEqual  (Elapsed, EndTime);
}

void Timer::Reset()
{
	Elapsed = 0.0L;
}

void Timer::Tick()
{
	if (Float64_ApproxEqual(Timing::Context.DeltaTime, 0.000001L) || Float64_ApproxLess(Timing::Context.DeltaTime, 0.000001L))
	{
		Elapsed = Elapsed + 0.000001L;

		return;
	}
	else
	{
		Elapsed = Elapsed + Timing::Context.DeltaTime;

		return;
	}
}

