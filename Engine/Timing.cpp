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

ro Timing::Data& Timing::GetContext(void)
{
	return Context;
}

void Timing::LoadModule(void)
{
	InitalizeData();
}

void Timing::TakeInitialSnapshot(void)
{
	QueryPerformanceCounter(getPtr(Context.TimeSnapshot_Initial));
}

void Timing::TakeEndingSnapshot(void)
{
	QueryPerformanceCounter(getPtr(Context.TimeSnapshot_End));
}

void Timing::Update(void)
{
	Context.Cycle_TicksElapsed = Context.TimeSnapshot_End.QuadPart - Context.TimeSnapshot_Initial.QuadPart;

	Context.Cycle_Microseconds = SCast<float64>( Context.Cycle_TicksElapsed * TickToMicroseconds );

	Context.Cycle_Microseconds = Context.Cycle_Microseconds / SCast<float64>(Context.TimeFrequency.QuadPart);

	Context.DeltaTime = SCast<float64>(Context.Cycle_TicksElapsed) / SCast<float64>(MicrosecondToSecond);

	Renderer::ProcessTiming();
}

// Private

void Timing::InitalizeData(void)
{
	QueryPerformanceFrequency(getPtr(Context.TimeFrequency));

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
	if (Float64_ApproxEqual(Timing::Context.DeltaTime, 0.000001) || Float64_ApproxLess(Timing::Context.DeltaTime, 0.000001))
	{
		Elapsed = Elapsed + 0.000001;

		return;
	}
	else
	{
		Elapsed = Elapsed + Timing::Context.DeltaTime;

		return;
	}
}

